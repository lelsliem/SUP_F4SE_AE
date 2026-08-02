@echo off
REM SUP F4SE Packaging Script
REM Run this after building Release configuration to create the Vortex-ready package

setlocal

set PROJECT_DIR=%~dp0
set BUILD_DIR=%PROJECT_DIR%x64\Release
set PACKAGE_DIR=%PROJECT_DIR%Package
set OUTPUT_ZIP=%PROJECT_DIR%SUP_F4SE_v0.7.7.zip

echo ========================================
echo SUP F4SE Packaging Script
echo ========================================
echo.

REM Check if the DLL exists
if not exist "%BUILD_DIR%\SUP_F4SE.dll" (
    echo ERROR: SUP_F4SE.dll not found in %BUILD_DIR%
    echo Please build the Release configuration first.
    pause
    exit /b 1
)

echo Copying DLL to package structure...
copy /Y "%BUILD_DIR%\SUP_F4SE.dll" "%PACKAGE_DIR%\Data\F4SE\Plugins\"

REM Copy INI if it exists
if exist "%PROJECT_DIR%SUP_F4SE.ini" (
    echo Copying INI file...
    copy /Y "%PROJECT_DIR%SUP_F4SE.ini" "%PACKAGE_DIR%\Data\F4SE\Plugins\"
)

REM Copy PEX files if they exist
if exist "%PROJECT_DIR%Scripts\*.pex" (
    echo Copying Papyrus scripts...
    if not exist "%PACKAGE_DIR%\Data\Scripts" mkdir "%PACKAGE_DIR%\Data\Scripts"
    copy /Y "%PROJECT_DIR%Scripts\*.pex" "%PACKAGE_DIR%\Data\Scripts\"
)

echo.
echo Package structure created in: %PACKAGE_DIR%
echo.
echo To create the final ZIP file, use 7-Zip or similar to compress
echo the contents of the Package folder (Data and fomod folders).
echo.
echo The ZIP should contain:
echo   Data\F4SE\Plugins\SUP_F4SE.dll
echo   fomod\info.xml
echo   fomod\ModuleConfig.xml
echo.

REM Try to create ZIP with PowerShell if available
echo Attempting to create ZIP with PowerShell...
powershell -Command "Compress-Archive -Path '%PACKAGE_DIR%\*' -DestinationPath '%OUTPUT_ZIP%' -Force"

if exist "%OUTPUT_ZIP%" (
    echo.
    echo SUCCESS: Created %OUTPUT_ZIP%
    echo This file can be installed directly with Vortex.
) else (
    echo.
    echo Could not create ZIP automatically.
    echo Please manually compress the Package folder contents.
)

echo.
pause
