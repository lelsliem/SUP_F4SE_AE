# SUP F4SE — Modernization Plan

**Documentation of the CommonLibF4 modernization.** This plan records every phase
of the rebuild — what was changed, why, and what was removed — and is the
companion to [README.md](README.md). The modernization itself was performed by a
**Codebuff coding agent** at the request of the repo owner; upstream credit and
the licensing situation are covered in the README.

**Status:** ALL PHASES COMPLETE. Release build (`e658e05a` — test-harness-stripped) ready in `Package/` + zip; only user soak-testing remains.
**Target runtime:** Fallout 4 1.11.221 (AE) — the AE port is complete and the
`SUP_F4SE_AE my updated to 1.11.221/` reference folder has been deleted.
**Target toolchain:** MSVC v143, `/std:c++23`, static runtime `/MT` (Release), `/MTd` (Debug).
**Dependencies:** CommonLibF4 (already vendored in `Depends/commonlibf4-…`) instead of the
F4SE source tree + `common` + Boost 1.84. spdlog (pulled in by CommonLibF4) instead of `gLog`/`IDebugLog`.
Keep `json.hpp` (nlohmann) as-is — it's header-only, self-contained, and works fine.

---

## 0. Progress

| Phase | Status | Notes |
|---|---|---|
| 0 — xmake + CommonLibF4 + entry point | ✅ Done | `xmake.lua`, `src/PCH.h`, `src/main.cpp` (F4SE::Init + messaging listener). `xmake build` produces `build/windows/x64/releasedbg/SUP_F4SE.dll` exporting `F4SEPlugin_Load` + `F4SEPlugin_Version`. CommonLibF4 + commonlib-shared build statically with `/MT`. `build.bat` now drives xmake. |
| 1 — Logging → spdlog | ✅ Done | `include/sup/util/Log.h` printf-style shim routing `_MESSAGE`/`_DMESSAGE`/`_ERROR` to spdlog. `gLog`/`IDebugLog` removed; `bDebugMode` → `sup::log::set_debug_enabled()`. |
| 2 — Drop Boost | ✅ Done | `include/sup/util/string.hpp`; all 93 `boost::` call sites replaced; Boost out of the build. |
| 3 — Core F4SE API swap | ✅ Done | `include/sup/compat/F4SECompat.h` reimplements the F4SE surface (`RelocAddr`/`RelocPtr`, `CALL_MEMBER_FN`, `LookupFormByID`, `g_dataHandler`/`g_player`/`g_ui` globals, `VMArray`/`VMValue`/`VMVariable`, `NativeFunctionN`, `DECLARE_STRUCT`, `tArray`) on CommonLibF4's `RE::`/`REL::`/`F4SE::` types. Single-TU build (`src/SUP_F4SE.cpp`) loads in-game, registers all Papyrus functions, events fire. |
| 4 — Address library | ✅ Done | Hard-coded `0x…` offsets → `REL::ID(n)`; `SUP_AddressLib.h/.cpp` deleted; workshop/power-grid cluster IDs (2194996–2195088) verified against the 1.11.221 DB with a runtime `VerifyWorkshopIDs()` self-check logged at GameDataReady. The 0x4CAF53 crash was analyzed across 56 dumps and proven a vanilla-game NULL-global bug, not SUP. |
| 5 — Module-by-module | ✅ Done (re-scoped) | See §5.5 below: every logged transition stub and Phase-5 TODO marker resolved to a real CommonLibF4-backed implementation (8 stubs + 5 TODOs). The header→.cpp split and namespacing were deliberately deferred — the single-TU architecture was kept because it is stable and the stubs were the actual functional gaps. |
| 6 — Subsystems | 🟡 Mostly done | Hooks all install via REL (18 hooks logged at startup); events/serialization on F4SE interfaces; Building Bridges converted (incl. `f_BB_Refresh_MessageMenu` → real `UIMessageQueue::AddMessage`). |
| 7 — Cleanup & modern C++ | 🟢 Done | Dead files deleted (`F4SE_Compat.h` placeholder, `inputbox_stub.cpp`, `Tomm_fn_Armor/Weapons/MP3`, `EventCaller.h`, `EventScriptCaller.h`, dead `third_party/Span.h`, disabled `REL::ID(0)` stubs `ItemInterruptsPower`/`ItemContainsNode`, superseded `package_mod.bat`, and ~20 one-off migration tools incl. the 63MB disassembly dump); crash handler hardened (`SUP_Crash_<pid>.txt` heartbeat + dumps; filters the benign vanilla AV — `Fallout4.exe+0x4CAF53` reading `0x50` — **and** skips dumping inside SEH-guarded regions via a thread-local `g_SehGuardDepth`, so handled connect-point faults no longer leave false-alarm dumps; clean sessions are fully dump-free); CommonLibF4 `structure_wrapper` null-termination bug patched (all 18 Papyrus structs resolve, zero type-info errors); `Iter` macro family flagged but kept (740+ uses, stable). `std::string_view`/`std::span`/`enum class` conversion — **dropped** (pointless: no behavioral gain, code is stable, and CommonLibF4's RE:: types already use modern C++). |
| 8 — Regression | 🟢 Green (release) | Repeated in-game test sessions: wiring, power grid, events, cosave, NISTRON placement/wiring — clean. TimeMultiplier global fixed (`RE::BSTimer`), knock-down check reimplemented over `ActorState::knockState`, cross-cell `MoveRefrToPosition` restored via `MoveRefToNewSpace` (REL ID 2201149). Vanilla launch hang + 0x4CAF53 AV confirmed independent of SUP and filtered by the crash handler. The dev self-test harness was removed for release: native battery (`bSelfTest` + `SelfTest.h` + 64-check battery), `SUP_STRUCT_PRE` startup diagnostic, `SUPSelfTest.esp`, holotape scripts, and `tools/gen_holotape_esp.py` are all deleted. Final packaged build: DLL `e658e05a` (test-harness-stripped), `bDebugMode=0`, PEX `ee2ea112` (blessed), INI ships `bScrapCrashFix`/`bTermLinkFix`/`bDisableAllHooks`. |

Both new headers were runtime-verified with a temporary self-test TU compiled and executed via xmake (printf→spdlog logging with correct source locations, level filtering, and all `sup::string` assertions).

---

## 1. Where we are now

The plugin is a **single translation unit**: `src/SUP_F4SE.cpp` includes ~60 headers, and *every
header defines all its functions inline* (no `#pragma once`, no .cpp/.h split). Consequences:

- **~1800 `_DMESSAGE` calls**, ~66 `_MESSAGE`, 10 `_ERROR` — all through F4SE's `gLog`/`IDebugLog`.
- **93 Boost uses**, but only **9 distinct string helpers** (`to_lower`, `iequals`, `replace_all`,
  `icontains`, `to_lower_copy`, `replace_first`, `replace_last`, `equals`, `contains`).
  Boost 1.84 is otherwise unused — it is trivially replaceable.
- **57 `RelocAddr` / 4 `RelocPtr` / 37 `CALL_MEMBER_FN`**, plus a hand-rolled address library
  (`include/sup/addresses/SUP_AddressLib.h/.cpp`, meh321 DB format) and hard-coded offsets in
  `Tomm_Addresses.h`. CommonLibF4 replaces all of this with `REL::Relocation` + the built-in
  Address Library / sig scanning.
- F4SE API usage throughout: `F4SEPlugin_Query/Load`, `PapyrusNativeFunctions`,
  `DECLARE_STRUCT`, `GetEventDispatcher<T>`, `LookupFormByID`, `g_dataHandler`, `VMArray`/`VMValue`,
  `CALL_MEMBER_FN`-style method calls, hand-rolled game-struct layouts (`Decoding.h`,
  `Tomm_fn_MapMarker.h`, `Tomm_NativeEvents.h`).
- Build was MSBuild (`SUP_F4SE.vcxproj`, only present in the AE folder) against
  `G:/F4SE_Developing_NEW_AFTER_REINSTALL_FINAL/f4se/…` — a hard-coded absolute path that only
  exists on the original dev machine, plus `C:\SUP_F4SE\boost_1_84_0` and `C:\SUP_F4SE\common`.

### Housekeeping (already done)

All project headers now live under `include/sup/<group>/`, vendored single-file libraries under
`include/third_party/`, and `.cpp` files under `src/`. Groups are by function:

| Group | Contents |
|---|---|
| `include/sup/core/` | plugin infrastructure: registration, serialization, console cmds, game loop, INI processing, PEX hash, function declarations |
| `include/sup/events/` | event system, script runners, native event sinks |
| `include/sup/hooks/` | code hooks (`Tomm_Hooks.h`) |
| `include/sup/addresses/` | address library (`SUP_AddressLib.h`, `Tomm_Addresses.h`, `F4SE_Compat.h`) |
| `include/sup/papyrus/general/` | Utility, Math, Strings, Misc, INI, FileFunctions, ModLocalData |
| `include/sup/papyrus/world/` | ObjectReference, Cell, Location, MapMarker, Nif, PowerGrid, Recipies, Armor*, Weapons*, Controller |
| `include/sup/papyrus/media/` | MP3*, Screenshots, Dialogue_Box, Excel, JSON, XML |
| `include/sup/papyrus/online/` | Online, Steam |
| `include/sup/papyrus/events/` | `Tomm_fn_Events.h` |
| `include/sup/buildingbridges/` | Building Bridges sub-mod (engine + Papyrus API) |
| `include/sup/util/` | `InternalFunctions.h`, `Decoding.h`, `AttachedScriptsReader.h`, `inputbox.h` |
| `include/sup/external/` | Steam API wrapper, SUPdater, Pastebin reader, MP3 player |
| `include/third_party/` | `json.hpp`, `SimpleIni.h`, `sha1.hpp`, `OpenXLSX.hpp`, `xml2json.hpp`, `Span.h`, `NanoTimer.h`, `MinHook.h`, `steam_api.h` |
| `src/` | `SUP_F4SE.cpp` (entry point), `addresses/SUP_AddressLib.cpp`, `ui/inputbox.cpp`, `ui/inputbox_stub.cpp` |
| `tools/` | Python address-lib/ID scripts (moved from root) |

All project-internal `#include`s were rewritten to the new paths and verified to resolve.
`SUP_F4SE_AE my updated to 1.11.221/` was **not touched**.

* = dead code (fully commented out): `Tomm_fn_Armor.h`, `Tomm_fn_Weapons.h`, `Tomm_fn_MP3.h`,
`MP3_Player.h`; and orphaned/superseded files `EventCaller.h`, `EventScriptCaller.h`
(replaced by `Tomm_EventScriptCaller.h`). Duplicates to consolidate:
`RegisterFunctions.h` / `Tomm_RegisterFunctions.h` / `Tomm_fn_aRegisterFunctions.h`.

---

## 2. Decisions

### 2.1 Use CommonLibF4 instead of F4SE + common + Boost

- CommonLibF4 **is** the modern replacement for the F4SE source tree and its `common` library
  (RE types, game data, Papyrus VM, serialization, messaging, hooks infrastructure). It is a
  **static** library; the F4SE *runtime* (`f4se_1_11_221.dll`) is still required at game runtime.
- It is **already vendored** in `Depends/` (commit `5ba1928a…`), requires C++23 and xmake, and its
  `commonlibf4.plugin` rule generates the plugin entry/resource boilerplate.
- It removes the hard-coded `G:/…` include, the separate `common` lib, and the hand-rolled
  address library (see 2.3).
- **Boost can be dropped entirely.** Only 9 string helpers are used; replace with `std::`
  algorithms + a tiny `sup::string` helper namespace (see mapping table). CommonLibF4 is strictly
  better here: no boost, spdlog included, version-independent addresses.

### 2.2 Logging → spdlog

CommonLibF4's `commonlib-shared` already depends on **spdlog v1.16.0** (`wchar=true`,
`std_format=true`) and provides `REX::LOG` / `logger::*` macros. No extra dependency to add.
`gLog`/`IDebugLog`/`_MESSAGE` go away (see mapping table).

