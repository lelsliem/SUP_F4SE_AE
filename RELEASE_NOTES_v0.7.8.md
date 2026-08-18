# SUP F4SE v0.7.8

**An unofficial, from-the-ground-up continuation of Tomm's SUP F4SE for the Next-Gen runtime.** Rebuilt on CommonLibF4 for Fallout 4 **1.11.221** / F4SE **0.7.8** (MSVC v143, C++23, `/MT`, xmake). The original author is unreachable and the intermediate NG update declares no license — full honesty about that is in the README. If Tomm or Datanomicron objects, this release will be removed or reworked on request.

## What's in it

- **450+ Papyrus functions** (451 in the shipped `SUP_F4SE.psc`) — the settlement toolset and script utilities: wiring, power-grid queries, events, map markers, quest hooks, file access, and more.
- **18 C++ hooks** installed via the Address Library (no hard-coded offsets — a game update can't silently break them): game loop, cell-seen data, radio, quest lifecycle, console command, script compile, map markers, knock-downs/explosions, dismemberment, grid connections, workshop references.
- **The `.psc` source ships in the package** so other mods can compile against SUP.
- **Hardened crash handler** — writes minidumps on real crashes, and filters the false alarms: the known benign vanilla launch crash (happens without SUP too), SUP's own SEH-guarded regions, and handled faults inside ENBHelperF4.
- **Previously-broken features fixed on real addresses**: `TimeMultiplier` (RE::BSTimer), the actor knock-down check, and cross-cell `MoveRefrToPosition`.
- **The console no longer flashes open on the main menu** (legacy poke removed).

## Changes in this build

- Console open/close poke removed from deferred init (no more console flash on the main menu).
- Crash handler filters ENBHelperF4's handled faults (ENB's proxy calls its exports with mismatched pointers; its own guards catch them — our handler now skips those false-alarm dumps).
- Console banner float bug fixed: prints `SUP F4SE V.11.70` instead of `v0.00`.

## What was removed (and why)

Honest list, all in the README: the wireless power fix (dropped during the crash investigation), the dev test harness, legacy modules with no working path on 1.11.221 (`Tomm_fn_Armor/Weapons/MP3`), disabled stubs with no valid address (`ItemInterruptsPower`, `ItemContainsNode`), Boost + the F4SE source tree (replaced by CommonLibF4 + std), and every hard-coded `0x…` offset.

## Requirements

- Fallout 4 Next-Gen / AE (runtime **1.11.221**)
- F4SE **0.7.8**
- Address Library for F4SE Plugins (NG)

## Install

Install with your mod manager (fomod included) or extract `Data\` into your game folder. `SUP_F4SE.ini` controls `bDebugMode`, `bScrapCrashFix`, `bTermLinkFix`, `bDisableAllHooks`.

## Checksums

- Zip: `444119089a8f216a4f7ad71bda5c555dc9e140a268b374c346eb4eb2cbec0d2e`
- DLL: `bee58b63a524f168d1148c7a5c9e6400971eb4916bdb962712a0bdcf48a40027`
- PEX: `ee2ea112c3662457d34e25a43e74acbbb4babbcb` (blessed, matches the DLL's expected hash)

## Credits

**Tomm** (original SUP F4SE), **Datanomicron** (SUP-F4SE-NG, first Address Library mapping), **lelsliem** (the 1.11.221 update this continues), and **Codebuff** (coding agent that performed the CommonLibF4 modernization, crash fixes, and this documentation). Full lineage and license honesty: see README.
