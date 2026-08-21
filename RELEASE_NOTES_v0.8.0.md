# SUP F4SE v0.8.0

**An unofficial, from-the-ground-up continuation of Tomm's SUP F4SE for the Next-Gen / AE runtimes.** Built on CommonLibF4 for Fallout 4 **1.11.137 / 1.11.159 / 1.11.169 / 1.11.191 / 1.11.221 / 1.11.240** with F4SE **0.7.9** (MSVC v143, C++23, `/MT`, xmake). The original author is unreachable and the intermediate NG update declares no license — full honesty about that is in the README. If Tomm or Datanomicron objects, this release will be removed or reworked on request.

## What's in it

- **450+ Papyrus functions** (451 in the shipped `SUP_F4SE.psc`) — the settlement toolset and script utilities: wiring, power-grid queries, events, map markers, quest hooks, file access, and more.
- **Multi-version**: the plugin declares compatibility with all six AE runtimes and resolves every address through the Address Library at runtime — one DLL works on 1.11.137/159/169/191/221/240 as long as the matching `version-1-11-*.bin` database is installed.
- **The wireless power fix — restored.** `bWirelessPowerFix = 1` in `SUP_F4SE.ini`. `OnWorkshopHandleReferenceEvent` fires on workshop reference events and drives the full `WirelessFixProcessChanges` / `RadiatorFixProcessPowerOnOffEvent` chain, so wireless devices power on/off correctly (verified with NISTRON Smart Home: device reset, scrapping, and light toggling are CTD-free).
- **Hook signature audit.** Every previously-enabled hook was disassembled against the 1.11.240 executable. The hooks whose signatures verified (grid connections, connect-to-radiator, workshop reference events, set-wants-delete, apply-dismemberment) are enabled; the ones whose upstream signatures no longer match the modern game (quest lifecycle, radio, console command, script compile, map markers, knock events) are disabled with the evidence inline in `Tomm_Hooks.h` — no more wrong-signature runtime CTDs.
- **The `.psc` source ships in the package** so other mods can compile against SUP.
- **Hardened crash handler** — writes minidumps on real crashes, and filters the false alarms: the known benign vanilla launch crash (happens without SUP too), SUP's own SEH-guarded regions, and handled faults inside ENBHelperF4.
- **Previously-broken features fixed on real addresses**: `TimeMultiplier` (RE::BSTimer), the actor knock-down check, and cross-cell `MoveRefrToPosition`.

## Changes since v0.7.8

- Ported to runtime **1.11.240** and F4SE **0.7.9**; plugin now declares compatibility with **all six** AE runtimes (137/159/169/191/221/240).
- **Wireless power fix restored** (was dropped in the 0.7.8 crash investigation) — workshop reference events now drive the full wireless power chain.
- **Hook signature audit** — wrong-signature hooks disabled with evidence; only verified hooks are active (see README).
- Version bumped to **0.8.0**; `bDebugMode` ships `0` in the release INI.

## What was removed (and why)

See the README's "What had to be removed" section — the honest list. Highlights: the dev test harness, legacy modules with no working modern path (`Tomm_fn_Armor/Weapons/MP3`), disabled stubs with no valid address (`ItemInterruptsPower`, `ItemContainsNode`), Boost + the F4SE source tree (replaced by CommonLibF4 + std), and every hard-coded `0x…` offset. The legacy-hook signature work is documented in `Tomm_Hooks.h` as future work.

## Requirements

- Fallout 4 Next-Gen / AE — runtime **1.11.137 / 1.11.159 / 1.11.169 / 1.11.191 / 1.11.221 / 1.11.240**
- F4SE **0.7.9**
- **Address Library for F4SE Plugins** (NG version) with the matching `version-1-11-*.bin` for your runtime

## Credits

**Tomm** (original SUP F4SE), **Datanomicron** (SUP-F4SE-NG, first Address Library mapping), **lelsliem** (AE runtime update, 1.11.240 port, wireless fix restoration, hook audit), and **Codebuff** (coding agent that performed the CommonLibF4 modernization, crash fixes, and this documentation). Full lineage and license honesty: see README.
