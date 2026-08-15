# Papyrus compilation (SUP_F4SE.psc -> SUP_F4SE.pex)

SUP_F4SE.psc is a `native` script: it only declares the Papyrus-facing function
signatures implemented in the plugin DLL. The compiled `.pex` must match what the
plugin expects, otherwise the plugin shows a "PEX VERSION WRONG" error (it SHA1-checks
`Data\Scripts\SUP_F4SE.pex` against `s_CurrentPEXHash` in
`include/sup/core/Tomm_PexFileSHA1.h`).

## Tools

The Papyrus compiler ships with Fallout 4:

    B:\SteamLibrary\steamapps\common\Fallout 4\Papyrus Compiler\PapyrusCompiler.exe

(ToolsShortcuts\PapyrusCompiler.exe - Shortcut.lnk points at it.)

The compiler needs:

- The import path (`-i`): where to find vanilla/other scripts the source imports.
  Use the game's `Data\Scripts\Source\Base` (and DLC folders), separated by `;`.
- The flags file (`-f`): `Data\Scripts\Source\Base\Institute_Papyrus_Flags.flg`.
- The output dir (`-o`): where the `.pex` is written (typically `Data\Scripts`).

## Compiling

Run from this directory (update the game root if it changed):

    "B:\SteamLibrary\steamapps\common\Fallout 4\Papyrus Compiler\PapyrusCompiler.exe" \
        SUP_F4SE.psc \
        -f="B:\SteamLibrary\steamapps\common\Fallout 4\Data\Scripts\Source\Base\Institute_Papyrus_Flags.flg" \
        -i="B:\SteamLibrary\steamapps\common\Fallout 4\Data\Scripts\Source\Base;B:\SteamLibrary\steamapps\common\Fallout 4\Data\Scripts\Source\User" \
        -o="out" \
        -op -r

`-op` = optimize, `-r` = release, `-final` = final release (strips debug info).

## Hash check

The plugin compares SHA1(Data\Scripts\SUP_F4SE.pex) against `s_CurrentPEXHash`
(include/sup/core/Tomm_PexFileSHA1.h) and refuses to run otherwise. The blessed
`Scripts/SUP_F4SE.pex` (hash `ee2ea112c3662457d34e25a43e74acbbb4babbcb`) is the one
that matches and must be shipped unchanged.

**Important: the Papyrus compiler output is non-deterministic.** Three identical
compiles of the same source produce three different .pex hashes (verified with
PapyrusCompiler 2.8.0.4). So a fresh compile can never reproduce the blessed hash,
and updating `s_CurrentPEXHash` to a fresh compile is futile (each rebuild changes it).

Do NOT ship a freshly compiled .pex unless you also intend to drop the SHA1 check.
The compile pipeline above exists to validate the source compiles cleanly, not to
regenerate the shipped artifact.
