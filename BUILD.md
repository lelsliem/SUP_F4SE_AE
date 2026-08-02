\# Build Instructions for SUP\_F4SE-AE



This document describes how to build the project and its f4se dependency on Windows (MSVC / Visual Studio 2022).

Follow the steps exactly to reproduce the build used for release v0.78.



\## Prerequisites

\- \*\*Windows 10/11\*\* with Visual Studio 2022 (Desktop development with C++)

\- \*\*CMake\*\* (>= 3.20)

\- \*\*Git\*\*

\- Windows SDK (installed with Visual Studio)

\- F4SE source tree and the `common` build artifacts placed under the repo root as described below



\## Repository layout (expected)

Place the repository at:

`C:\\\\SUP\\\_F4SE\\\\SUP-F4SE-AE`



Inside that folder you should have:

\- `SUP\\\_F4SE.sln` and project source (`SUP\\\_F4SE.cpp`, headers, Tomm\_\*.h, etc.)

\- `f4se` (F4SE source tree used to build f4se static lib)

\- `common` (common library build/install prefix used by f4se)



\## Recommended build configuration

\- \*\*Configuration\*\*: Release

\- \*\*Platform\*\*: x64

\- \*\*CRT\*\*: MultiThreaded (static) — ensure all projects use the same runtime to avoid unresolved externals



\---



\## Build steps (copy/paste into PowerShell or CMD)



1\. Open an elevated Developer Command Prompt for VS 2022 or use a VS x64 Native Tools prompt.



2\. Prepare f4se build directories and configure CMake:



```powershell

cd C:\\\\SUP\\\_F4SE 

rmdir /s /q common\\\\build

rmdir /s /q common\\\\install

rmdir /s /q f4se\\\\build

rmdir /s /q f4se\\\\install

git clone https://github.com/ianpatt/common

git clone https://github.com/ianpatt/f4se

cmake -B common/build -S common -DCMAKE\\\_INSTALL\\\_PREFIX=common/install -DCMAKE\\\_MSVC\\\_RUNTIME\\\_LIBRARY=MultiThreaded

cmake --build common/build --config Release --target install



\\# configure f4se build

cmake -B f4se/build -S f4se `

\&#x20; -DCMAKE\\\_INSTALL\\\_PREFIX=f4se/install `

\&#x20; -DCMAKE\\\_PREFIX\\\_PATH=C:/SUP\\\_F4SE/common/install `

\&#x20; -DCMAKE\\\_MSVC\\\_RUNTIME\\\_LIBRARY=MultiThreaded `

\&#x20; -DF4SE\\\_STATIC\\\_LIB=ON

git clone http://github.com/thisSUPF4SE](https://github.com/lelsliem/SUP_F4SE_AE

use SUP\\\_F4SE.sln right click solutions explorer>properties check paths in CC++ and linker under general are correct 