### 2.3 Addresses → Address Library (built into CommonLibF4)

CommonLibF4 loads the game's **Address Library** database (meh321 `offsets-1.11.221.7z`)
automatically at startup. `REL::ID{ id }.address()` gives a runtime-version-independent address.
This replaces:
- the hard-coded `#define fn_* 0x…` block in `Tomm_Addresses.h`, and
- `SUP_AddressLib` (its `VersionDb` DB reader) — delete both once IDs are migrated.
Keep the `fn_*` macro *names* during the transition by re-pointing them, e.g.
`#define fn_WorkShop_DeleteWorkshopItem REL::ID(12345).address()`, then delete the macros after
Phase 4 and use `REL::Relocation` / `REL::ID` directly.
> The exact IDs come from the Address Library database (`Tools/parse_ng_db.py` + `find_ids.py`
> already exist in `tools/` to look them up).

### 2.4 Build system → xmake

CommonLibF4 and `commonlib-shared` are xmake-native (their `xmake.lua` requires xmake ≥ 3.0).
Using xmake for the plugin means:
- `add_requires("commonlibf4")`-style consumption or direct `add_subdirs` of `Depends/`;
- the `commonlibf4.plugin` rule handles the DLL, exports, and resource (version info) boilerplate;
- `xmake project -k vsxmake` generates the VS solution when you want the IDE;
- `compile_commands.json` for clangd via `xmake project -k compile_commands`.
CMake is a fallback (CommonLibF4 also ships a CMake path via its submodule), but xmake is the
path of least resistance and the one the library is tested with. MSBuild/vcxproj is retired.

