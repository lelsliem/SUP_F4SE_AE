@echo off
setlocal
set "VC=C:\Program Files\Microsoft Visual Studio\2022\Community"
set "MSVC=%VC%\VC\Tools\MSVC\14.44.35207"
set "KIT=C:\Program Files (x86)\Windows Kits\10"
set "KVER=10.0.26100.0"
set "PATH=%MSVC%\bin\Hostx64\x64;%VC%\Common7\IDE;%PATH%"
cd /d "%~dp0\.."
cl /nologo /O2 /EHsc ^
  /I"%MSVC%\include" ^
  /I"%KIT%\Include\%KVER%\ucrt" ^
  /I"%KIT%\Include\%KVER%\um" ^
  /I"%KIT%\Include\%KVER%\shared" ^
  /I"%VC%\DIA SDK\include" ^
  tools\symresolve.cpp ^
  /link ^
  /LIBPATH:"%KIT%\Lib\%KVER%\ucrt\x64" ^
  /LIBPATH:"%KIT%\Lib\%KVER%\um\x64" ^
  /LIBPATH:"%VC%\VC\Tools\MSVC\14.16.27023\lib\onecore\x64" ^
  /LIBPATH:"%MSVC%\lib\x64" ^
  msdia140.lib
if exist symresolve.exe (
  move /y symresolve.exe tools\symresolve.exe >nul
  echo BUILT tools\symresolve.exe
) else (
  echo BUILD FAILED
  exit /b 1
)
