# SUP F4SE — Script Utilities Plus

A continuation of **Tomm's SUP F4SE** for Fallout 4, rebuilt for the modern
Next-Gen / AE runtimes — **1.11.137 / 1.11.159 / 1.11.169 / 1.11.191 /
1.11.221 / 1.11.240** — with F4SE **0.7.9** (official for 1.11.240), built
with **CommonLibF4** on MSVC v143 / C++23 via xmake. Addresses resolve through
the Address Library at runtime, so the same DLL works on every AE version as
long as the matching `version-1-11-*.bin` database is installed.

This README is the honest version. It says what works, what had to be removed
and why, who made what, and what the legal situation actually is. The full
technical history is in [MODERNIZATION_PLAN.md](MODERNIZATION_PLAN.md).

## What SUP is

SUP F4SE is a Papyrus function library and settlement toolset — roughly **450
native functions** (451 in the shipped `SUP_F4SE.psc`) that Fallout 4 mods call
for things the game doesn't expose to Papyrus: system traits, file access,
screenshots, media playback helpers, settlement wiring and power-grid queries,
events, map markers, quest hooks, and more. The plugin also installs **18 C++
hooks** that let script mods react to game events (cell changes, radio, quest
completion, console commands, map markers, power-grid connections, knock-downs
and explosions, dismemberment, workshop references).

## Lineage and permission — the honest part

This project exists because the original author is gone, and it is an
**unofficial continuation carried out in good faith**. That needs to be said
plainly:

- **Tomm's original SUP F4SE** (Nexus mod 17295) is the foundation. Tomm is
  unreachable — the author has disappeared and **no permission can be
  obtained** from them for this work.
- **Datanomicron's SUP-F4SE-NG** (github.com/Datanomicron/SUP-F4SE-NG, Dec
  2025) updated the original to the Next-Gen runtime (1.11.191) with Claude AI
  assistance. That repository declares **no license**.
- **lelsliem's SUP_F4SE_AE** (Aug 2026) carried that update to the AE runtimes
  (v0.78 for 1.11.221; this v0.8.0 continues to 1.11.240 and restores the
  wireless power fix).
- **This repository** is a from-the-ground-up rebuild of the modernized source
  on CommonLibF4, performed by a **Codebuff coding agent** at the request of
  the repo owner.

So: there is no explicit upstream permission for any of this chain. Credit is
preserved for everyone involved, and if Tomm or Datanomicron ever objects, this
repository will be removed or reworked on request. Until then, the intent is
that SUP keeps working for the people who depend on it.

## What works now

Verified in repeated in-game test sessions (wiring, power grid, events,
cosave, and third-party settlement mods like NISTRON Smart Home):

- **All 450+ Papyrus functions register and load** — the plugin boots clean,
  zero type-info errors, all 18 structs resolve. The `.psc` source ships in the
  package so other mods can compile against it.
- **C++ hooks** install via the Address Library (REL IDs), logged at startup:
  grid connection add/remove, connect-to-radiator, workshop reference events
  (the wireless power path), set-wants-delete, and apply-dismemberment. Each
  hook's address and function signature were verified against the 1.11.240
  executable before being enabled; the hooks whose upstream signatures no
  longer match the modern game (quest lifecycle, radio, console command,
  script compile, map markers, knock events) are deliberately left disabled
  with the evidence documented in `Tomm_Hooks.h` rather than shipped as latent
  crash bugs.
- **The wireless power fix** (`bWirelessPowerFix`) — restored and working.
  `OnWorkshopHandleReferenceEvent` fires on workshop reference events and runs
  the full `WirelessFixProcessChanges` / `RadiatorFixProcessPowerOnOffEvent`
  chain, so wireless devices (radiators, lights) power on/off correctly.
- **Wiring and power grid** work, including the connect-point snap query path
  that crashed repeatedly during development — it is now wrapped in SEH guards.
- **Events and cosave** work (events persist to the cosave, as the original
  did).
- **Crash handler** is hardened: it writes `SUP_Crash_<pid>.dmp`/`.txt`, skips
  dumps for the known benign vanilla launch crash (a null-read in
  `Fallout4.exe` that also happens with SUP uninstalled), skips dumps for
  faults inside SUP's own SEH-guarded regions, and skips dumps for handled
  access violations inside `ENBHelperF4.dll` (ENB's proxy calls that plugin's
  exports with a mismatched prototype and poisoned output pointers; its own
  SEH guards catch them — cross-module guards our thread-local can't see, so
  the handler matches the fault signature instead). Clean sessions leave zero
  dumps.
- **Previously-broken features on real addresses**: `TimeMultiplier`
  (`RE::BSTimer`), the actor knock-down check (`ActorState::knockState`), and
  cross-cell `MoveRefrToPosition` (`MoveRefToNewSpace`).

## What had to be removed — and why

Not everything survived the move to the modern runtime. This is the honest
list:

