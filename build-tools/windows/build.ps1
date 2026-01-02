# Yetty Windows Build Script
# Usage: .\build.ps1 [-Config Debug|Release] [-Clean]

param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release",
    [switch]$Clean
)

$ErrorActionPreference = "Stop"
$ProjectRoot = (Get-Item $PSScriptRoot).Parent.Parent.FullName
$BuildDir = Join-Path $ProjectRoot "build-desktop-$($Config.ToLower())"

Write-Host "Yetty Windows Build" -ForegroundColor Cyan
Write-Host "  Project Root: $ProjectRoot"
Write-Host "  Build Dir: $BuildDir"
Write-Host "  Config: $Config"
Write-Host ""

# Clean if requested
if ($Clean -and (Test-Path $BuildDir)) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BuildDir
}

# Configure if needed
if (-not (Test-Path (Join-Path $BuildDir "build.ninja"))) {
    Write-Host "Configuring CMake..." -ForegroundColor Green

    # Use Ninja if available, otherwise use Visual Studio
    $Generator = if (Get-Command ninja -ErrorAction SilentlyContinue) {
        @("-G", "Ninja")
    } else {
        @("-G", "Visual Studio 17 2022", "-A", "x64")
    }

    cmake -B $BuildDir $Generator -DCMAKE_BUILD_TYPE=$Config $ProjectRoot
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed"
        exit 1
    }
}

# Build
Write-Host "Building..." -ForegroundColor Green
cmake --build $BuildDir --config $Config --parallel
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit 1
}

# Copy DLLs to output directory
$ExeDir = if (Test-Path (Join-Path $BuildDir "yetty.exe")) {
    $BuildDir
} else {
    Join-Path $BuildDir $Config
}

$WgpuDll = Join-Path $BuildDir "_deps\wgpu-native\lib\wgpu_native.dll"
if (Test-Path $WgpuDll) {
    Copy-Item $WgpuDll $ExeDir -Force
    Write-Host "Copied wgpu_native.dll to output directory" -ForegroundColor Gray
}

Write-Host ""
Write-Host "Build complete!" -ForegroundColor Green
Write-Host "Executable: $(Join-Path $ExeDir 'yetty.exe')"