---

## 3. Target layout (after migration)

Mirrors the current `include/sup/…` tree; every module gets a declaration header and an
implementation TU.

```
src/
  main.cpp                    # F4SE::Init + Plugin entry (generated file + Load)
  PCH.h                       # precompiled header: F4SE/F4SE.h + project pch
  SUP_Globals.h/.cpp          # consolidated global state (currently scattered in headers)
  SUP_Log.h                   # logger macros (debug/info/warn/error)
  sup/
    papyrus/…/Tomm_fn_*.h     # declarations only
    papyrus/…/Tomm_fn_*.cpp   # implementations
    events/…, hooks/…, addresses/…, core/…, external/…, util/…
  ui/inputbox.cpp
xmake.lua
include/sup/…                 # public headers (declarations only)
include/third_party/…         # unchanged
tools/…                       # unchanged
```

---

## 4. Mapping tables

### 4.1 Logging (F4SE `IDebugLog` → spdlog)

| Old | New | Notes |
|---|---|---|
| `_MESSAGE(fmt, …)` | `logger::info(fmt, …)` | always-on info |
| `_DMESSAGE(fmt, …)` | `logger::debug(fmt, …)` | was gated on `bDebugMode`; spdlog level filters at runtime — remove the manual `if (bDebugMode)` |
| `_DMESSAGESUP(fmt, …)` | `logger::debug(fmt, …)` | same |
| `_ERROR(fmt, …)` | `logger::error(fmt, …)` | |
| `_DMESSAGEADD` | delete | was a no-op/optional debug macro |
| `gLog.OpenRelative(…)`, `SetPrintLevel`, `SetLogLevel`, `Indent/Outdent` | `spdlog::basic_logger` / CommonLibF4 `InitInfo{ .logName = "SUP_F4SE.log" }` | CommonLibF4 `F4SE::Init(…, { .log = true, .logName = … })` handles path/log rotation |

