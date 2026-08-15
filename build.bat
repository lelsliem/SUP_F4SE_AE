@echo off
REM SUP F4SE build script (xmake, see MODERNIZATION_PLAN.md Phase 0)
REM Requires: xmake >= 3.0.0, Visual Studio 2022 Build Tools (v143)

xmake f -p windows -m releasedbg -y
if errorlevel 1 exit /b 1
xmake build
if errorlevel 1 exit /b 1
xmake package_mod
