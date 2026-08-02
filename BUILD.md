# Build Instructions for SUP_F4SE-AE

This document describes how to build the project and its f4se dependency on Windows (MSVC / Visual Studio 2022).
Follow the steps exactly to reproduce the build used for release v0.78.

## Prerequisites
- Windows 10/11 with Visual Studio 2022 (Desktop development with C++)
- CMake (>= 3.20)
- Git
- Windows SDK (installed with Visual Studio)
- F4SE source tree and the `common` build artifacts placed under the repo root as described below

## Repository layout (expected)
Place the repository at:
`C:\SUP_F4SE\SUP-F4SE-AE`

Inside that folder you should have:
- `SUP_F4SE.sln` and project source (`SUP_F4SE.cpp`, headers, `Tomm_*.h`, etc.)
- `f4se` (F4SE source tree used to build the f4se static lib)
- `common` (common library source used by f4se)

## Recommended build configuration
- **Configuration**: Release
- **Platform**: x64
- **CRT**: MultiThreaded (static) — ensure all projects use the same runtime to avoid unresolved externals

---

## Build steps (copy/paste into PowerShell or CMD)

1. Open a Developer Command Prompt for VS 2022 (x64 Native Tools) or a PowerShell session with the VS environment loaded.

2. Prepare `common` and `f4se` build directories and clone sources if you don't already have them.
   Replace the example clone URLs below with the correct upstream repositories you use.

```powershell
cd C:\SUP_F4SE\SUP-F4SE-AE

# Remove previous build/install directories if present
rmdir /s /q common\build
rmdir /s /q common\install
rmdir /s /q f4se\build
rmdir /s /q f4se\install

# Clone sources (replace these URLs with the official sources you use)
git clone https://github.com/ianpatt/common.git common
git clone https://github.com/ianpatt/f4se.git f4se
git clone https://github.com/lelsliem/SUP_F4SE_AE

use SUP_F4SE.sln right click solutions explorer>properties check paths in CC++ and linker under general are correct 