### 4.2 Boost → standard library

| Boost | Replacement |
|---|---|
| `boost::to_lower(s)` | `std::ranges::transform(s, s.begin(), [](char c){ return std::tolower(static_cast<unsigned char>(c)); })` (ASCII paths only) |
| `boost::to_lower_copy(s)` | same on a copy |
| `boost::iequals(a,b)` | `sup::string::iequals` (compare via `std::equal` + `tolower`) or `_stricmp` for C strings |
| `boost::icontains(hay, needle)` | `sup::string::icontains` |
| `boost::contains(hay, needle)` | `hay.find(needle) != std::string::npos` |
| `boost::replace_all(s, old, new)` | `std::regex_replace` or a small loop with `find/replace` (preferred: loop, regex is slow) |
| `boost::replace_first / replace_last` | small `find` + `replace` helper |
| `boost::equals(a,b)` | `a == b` |

All 93 uses collapse into one ~80-line `include/sup/util/string.hpp` (namespace `sup::string`).
After Phase 2, remove Boost from the build entirely.

### 4.3 F4SE / common → CommonLibF4

| Old (F4SE) | New (CommonLibF4) |
|---|---|
| `F4SEPlugin_Query / F4SEPlugin_Load` + `F4SEInterface` | generated `commonlibf4-plugin.cpp` entry + `F4SE::Init(loadIntfc, { .logName = "SUP_F4SE.log" })`, then `F4SE::GetMessagingInterface()->RegisterListener(...)` |
| `gLog` / `IDebugLog.h` | `logger::*` (spdlog) |
| `RelocAddr<T>(0x…)` / `RelocAddr<T>(fn_X)` / `RelocPtr` | `REL::Relocation<T>{ REL::ID(id) }` or `REL::Relocation<std::uintptr_t>(REL::ID(id))` |
| `CALL_MEMBER_FN(obj, Method)(args)` | `REL::Relocation<fn_t> fn{ REL::ID(id) }; fn(obj, args)` (or `obj->Method()` when the member exists in RE::) |
| `LookupFormByID(id)` | `RE::TESDataHandler::GetSingleton()->GetForm(id)` |
| `GetFormByEditorID("X")` | `RE::TESDataHandler::GetSingleton()->LookupFormByEditorID("X")` |
| `GetEventDispatcher<T>()->AddEventSink(&sink)` | `RE::ScriptEventSourceHolder::GetSingleton()->GetEventSource<T>()->AddEventSink(&sink)` |
| `VMArray<T>` / `VMValue` / `VMVariable` | `RE::BSScript::Array<T>` / `RE::BSScript::Variable` / `RE::BSScript::Internal::*` |
| `DECLARE_STRUCT(X, "SUP_F4SE")` | `RE::BSScript::Internal::VirtualMachine` struct registration (`IVirtualMachine::RegisterStruct`) |
| `NativeFunctionN<…>` + `vm->RegisterFunction` | `vm->RegisterFunction(name, script, fn)` with `RE::BSScript::IFunction` — CommonLibF4 provides `RE::BSScript::Internal::VirtualMachine::RegisterFunction` overloads; wrap in one `sup::papyrus::Register(...)` helper to keep the 60+ registration lines mechanical |
| `g_dataHandler`, `g_player`, `g_ui`, `g_inputMgr`, `g_iniSettings` | `RE::TESDataHandler::GetSingleton()`, `RE::PlayerCharacter::GetSingleton()`, `RE::UI::GetSingleton()`, `RE::ControlMap::GetSingleton()` / `RE::PlayerControls::GetSingleton()`, `RE::INISettingCollection::GetSingleton()` |
| `BranchTrampoline` / `trampoline.Write5Call` | `F4SE::GetTrampoline()` / `REL::Trampoline` (xbyak optional) |
| `MinHook` (in `Tomm_Hooks.h`) | CommonLibF4 hooking (`REL::Relocation` + `F4SE::Trampoline`) or keep MinHook temporarily |
| `kExtraData_*`, `BSExtraData`, `BGSKeyword`, … | `RE::BSExtraData`-derived, `RE::BGSKeyword`, … (full RE type surface) |
| `Runtime_DynamicCast`, `RTTI_*` | `RE::BsTypeInfo` / `netimmerse_cast` / `RE::NiPointer` |
| `STATIC_ASSERT(offsetof(...))` hand layouts in `Decoding.h`, `Tomm_fn_MapMarker.h`, `Tomm_NativeEvents.h` | delete — RE:: classes already carry correct 1.11.x offsets; only keep layouts CommonLibF4 lacks (rare) |

