@echo off
:: ==============================
:: Run as Administrator
:: ==============================
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo This script is not running with Administrator privileges.
    echo Restarting with elevation...
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)

echo Getting Minecraft UWP installation directory...

:: ==============================
:: Get the installation path
:: ==============================
for /f "delims=" %%I in ('powershell -NoProfile -Command "(Get-AppxPackage -Name 'Microsoft.MinecraftUWP').InstallLocation"') do (
    set "InstallPath=%%I"
)

if "%InstallPath%"=="" (
    echo Microsoft.MinecraftUWP was not found. Please make sure it is installed.
    pause
    exit /b 1
)

echo Minecraft installation path: %InstallPath%

:: ==============================
:: Set the DLL download URL and target
:: ==============================
set "DllUrl=https://github.com/QYCottage/ModLoader/releases/download/v1.1.0/WINHTTP.dll"
set "TargetFile=%InstallPath%\WINHTTP.dll"

echo Preparing to download DLL to: %TargetFile%

:: ==============================
:: Download the file
:: ==============================
powershell -NoProfile -Command "Invoke-WebRequest -Uri '%DllUrl%' -OutFile '%TargetFile%' -UseBasicParsing"

if %ERRORLEVEL% NEQ 0 (
    echo Download failed!
    pause
    exit /b 1
)

echo Download completed successfully.

pause