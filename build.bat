@echo off
setlocal enabledelayedexpansion
cd /D "%~dp0"

:: --- Parse Arguments ---
set MODE=%~1
set APP=%~2

:: Default to debug if no arguments are provided
if "!MODE!"=="" (
    set MODE=debug
) else (
    :: If the first argument isn't debug or release, assume it's just the app name
    if /I "!MODE!" NEQ "debug" (
        if /I "!MODE!" NEQ "release" (
            set APP=!MODE!
            set MODE=debug
        )
    )
)



:: --- Compiler Settings ---
set CFLAGS= -std=c99 -Ipgl -Ithird_party -fdiagnostics-absolute-paths -Wall -Wno-unknown-warning-option -Wno-missing-braces -Wno-unused-function -Wno-unused-parameter -Wno-writable-strings -Wno-missing-field-initializers -Wno-unused-value -Wno-unused-variable -Wno-unused-local-typedef -Wno-deprecated-register -Wno-deprecated-declarations -Wno-unused-but-set-variable -Wno-single-bit-bitfield-constant-conversion -Wno-compare-distinct-pointer-types -Wno-initializer-overrides -Wno-incompatible-pointer-types-discards-qualifiers -ferror-limit=10000

:: --- Linker Settings ---
set LDFLAGS=-Xlinker /subsystem:windows -luser32 -fuse-ld=lld

:: --- Apply Config Flags ---
if /I "!MODE!"=="release" (
    echo [Config] Mode: RELEASE
    :: -O3 for max optimization, -DNDEBUG to disable asserts
    set CFLAGS=!CFLAGS! -O3 -DNDEBUG
    set OUT_DIR=bin_release
) else (
    echo [Config] Mode: DEBUG
    :: -O0 for no optimization, -g for debug symbols, -D_DEBUG for debug macros
    set CFLAGS=!CFLAGS! -O0 -g -D_DEBUG
    set OUT_DIR=bin_debug
)

:: Create an output directory for the executables
if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"

:: --- Command: Run specific app ---
if "!APP!" NEQ "" (
    echo [Building] !APP!...
    
    :: Note: Using !CFLAGS! and !LDFLAGS! because they were modified at runtime
    clang !CFLAGS! "apps\!APP!\!APP!_main.c" -o "%OUT_DIR%\!APP!.exe" !LDFLAGS!
    
    :: If compilation succeeds, run it
    if !ERRORLEVEL! EQU 0 (
        echo [Running] !APP! in %OUT_DIR%
        echo ========================================
        "%OUT_DIR%\!APP!.exe"
    )
    exit /b !ERRORLEVEL!
)

:: --- Command: Build All (Default) ---
echo Building all apps...
for /d %%D in (apps\*) do (
    set APP_DIR=%%~nxD
    echo [Building] !APP_DIR!...
    clang !CFLAGS! "%%D\!APP_DIR!_main.c" -o "%OUT_DIR%\!APP_DIR!.exe" !LDFLAGS!
)

echo Done.