### 4.4 Misc

| Item | Action |
|---|---|
| `json.hpp` | keep, vendored at `include/third_party/json.hpp` (works on C++23) |
| `SimpleIni.h` | keep (only needed for a few INI reads; could later switch to CommonLibF4's `REX::INI` option `commonlib_ini`) |
| `sha1.hpp` | keep (PEX hash check); consider `REX::SHA1` if available |
| `OpenXLSX.hpp`, `xml2json.hpp` | keep as-is; note `xml2json.hpp` currently **stubs** (`return "{}"`) |
| `steam_api.h` | keep vendored; note it is a *stub* header — real Steamworks SDK header must be dropped in for a live Steam build |
| `Span.h` | delete once C++23: use `std::span` |
| `NanoTimer.h` | keep (plf::nanotimer) or replace with `std::chrono` |
| `codecvt`/`wstring_convert` (in `InternalFunctions.h`) | replace with `std::filesystem::path` conversions or Win32 `MultiByteToWideChar` (codecvt is deprecated/removed in C++23) |
| `#include "G:/F4SE_…/cmake_pch.hxx"` (in `src/SUP_F4SE.cpp`, `src/ui/inputbox.cpp`) | **delete** — replaced by `src/PCH.h` |

---

## 5. Migration phases

Order matters: get a *buildable* skeleton first, then migrate in slices, keeping the plugin
loadable at the end of each phase.

### Phase 0 — Build skeleton (xmake + CommonLibF4 + entry point)
1. Add `xmake.lua` at root (sketch in §6): `set_languages("c++23")`,
   `set_runtimes("MT")` for Release / `MTd` for Debug, `add_rules("mode.releasedbg")`,
   link `Depends/commonlibf4-…` (`add_subdirs` or `add_requires`), apply
   `commonlibf4.plugin` rule, PCH `src/PCH.h`.
2. Create `src/PCH.h` (`#include <F4SE/F4SE.h>` + `#include "sup/PCH.h"` with common STL).
3. Create `src/main.cpp` with the CommonLibF4 entry:
   `F4SE_PLUGIN_VERSION` (name "SUP F4SE", version, `UsesAddressLibrary(true)`,
   `CompatibleVersions({ F4SE::RUNTIME_LATEST })`) and `F4SE::Init` + messaging listener.
4. Temporarily stub `F4SEPlugin_Query/Load` out of `SUP_F4SE.cpp` and relocate the init logic
   (INI processing, PEX hash check, `RegisterFuncs`) into the new entry.
5. **Exit criteria:** `xmake build` produces `SUP_F4SE.dll` that loads in-game (logs to
   `My Games/Fallout4/F4SE/SUP_F4SE.log`), console prints version, no crash.

### Phase 1 — Logging swap ✅ done
Implemented as `include/sup/util/Log.h`: a printf-style shim (`sup::log::log(level, loc, fmt, ...)` → `vsnprintf` → spdlog default logger, message passed as data so braces never get fmt-parsed) + `_MESSAGE`/`_DMESSAGE`/`_ERROR` macros. `gLog`/`IDebugLog`/`_DMESSAGESUP`/`_DMESSAGEADD` deleted; `bDebugMode` gating replaced by `sup::log::set_debug_enabled()` (spdlog level filtering). Call sites keep printf formats; per-module conversion to `REX::INFO`/`REX::DEBUG` (std::format) is part of Phase 5 — a mechanical `%d`→`{}` sed would break ~1,900 format strings and can only converge with a compiler (which needs Phase 3 first).

### Phase 2 — Drop Boost ✅ done
`include/sup/util/string.hpp` provides `sup::string::{to_lower, to_lower_copy, iequals, icontains, contains, equals, replace_all, replace_first, replace_last}` (all `std::string_view`-based, per §4.2). All 93 call sites replaced; the boost include was removed from `SUP_F4SE.cpp`; every file that uses `sup::string` includes the header. Boost is fully out of the build. `json.hpp` was untouched (its only `boost::` mention is a comment).

### Phase 3 — Core F4SE API swap
1. `RelocAddr`/`RelocPtr` → `REL::Relocation` (types from `Tomm_FunctionDeclarations.h` first —
   it's the most-used header).
2. `CALL_MEMBER_FN` → member-function `REL::Relocation`s or real RE:: members.
3. `LookupFormByID`/`GetFormByEditorID`/globals (`g_dataHandler`, `g_player`, `g_ui`, …) → RE::
   singleton accessors (this touches nearly every `Tomm_fn_*` file, but is one-to-one).
4. VM plumbing: `VMArray`/`VMValue`/`VMVariable`, `DECLARE_STRUCT`, `PackArray`,
   `CallGlobalFunctionNoWait_Internal` → RE::BSScript equivalents.
5. Entry: delete `F4SEPlugin_Query/Load` bodies, fold into new entry (Phase 0 stub becomes real).
6. **Exit criteria:** DLL loads, all Papyrus functions register (verify with
   `GetSUPF4SEVersion` etc. in-game console), core events fire.

### Phase 4 — Address library
1. Migrate `Tomm_Addresses.h` hard-coded `0x…` offsets to `REL::ID(n)` (look up IDs with
   `tools/find_ids.py` / `parse_ng_db.py` against the 1.11.221 Address Library DB).
2. Re-point `SUP_AddressLib.h`'s `#define fn_* SUP_AddressLib::addr_*` to the new IDs, then
   **delete** `SUP_AddressLib.h/.cpp` (CommonLibF4 already loads the DB).
3. **Exit criteria:** power-grid / workshop functions behave identically on 1.11.221.

### Phase 5 — Module-by-module conversion (the bulk)
For each `Tomm_fn_*` group (order: `general` → `world` → `media` → `online` → `events`):
1. Split header → declarations (`include/sup/papyrus/…/X.h`) + definitions
   (`src/papyrus/…/X.cpp`). Add `#pragma once` + real includes (self-contained).
2. Wrap in `namespace sup::papyrus` (or keep global during transition — namespacing last is
   lower-risk).
3. Convert `cmd_*` signatures to the RE::BSScript API via one registration helper
   (`sup::papyrus::Register(vm, name, fn)`), then delete the old
   `NativeFunctionN` registration lines.
4. Consolidate globals touched by the module into `SUP_Globals` (see §5.7).
5. **Exit criteria per module:** function works in-game (spot-check the module's main entry).

#### Phase 5 as executed (2026-08-14) — every stub resolved to a real implementation

The transition stubs that logged `not implemented` at first use and the `Phase 5 TODO`
markers are all gone. Each now routes through a verified CommonLibF4/Address-Library surface
(all in `include/sup/compat/F4SECompat.h` unless noted):

| Former stub / TODO | Resolution |
|---|---|
| `UIMessageManager::SendUIMessage` | `RE::UIMessageQueue::AddMessage` (REL ID 2284929 == F4SE's SendUIMessage address 0x1A89170) via a pointer-to-member so legacy `CALL_MEMBER_FN(g_uiMessageManager, &UIMessageManager::SendUIMessage)` calls keep working |
| `f_BB_Refresh_MessageMenu` | real `kMessage_Refresh` UI message (now functional via the game queue) + message-ref fullName update (`Tomm_BuildingBridges.h`) |
| `Workshop::hCurrentWorkshop` | Address Library data address (REL ID 4797241, 0x30EC618) — reads/writes the game's own global; `SetCurrentWorkshop` via `BSPointerHandleManagerInterface::GetHandle` (REL ID 2188676) |
| `UIManager::numPauseGame` | reads the game's real `UI::menuMode` counter (now a method — `Tomm_Gameloop.h` updated) |
| `DefaultObjectMap::GetDefaultObject` | editor-ID lookup via `TESDataHandler` (`BGSDefaultObjectManager` is enum-indexed and callers don't carry the enum) |
| `Workshop::ScrapReference` | REL ID 2195125 (0x393C10), disassembly-verified 3-arg free function matching F4SE's typedef (`Tomm_Hooks.h` passes the context + NiPointer it already builds) |
| `SetPlayerMapMarker` / `RemovePlayerMapMarker` | `SetPlayerMapMarker` = REL ID 2238349 (0xEA7470), disassembly-verified; `RemovePlayerMapMarker` clears the player's own map-marker members (no separate game function exists — F4SE shared the address) |
| `TerminalUtils::EstablishTerminalLinks` | REL ID 2195102 (0x38F820) |
| `GetObjectAtConnectPoint` | full reimplementation: `GetSnappedReferenceImpl` (REL ID 2195571) + F4SE's exact `SnappedReferencePointStatus`/`SnapPointParent` layouts with proper refcount Release |
| `SplineUtils::UpdateSpline` | REL ID 2195071 (0x38D830), F4SE typedef signature |
| `GetObjectClassName` RTTI read | real RTTI read via `REL::Module` (compat `GetObjectClassName` is a debug dumper) |
| `GetFormFullName` | `TESFullName::GetFullName` static helper |
| `AttachedScriptsReader` instance check | `IVirtualMachine::GetScriptObjectTypeNoLoad` — the exact "is loaded without loading it" check (F4SE equivalent of the VM registry lookup) |
| `LookupREFRByHandle` (raw u32) | layout-compatible seed of `ObjectRefHandle` + `get()` — the only supported reverse path (no public raw-handle ctor exists; the game serializes handles exactly this way) |
| `CalculateCapacityAndLoad` | reimplemented in C++ over modeled `PowerGrid` fields (NG inlined the original) |
| `IsSwitch` / `GetOpenState` | replaced the two **wrong** fuzzy IDs that crashed on item placement (0x328AB0 / 0x143990 were NOT these functions) with modeled API / PDB-verified REL ID 2192799 |
| `GetObjectAtConnectPoint` hot-path spam | log-once guard |
| `CrosshairChangeHandler` | disabled + logged once — F4SE-internal event source doesn't exist in CommonLibF4 (`Tomm_NativeEvents.h`) |

**Not portable (deliberate):** none remain — the list above covers all 8 logged stubs and all
5 TODO markers.

### Phase 6 — Subsystems
1. **Events/serialization:** `Tomm_Serialization.h` → `F4SE::GetSerializationInterface()`
   callbacks (CommonLibF4's `SKSESerializationInterface` wrapper); `Tomm_Events.h` →
   RE:: event sources; `Tomm_NativeEvents.h` → RE::BSTEventSink or keep as vendored structs
   if CommonLibF4 lacks them.
2. **Hooks:** `Tomm_Hooks.h` from MinHook → CommonLibF4 trampoline hooking; verify each hook
   (`UpdateSeenDataForPlayerLoc`, console/quest/radio hooks in `Tomm_Addresses.h`).
3. **Building Bridges:** keep as-is functionally; convert its ~8.5k lines last (biggest file
   risk); it depends on many `sup::util` helpers that will already be converted.
4. **External:** SUPdater (wininet → `REX::`/WinHTTP or keep wininet), Pastebin reader, Steam
   (keep stub), MP3 (dead code — delete).

### Phase 7 — Cleanup & modern C++
1. Delete duplicates/dead code: 2 of the 3 registration files, `EventCaller.h`,
   `EventScriptCaller.h`, `MP3_Player.h`, `Tomm_fn_MP3.h`, `Tomm_fn_Armor.h`,
   `Tomm_fn_Weapons.h`, `F4SE_Compat.h` stubs, `Span.h` (use `std::span`), xbyak artifacts.
2. ~~Modernize: `std::string_view` params, `std::span`, ranges, structured bindings,
   `enum class`, `constexpr`/`consteval`, `[[nodiscard]]`, `std::expected`, `NULL`→`nullptr`,
   `UInt32`→`std::uint32_t` in *project* code~~ — **dropped as pointless** (cosmetic only,
   no behavioral gain; the plugin already builds as C++23 and the RE:: types are modern).
3. Global state audit: the codebase has dozens of file-scope globals (`g_*`, `s_*`,
   `v*`/`b*`/`i*`); consolidate into `SUP_Globals` (one TU owns them, others use accessors) —
   this is what makes the multi-TU split safe.
4. Add `.clang-format` (CommonLibF4 ships one) and run it.

### Phase 8 — Regression & release
1. Port `exports.def` needs (F4SE uses `Query/Load` exports — CommonLibF4's plugin rule handles
   the resource/def; confirm exported entry matches F4SE expectations). ✅
2. Test matrix: fresh game + existing save, new game, workshop power grid, events, save/load
   (cosave round-trip), each Papyrus function category. ✅ — multiple clean sessions driven by the
   user; wiring/placement/explosions/save-load all green, zero errors, no dumps.
3. Packaging: `xmake package_mod` (custom task in `xmake.lua`) assembles `Package/` (DLL + INI +
   blessed PEX + **PSC source** under `Data/Scripts/Source/User/` + fomod) and deploys to the MO2
   mod folder (`SUP_MODS_PATH`, defaulting to `B:/Modding/MO2/Fallout 4/mods`). `build.bat` now
   chains `xmake build && xmake package_mod`. `package_mod.bat` is deleted (superseded).
   Single command: `xmake package_mod` → build + `Package/` + `SUP_F4SE_v0.7.7.zip` + MO2 deploy.
4. ~~Update `build.bat` → `xmake build`~~ — done: `build.bat` drives `xmake build && xmake package_mod`.
5. Dev test harness removed for release: native self-test battery (`bSelfTest`, `SelfTest.h`,
   64 checks), `SUP_STRUCT_PRE` diagnostic block, `SUPSelfTest.esp` + both test scripts, and
   `tools/gen_holotape_esp.py` — all deleted; `bDebugMode` shipped at `0`.

---

## 6. xmake.lua sketch (Phase 0)

```lua
set_xmakever("3.0.0")
set_project("SUP_F4SE")
set_version("0.7.7")
set_languages("c++23")
set_encodings("utf-8")

add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- point at the vendored copy; alternatively add_requires("commonlibf4")
add_subdirs("Depends/commonlibf4-5ba1928a32c6ccd5690164b79066fc2f5dcb5c65")

target("SUP_F4SE", function()
    set_kind("shared")
    add_rules("commonlibf4.plugin", { name = "SUP F4SE", author = "…", version = "0.7.7" })

    -- static CRT: /MT (release), /MTd (debug)
    if is_mode("release") then set_runtimes("MT") else set_runtimes("MTd") end

    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    set_pcxxheader("src/PCH.h")

    add_defines("SUP_F4SE_EXPORTS")
end)
```

Notes:
- Build: `xmake f -m releasedbg && xmake build` (or `xmake f -m release` for /MT).
- VS solution: `xmake project -k vsxmake` → opens in VS2022 with v143.
- `set_runtimes` must be set **per-config** as shown; CommonLibF4 itself should be built with the
  same runtime to avoid mixed-CRT.

---

## 7. Risks & gotchas

1. **Struct layouts:** `Decoding.h`, `Tomm_fn_MapMarker.h` (`MapMarkerData`), `Tomm_NativeEvents.h`
   hand-roll game structs. CommonLibF4's RE:: types are authoritative for 1.11.x — prefer them;
   keep a vendored struct only if CommonLibF4 lacks the type, and keep the
   `STATIC_ASSERT(offsetof…)` guards as a compile-time canary.
2. **Global state:** ~hundreds of file-scope globals make the multi-TU split the riskiest step.
   Do Phase 7.3 (SUP_Globals) *before* splitting modules, not after.
3. **`xml2json.hpp` is a stub** and `steam_api.h` is a stub header — Steam/XML features are
   currently non-functional in the root tree; don't invest in them before confirming scope with
   the AE folder behavior.
4. **F4SE runtime version:** CommonLibF4 targets 1.11.x (`RUNTIME_LATEST`); the reference AE port
   is 1.11.221. Confirm `Depends/commonlibf4-…` is the 1.11.221-aligned revision before Phase 0.
5. **Big files:** `Tomm_BuildingBridges.h` (4.9k lines) and `Tomm_fn_BuildingBridges.h` (3.6k)
   should be converted last and kept header-only as long as possible.
6. **`codecvt` removal in C++23:** `std::wstring_convert<std::codecvt_utf8_utf16>` in
   `InternalFunctions.h` will not compile under `/std:c++23` with the standard library — replace
   during Phase 2/3.
7. **Commented-out includes / dead code** in the root tree (e.g. `Tomm_fn_Controller.h` not
   included by `SUP_F4SE.cpp`) must not silently become part of the new TU list.
8. **ODR discipline:** current headers define everything (single TU). Once split, every function
   gets exactly one definition in a `.cpp`; use `inline` only for small accessors.

---

## 8. Suggested execution order (short version)

1. Phase 0 (xmake + entry + PCH) — *must come first, unblocks everything.*
2. Phase 1 (logging) + Phase 2 (boost) — mechanical, low risk, do together.
3. Phase 4 (addresses) early — removes the most fragile legacy (hard-coded offsets) before
   touching `Tomm_*` bodies.
4. Phase 3 (core API) — the big sed-able surface.
5. Phase 5 + 7.3 (globals) per module.
6. Phase 6 (subsystems), Phase 7 (cleanup), Phase 8 (regression).

Estimated effort: Phases 0–4 are ~a day of mechanical work; Phase 5 is the long tail
(module-by-module); Phase 6–7 the risk area.
