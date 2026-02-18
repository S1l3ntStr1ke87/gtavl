@echo off
setlocal enabledelayedexpansion

REM Build script for gtavl project
echo Building gtavl project...

REM -----------------------------------------------------------------------
REM Locate Visual Studio 2022 vcvarsall.bat to set up the MSVC environment
REM -----------------------------------------------------------------------
set "VCVARS="
for %%E in (Enterprise Professional Community BuildTools) do (
    if exist "C:\Program Files\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvarsall.bat" (
        set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvarsall.bat"
        goto :found_vcvars
    )
)
echo ERROR: Could not find Visual Studio 2022 vcvarsall.bat
echo Make sure Visual Studio 2022 (any edition) is installed.
exit /b 1
:found_vcvars
echo Found MSVC environment: %VCVARS%
call "%VCVARS%" x64
if errorlevel 1 (
    echo ERROR: Failed to initialize MSVC x64 environment.
    exit /b 1
)

REM -----------------------------------------------------------------------
REM Locate Ninja (bundled with VS or on PATH)
REM -----------------------------------------------------------------------
set "NINJA_EXE="
for %%E in (Enterprise Professional Community BuildTools) do (
    if exist "C:\Program Files\Microsoft Visual Studio\2022\%%E\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" (
        set "NINJA_EXE=C:\Program Files\Microsoft Visual Studio\2022\%%E\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"
        goto :found_ninja
    )
)
REM Fall back to ninja on PATH
where ninja >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%P in ('where ninja') do set "NINJA_EXE=%%P"
    goto :found_ninja
)
echo ERROR: Could not find ninja.exe. Install Ninja or use Visual Studio 2022.
exit /b 1
:found_ninja
echo Found Ninja: %NINJA_EXE%

REM -----------------------------------------------------------------------
REM Determine Windows SDK include path for the RC compiler
REM vcvarsall sets WindowsSDKVersion; strip trailing backslash
REM -----------------------------------------------------------------------
set "SDK_VER=%WindowsSDKVersion%"
if "%SDK_VER:~-1%"=="\" set "SDK_VER=%SDK_VER:~0,-1%"
set "SDK_INCLUDE=C:\Program Files (x86)\Windows Kits\10\Include\%SDK_VER%"
echo Windows SDK version: %SDK_VER%
echo RC include path: %SDK_INCLUDE%

REM -----------------------------------------------------------------------
REM Set up build directory (clean previous configure cache if present)
REM -----------------------------------------------------------------------
set "BUILD_DIR=out\build\x64-Release"
if exist "%BUILD_DIR%\CMakeCache.txt" (
    echo Cleaning previous CMake cache...
    del /f /q "%BUILD_DIR%\CMakeCache.txt"
)
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM -----------------------------------------------------------------------
REM Run CMake configuration with Ninja generator
REM Note: -DCMAKE_MAKE_PROGRAM must NOT have extra quotes around the path
REM       or CMake will treat it as a directory and append /ninja.exe again.
REM -----------------------------------------------------------------------
echo.
echo Configuring with CMake (Ninja, Release)...
cmake -S . -B "%BUILD_DIR%" ^
    -G Ninja ^
    -DCMAKE_BUILD_TYPE=Release ^
    "-DCMAKE_MAKE_PROGRAM=%NINJA_EXE%" ^
    "-DCMAKE_RC_FLAGS=/I \"%SDK_INCLUDE%\um\" /I \"%SDK_INCLUDE%\shared\" /I \"%CD%\""
if errorlevel 1 (
    echo ERROR: CMake configuration failed.
    exit /b 1
)

REM -----------------------------------------------------------------------
REM Build the project
REM -----------------------------------------------------------------------
echo.
echo Building...
cmake --build "%BUILD_DIR%" --config Release
if errorlevel 1 (
    echo ERROR: Build failed.
    exit /b 1
)

REM -----------------------------------------------------------------------
REM Report output location
REM -----------------------------------------------------------------------
echo.
echo Build completed successfully!
echo Output binary: %BUILD_DIR%\bin\GTA5_launcher.exe

endlocal
