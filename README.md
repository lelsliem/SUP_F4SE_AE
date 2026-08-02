SUP F4SE - Script Utilities Plus (Next Generation Update)

Version 0.78 — Compatible with F4SE 0.7.8 and Fallout 4 v1.11.221



Overview

This repository is an unofficial Next Generation compatibility update for Tomm's

SUP F4SE mod. It updates address handling and runtime compatibility so the

plugin works with Fallout 4 NG/AE (runtime 1.11.221) and F4SE 0.7.8.



This fork was produced by a human maintainer with assistance from an AI tool.

It maps game addresses to the Address Library and adds runtime resolution so

the plugin does not rely on hardcoded offsets.



Quick status



Plugin version: 0.78 (F4SE 0.7.8)



Target runtime: Fallout 4 v1.11.221 (NG/AE)



Address Library: NG format support



Hooks enabled: 17 (varies by runtime and available addresses)



Requirements



Fallout 4 Next Generation or Anniversary Edition (v1.10.980+; tested on 1.11.221)



F4SE 0.7.8



Address Library for F4SE Plugins (NG version)



Visual Studio 2022 (for building from source)



CMake (for building f4se subproject)



Windows SDK



Features



500+ Papyrus functions (file I/O, JSON, XML, INI, math, strings, etc.)



Workshop and power-grid utilities



Map marker and quest event hooks



Screenshot capture and GDI+ support



Steam API integration



Address Library integration for runtime address resolution



What changed in this NG update



Replaced hardcoded addresses with Address Library IDs



Added SUP\_AddressLib (loader and resolver)



Mapped 35+ game functions to Address Library IDs



Added graceful fallbacks for unresolved addresses



Rebuilt f4se as static library option to match CRT settings



Installation (end user)



Using a mod manager (Vortex, MO2)



Install F4SE 0.7.8 manually.



Install Address Library (NG version).



Install this mod.



Launch Fallout 4 with f4se\_loader.exe.



Manual install



Copy SUP\_F4SE.dll to:

<Fallout 4 folder>\\Data\\F4SE\\Plugins\\



Ensure Address Library database file for your runtime is present:

version-1-11-221-0.bin (or the matching NG file)



Building from source (developer)

Prerequisites:



Visual Studio 2022



CMake



F4SE source tree and common build artifacts



Example build steps (Windows, PowerShell or CMD):



Prepare f4se build (example used in this project):

cd C:\\SUP\_F4SE

rmdir /s /q f4se\\build

rmdir /s /q f4se\\install



cmake -B f4se/build -S f4se ^

\-DCMAKE\_INSTALL\_PREFIX=f4se/install ^

\-DCMAKE\_PREFIX\_PATH=C:/SUP\_F4SE/common/install ^

\-DCMAKE\_MSVC\_RUNTIME\_LIBRARY=MultiThreaded ^

\-DF4SE\_STATIC\_LIB=ON



cmake --build f4se/build --config Release



Build the plugin:

Open SUP\_F4SE.sln in Visual Studio or use msbuild:

msbuild SUP\_F4SE.sln /t:Rebuild /p:Configuration=Release;Platform=x64



Notes:



Ensure CRT/runtime settings match across projects (MultiThreaded vs MultiThreadedDLL).



If you change F4SE build options, rebuild dependent libs (common, f4se\_common, f4se).



The project uses absolute library paths by default; you can change these to

relative paths and set AdditionalLibraryDirectories in the vcxproj.



Key source files



SUP\_AddressLib.h / SUP\_AddressLib.cpp   (Address Library loader and resolver)



SUP\_F4SE.cpp                            (main plugin code, initialization)



Tomm\_.h / Tomm\_.cpp                   (original function implementations)



exports.def                              (exported symbols)



Address Library mappings (sample)

The project maps many game functions to Address Library IDs. A few examples:



REFR\_HasKeywordHelper        -> 2196779



WorkShopExtra\_RemoveItem     -> 4472849



WorkShopExtra\_AddGridConnection -> 4472860



Main\_OnQuestComplete         -> 2204940



Main\_OnRadioAddStation       -> 2229305



Actor\_Jump                   -> 2233155

(See SUP\_AddressLib.h for the full list of mapped IDs.)



Known limitations



Some hooks remain disabled when the Address Library does not provide an ID:



Gameloop hook (address not found)



OnRemovePlayerMapMarker (address not found)



Crosshair handler (NG compatibility)



Time multiplier access (address not found)



Behavior may vary between NG and non-NG game builds. Test in your target runtime.



Credits and attribution

Original mod and code:



Tomm (TommInfinite) — original SUP F4SE author



Contributors to this NG update:



Datanomicron — original contributor referenced



jim\_t — human collaborator and maintainer of this fork



Claude (Anthropic AI) — assisted with code and mapping (tool-assisted work)



Dependencies and libraries



F4SE (F4SE team)



Address Library (meh321)



nlohmann/json (JSON for Modern C++)



MinHook (Tsuda Kageyu) or equivalent hooking library



OpenXLSX or other third-party libs used by original project



License and redistribution

This repository is an unofficial update. The original SUP F4SE is created by

Tomm. This fork is provided for compatibility and development purposes.



Before publishing or redistributing:



Verify the original project's license and comply with its terms.



If the original project has no license, obtain permission from the original

author before publishing publicly.



Include attribution to the original author(s) in README and CONTRIBUTORS.



Suggested repository files to include



README.txt (this file)



BUILD.md (exact build steps and CMake flags)



CONTRIBUTORS (list of authors and maintainers)



LICENSE (choose a license compatible with original work)



.gitignore (exclude build artifacts: \*.obj, \*.lib, \*.dll, x64/Release, f4se/build, f4se/install)



Changelog (high level)

v0.78



Updated plugin to target F4SE 0.7.8 and Fallout 4 runtime 1.11.221



Rebuilt f4se with static lib option and matching CRT



Address Library NG format support and 35+ mapped IDs



Resolved many runtime linker issues and GDI+ dependencies



Enabled 17 runtime hooks where addresses are available



v0.77 (previous)



Initial NG compatibility work for F4SE 0.7.7 and runtime 1.11.191



Address Library integration and initial mappings



Contact and support



Open an issue on the GitHub repository with:



Fallout 4 runtime version



F4SE version



SUP\_F4SE.log and f4se.log



Steps to reproduce the problem



Final notes



Keep your Address Library database file for the exact runtime you use.



If you change build settings (CRT, static vs dynamic), rebuild all dependent

libraries to avoid unresolved externals.



If you want, I can generate a short BUILD.md, a CONTRIBUTING template, and a

clean LICENSE suggestion for the repo.

