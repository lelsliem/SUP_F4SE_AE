-- SUP F4SE — xmake build (Phase 0, see MODERNIZATION_PLAN.md)
--
-- Requirements:
--   xmake >= 3.0.0, Visual Studio 2022 (v143), Fallout 4 / F4SE runtime 1.11.221
--
-- Build:     xmake f -m releasedbg -y && xmake build
-- VS2022:    xmake project -k vsxmake
-- clangd:    xmake project -k compile_commands

set_xmakever("3.0.0")

set_project("SUP_F4SE")

-- Platform pin: when xmake runs from Git Bash / MSYS2 (MSYSTEM=MINGW64 is set),
-- it can misdetect the platform as "mingw" and try to build the spdlog package
-- with a nonexistent gcc, failing with "cannot get program for cc". Force the
-- platform to windows on a Windows host so the project configures identically
-- from any shell. A command-line `xmake f -p <plat>` still overrides this.
if os.host() == "windows" then
    set_config("plat", "windows")
end

set_version("0.7.7")

set_languages("c++23")
set_encodings("utf-8")

add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- Static CRT: /MT (release), /MTd (debug). Applied at project scope so the vendored
-- CommonLibF4 / commonlib-shared static libs are built with the same runtime.
if is_mode("release") then
    set_runtimes("MT")
else
    set_runtimes("MTd")
end

-- Vendored CommonLibF4 (its lib/commonlib-shared submodule is populated).
includes("Depends/commonlibf4-5ba1928a32c6ccd5690164b79066fc2f5dcb5c65")

target("SUP_F4SE", function()
    set_kind("shared")

    add_rules("commonlibf4.plugin", {
        name        = "SUP F4SE",
        author      = "SUP",
        description = "SUP F4SE - Papyrus function library and settlement tools",
        version     = "0.7.7",
    })

    add_files("src/main.cpp")
    add_files("src/SUP_F4SE.cpp")
    add_files("src/CrashDump.cpp")
    add_files("src/ui/inputbox.cpp")

    -- Real MinHook (vendored) for function-entry detours — handles instruction boundaries,
    -- 64-bit addressing and thunks that a hand-rolled 5-byte patch cannot.
    add_files("Depends/minhook/src/hook.c")
    add_files("Depends/minhook/src/buffer.c")
    add_files("Depends/minhook/src/trampoline.c")
    add_files("Depends/minhook/src/hde/hde64.c")

    add_includedirs("include", { public = true })
    add_includedirs("Depends", { public = true })  -- minhook/include/MinHook.h

    -- PCH: keeps CommonLibF4 + std headers out of every TU's critical path
    set_pcxxheader("src/PCH.h")

    add_defines("NOMINMAX", "UNICODE", "_UNICODE")

    -- GDI+/GDI are used directly by the screenshot/path Papyrus modules.
    add_syslinks("gdiplus", "gdi32")
end)

-- Assemble the complete mod folder from the built DLL + the blessed PEX/INI/fomod.
-- The Papyrus compiler output is non-deterministic, so we ship the *blessed*
-- Scripts/SUP_F4SE.pex unchanged rather than recompiling (see tools/compile/README.md).
--
--   xmake package_mod            package after an existing build
--   xmake f -m releasedbg -y && xmake package_mod    build + package in one go
--
task("package_mod", function()
    set_menu {
        usage = "xmake package_mod",
        description = "Assemble the full mod (DLL + PEX + INI + fomod) into Package/ and deploy to MO2",
    }

    on_run(function()
        import("core.project.config")
        import("core.project.task")
        import("core.project.project")

        local projectdir = os.projectdir()

        task.run("build")

        local dll = path.join(config.builddir(), config.plat(), config.arch(), config.mode(), "SUP_F4SE.dll")
        if not os.isfile(dll) then
            raise("SUP_F4SE.dll not found at %s", dll)
        end

        -- Vortex-ready layout: Data/ (F4SE/Plugins + Scripts) and fomod/.
        local pkg = path.join(projectdir, "Package")
        os.mkdir(path.join(pkg, "Data", "F4SE", "Plugins"))
        os.mkdir(path.join(pkg, "Data", "Scripts"))
        os.mkdir(path.join(pkg, "fomod"))

        os.cp(dll, path.join(pkg, "Data", "F4SE", "Plugins", "SUP_F4SE.dll"))
        os.cp(path.join(projectdir, "SUP_F4SE.ini"), path.join(pkg, "Data", "F4SE", "Plugins", "SUP_F4SE.ini"))
        os.cp(path.join(projectdir, "Scripts", "SUP_F4SE.pex"), path.join(pkg, "Data", "Scripts", "SUP_F4SE.pex"))
        -- Ship the source so other mods can compile against SUP.
        os.mkdir(path.join(pkg, "Data", "Scripts", "Source", "User"))
        os.cp(path.join(projectdir, "SUP_F4SE.psc"), path.join(pkg, "Data", "Scripts", "Source", "User", "SUP_F4SE.psc"))
        for _, src in ipairs(os.files(path.join(projectdir, "fomod", "*"))) do
            os.cp(src, path.join(pkg, "fomod", path.filename(src)))
        end
        cprint("${bright}Packaged -> ${underline}%s${clear}", pkg)

        -- Emit a Vortex-ready zip of the Package folder.
        local archive = import("utils.archive")
        local zip = path.join(projectdir, "SUP_F4SE_v" .. (project.version() or "0.7.7") .. ".zip")
        os.tryrm(zip)
        local olddir = os.cd(pkg)
        local zfiles = os.files("**")
        os.cd(olddir)
        if #zfiles > 0 then
            archive.archive(zip, zfiles, { curdir = pkg })
            cprint("${bright}Zipped -> ${underline}%s${clear}", zip)
        end

        -- Deploy to the MO2 mod folder (flat Data-root layout). Overridable via SUP_MODS_PATH.
        local mods = os.getenv("SUP_MODS_PATH") or "B:/Modding/MO2/Fallout 4/mods"
        local moddir = path.join(mods, "SUP F4SE - Script Utilities Plus")
        if os.isdir(moddir) then
            os.mkdir(path.join(moddir, "F4SE", "Plugins"))
            os.mkdir(path.join(moddir, "Scripts"))
            os.mkdir(path.join(moddir, "Scripts", "Source", "User"))
            -- trycp: warn (not abort) when the game holds the DLL lock.
            local ok = os.trycp(dll, path.join(moddir, "F4SE", "Plugins", "SUP_F4SE.dll"))
            os.trycp(path.join(projectdir, "SUP_F4SE.ini"), path.join(moddir, "F4SE", "Plugins", "SUP_F4SE.ini"))
            os.trycp(path.join(projectdir, "Scripts", "SUP_F4SE.pex"), path.join(moddir, "Scripts", "SUP_F4SE.pex"))
            os.trycp(path.join(projectdir, "SUP_F4SE.psc"), path.join(moddir, "Scripts", "Source", "User", "SUP_F4SE.psc"))
            cprint("${bright}Deployed -> ${underline}%s${clear}", moddir)
            if not ok then
                cprint("${yellow}DLL copy skipped (game is running and holds the file lock) — redeploy after closing the game${clear}")
            end
        else
            cprint("${dim}MO2 mod folder not found (SUP_MODS_PATH=%s) — skipping deploy${clear}", mods)
        end

        cprint("${dim}DLL: %s${clear}", dll)
        cprint("${dim}PEX: %s (blessed hash must match s_CurrentPEXHash)${clear}", path.join(projectdir, "Scripts", "SUP_F4SE.pex"))
    end)
end)

