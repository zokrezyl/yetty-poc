@echo off
REM Yetty Windows Build Script
REM Usage: build.bat [debug|release] [clean]

setlocal enabledelayedexpansion

set CONFIG=Release
set CLEAN=0

:parse_args
if "%1"=="" goto :start
if /i "%1"=="debug" (
    set CONFIG=Debug
    shift
    goto :parse_args
)
if /i "%1"=="release" (
    set CONFIG=Release
    shift
    goto :parse_args
)
if /i "%1"=="clean" (
    set CLEAN=1
    shift
    goto :parse_args
)
shift
goto :parse_args

:start
set SCRIPT_DIR=%~dp0
for %%i in ("%SCRIPT_DIR%..\..\") do set PROJECT_ROOT=%%~fi

REM Convert to lowercase for build dir
set CONFIG_LOWER=%CONFIG%
if /i "%CONFIG%"=="Debug" set CONFIG_LOWER=debug
if /i "%CONFIG%"=="Release" set CONFIG_LOWER=release

set BUILD_DIR=%PROJECT_ROOT%build-desktop-%CONFIG_LOWER%

echo Yetty Windows Build
echo   Project Root: %PROJECT_ROOT%
echo   Build Dir: %BUILD_DIR%
echo   Config: %CONFIG%
echo.

REM Clean if requested
if %CLEAN%==1 (
    if exist "%BUILD_DIR%" (
        echo Cleaning build directory...
        rmdir /s /q "%BUILD_DIR%"
    )
)

REM Configure if needed
if not exist "%BUILD_DIR%\build.ninja" (
    if not exist "%BUILD_DIR%\*.sln" (
        echo Configuring CMake...

        REM Check for Ninja
        where ninja >nul 2>&1
        if %errorlevel%==0 (
            cmake -B "%BUILD_DIR%" -G "Ninja" -DCMAKE_BUILD_TYPE=%CONFIG% "%PROJECT_ROOT%"
        ) else (
            cmake -B "%BUILD_DIR%" -G "Visual Studio 17 2022" -A x64 "%PROJECT_ROOT%"
        )

        if %errorlevel% neq 0 (
            echo CMake configuration failed
            exit /b 1
        )
    )
)

REM Build
echo Building...
cmake --build "%BUILD_DIR%" --config %CONFIG% --parallel
if %errorlevel% neq 0 (
    echo Build failed
    exit /b 1
)

REM Copy wgpu DLL
set WGPU_DLL=%BUILD_DIR%\_deps\wgpu-native\lib\wgpu_native.dll
if exist "%WGPU_DLL%" (
    if exist "%BUILD_DIR%\yetty.exe" (
        copy /y "%WGPU_DLL%" "%BUILD_DIR%\" >nul
    ) else if exist "%BUILD_DIR%\%CONFIG%\yetty.exe" (
        copy /y "%WGPU_DLL%" "%BUILD_DIR%\%CONFIG%\" >nul
    )
    echo Copied wgpu_native.dll to output directory
)

echo.
echo Build complete!

if exist "%BUILD_DIR%\yetty.exe" (
    echo Executable: %BUILD_DIR%\yetty.exe
) else (
    echo Executable: %BUILD_DIR%\%CONFIG%\yetty.exe
)

endlocal
