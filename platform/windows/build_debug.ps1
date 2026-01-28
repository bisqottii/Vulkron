Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# defaults to all if no argument is provided
$Target = if ($args.Count -ge 1) { $args[0] } else { "all" }

# --- Find Conda ---
$condaExe = "$env:USERPROFILE\Miniconda3\Scripts\conda.exe"
if (-not (Test-Path $condaExe)) {
    Write-Host "Miniconda not found at $env:USERPROFILE\Miniconda3" -ForegroundColor Red
    exit 1
}

# --- Get environment prefix ---
$condaPrefix = & $condaExe info --base
$envPrefix = Join-Path $condaPrefix "envs\vk-env"
if (-not (Test-Path $envPrefix)) {
    Write-Host "Error: vk-env environment not found at $envPrefix" -ForegroundColor Red
    exit 1
}

# --- Out-of-source build directory ---
$buildDir = "..\build\debug"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Force -Path $buildDir }

# --- Find clang executables dynamically ---
# This is because on windows clang++ comes with versioning appended
$clang = Get-ChildItem "$envPrefix\Library\bin" -Filter "clang.exe" | Select-Object -First 1
$clangpp = Get-ChildItem "$envPrefix\Library\bin" -Filter "clang++*.exe" | Select-Object -First 1

# --- Configure CMake ---
cmake -G Ninja `
      -B "$buildDir" `
      -S . `
      -DCMAKE_BUILD_TYPE=Debug `
      -DCMAKE_C_COMPILER="$($clang.FullName)" `
      -DCMAKE_CXX_COMPILER="$($clangpp.FullName)"

# --- Build with Ninja explicitly ---
ninja -C "$buildDir" $Target