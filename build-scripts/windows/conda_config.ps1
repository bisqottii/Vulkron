Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# --- Check if Miniconda exists ---
$condaSh = "$env:USERPROFILE\Miniconda3\shell\condabin\conda-hook.ps1"
if (-not (Test-Path $condaSh)) {
    Write-Host "Miniconda not found at $env:USERPROFILE\Miniconda3." -ForegroundColor Red
    Write-Host "Please install Miniconda before running this script."
    Write-Host "Important: During installation, decline any PATH modifications." -ForegroundColor Yellow
    exit 1
}

# --- Load Conda ---
& $condaSh

# --- Check if vk-env exists ---
$envs = conda env list | Select-String "vk-env"
if (-not $envs) {
    Write-Host "Creating vk-env Conda environment..." -ForegroundColor Cyan
    conda create -y -n vk-env

    Write-Host "Activating vk-env..." -ForegroundColor Cyan
    conda activate vk-env

    Write-Host "Installing dependencies..." -ForegroundColor Cyan
    conda install -y -n vk-env -c schrodinger -c conda-forge `
        cmake `
        ninja `
        clang `
        shader-slang
} else {
    Write-Host "vk-env environment already exists." -ForegroundColor Green
    Write-Host "Activating vk-env..." -ForegroundColor Cyan
    conda activate vk-env
}

# --- Final messages ---
Write-Host "Setup complete. Remember to have your GPU drivers installed!" -ForegroundColor Green
Write-Host "Important: Make sure you have the Vulkan SDK downloaded and installed before using Vulkan applications." -ForegroundColor Yellow
Write-Host "You can get it from: https://vulkan.lunarg.com/sdk/home" -ForegroundColor Cyan
Write-Host ""