- **Legacy hooks whose signatures no longer match the modern game** — the
  quest active/completed/failed, radio add, console command, script compile,
  player map-marker, knock explosion, and get-up-from-knock hooks were
  re-enabled during the 1.11.240 port and each one crashed at runtime with a
  wrong-signature call. They are disabled with the disassembly evidence inline
  in `Tomm_Hooks.h`. Restoring them means re-typing each hook against the real
  function signature at its Address Library ID — tracked as future work.
- **The dev test harness** — the native 64-check self-test battery, the
  `SUP_STRUCT_PRE` startup diagnostic, `SUPSelfTest.esp`, the holotape test
  scripts, and the ESP generator script were all removed for the release build.
- **Legacy modules with no working path on the modern runtime** —
  `Tomm_fn_Armor`, `Tomm_fn_Weapons`, `Tomm_fn_MP3` were deleted rather than
  shipped broken.
- **Disabled stubs with no valid address** — `ItemInterruptsPower` and
  `ItemContainsNode` had no resolvable Address Library ID; they were removed
  instead of left as `REL::ID(0)` no-ops.
- **The old dependency stack** — Boost 1.84, the F4SE source tree, and
  `common` were replaced by CommonLibF4 + the standard library (spdlog for
  logging). `json.hpp` (nlohmann) was kept as-is: header-only, self-contained,
  works fine.
- **Hard-coded offsets** — every `0x…` address was replaced with Address
  Library `REL::ID(n)` so a game update can't silently break call sites.
- **The legacy F4SE API surface** — `RelocAddr`, `CALL_MEMBER_FN`,
  `LookupFormByID`, `g_dataHandler`/`g_player` globals, `VMArray`/`VMValue`,
  hand-rolled `DECLARE_STRUCT` layouts were reimplemented on CommonLibF4's
  `RE::`/`REL::`/`F4SE::` types in `include/sup/compat/F4SECompat.h`.
- **~20 one-off migration tools** and a 63 MB disassembly dump from the
  address-hunting phase were deleted; the tools that are still useful live in
  `tools/`.

## Known limitations and deferred work

- The codebase is a **single translation unit** (~60 headers, all inline). It
  is stable and loads fast; a header/.cpp split was deliberately deferred
  because the single-TU architecture works and the split buys nothing
  functional.
- The `Iter` macro family (740+ uses) is kept as-is — flagged, but stable.
- Some original SUP functions simply have no valid address or modern
  equivalent on the AE runtimes; they were removed rather than shipped broken.
- The console banner prints the legacy `iVersion`-derived string
  (`SUP F4SE V.11.70`) rather than the DLL's own version — cosmetic only; the
  game-formatter float bug that made it print `v0.00` is fixed in the
  `Console_Print` shim.
- Multi-version by design: the plugin declares compatibility with all six AE
  runtimes (1.11.137/159/169/191/221/240) and every address resolves through
  the Address Library, so no per-version build is needed.

## Requirements

- Fallout 4 Next-Gen / AE (runtime **1.11.137 / 1.11.159 / 1.11.169 /
  1.11.191 / 1.11.221 / 1.11.240**)
- F4SE **0.7.9** (official F4SE for 1.11.240)
- **Address Library for F4SE Plugins** (NG version) — the matching
  `version-1-11-*.bin` for your runtime must be present in
  `Data/F4SE/Plugins/`

## Building

MSVC v143 (Visual Studio 2022 Build Tools), xmake 3.0.0+, static CRT (`/MT`).
From Git Bash, pin the platform explicitly or rely on the platform pin in
`xmake.lua`:

```sh
xmake f -m releasedbg -y
xmake build
xmake package_mod        # assembles Package/ + a Vortex-ready zip + deploys to MO2
```

Output: `build/windows/x64/releasedbg/SUP_F4SE.dll`. CommonLibF4 and MinHook
are vendored in `Depends/` (no submodules), so a fresh clone builds out of the
box.

## Installing

The packaged mod (Data/F4SE/Plugins + Data/Scripts + fomod) is in `Package/`,
or use the zip. `SUP_F4SE.ini` controls: `bDebugMode`, `bScrapCrashFix`,
`bTermLinkFix`, `bWirelessPowerFix`, `bDisableAllHooks`.

## Documentation

- [MODERNIZATION_PLAN.md](MODERNIZATION_PLAN.md) — the full technical history:
  every phase, what was changed and why, the crash investigations, and the
  final state of each subsystem.

## Credits

- **Tomm** — the original SUP F4SE. This project only exists because of that
  work, and it is credited and preserved in good faith.
- **Datanomicron** — the Next-Gen update (SUP-F4SE-NG) that first mapped the
  functions to the Address Library.
- **lelsliem** — the AE runtime update (v0.78) this repository continues,
  plus the 1.11.240 port, the restored wireless power fix, and the hook
  signature audit.
- **Codebuff (coding agent)** — performed the CommonLibF4 modernization, the
  crash fixes, and this documentation at the request of the repo owner.

## License

There is no clean license to point at, and this README won't pretend
otherwise: the original author is unreachable and the intermediate update
declares no license. This project is an unofficial continuation with full
credit to all upstream authors. If any upstream author objects, this
repository will be removed or reworked on request.
