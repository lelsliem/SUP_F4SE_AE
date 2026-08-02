@echo off
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" "%~dp0SUP_F4SE.vcxproj" /p:Configuration=Release /p:Platform=x64 /t:Build
