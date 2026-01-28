
<img src="assets/vulkron.png" width="100%"/>

## Introduction

Vulkron is a cross-platform rendering framework built on Vulkan for high-performance graphics and SDL3 for windowing and input management. It is designed to be developed and run on both Windows and Linux, with Conda providing a consistent and reproducible development environment across platforms.

Shaders in Vulkron are authored using Slang, a modern, flexible shading language that enables expressive and efficient programmable effects. By combining Vulkan’s low-level control with Slang’s high-level flexibility, Vulkron provides a foundation for graphics experimentation, engine development, and advanced rendering research.

## Building Vulkron

### System Dependencies

* Vulkan SDK
* Up-to-date GPU drivers
* Miniconda3

### Conda Dependencies

| Package      | Purpose                          |
| ------------ | -------------------------------- |
| sdl3         | SDL3 library with Vulkan support |
| cmake        | Build system for C++ projects    |
| ninja        | Optional fast build system       |
| clang        | Project compiler                 |
| shader-slang | Shader authoring and compilation |

## Build Process

### 1. Clone the Repository

```bash
git clone https://github.com/bisqottii/vulkron.git
```

### 2. Configure the Conda Environment

This step installs all required development dependencies using Conda.

#### Linux
```bash
source platform/linux/configure_conda_env.sh
```

#### Windows
PowerShell may block the execution of local scripts. Before running any setup scripts, allow execution for the current session only:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\platform\windows\configure_conda_env.ps1
```

**What the script does**  
* Checks for an existing Miniconda installation
* Creates a Conda environment named `vk-env` if it does not exist
* Installs all required dependencies into the environment
* Activates the environment for the current shell session

**When to use it:**  
Run this script after cloning the repository and before building Vulkron. It ensures a consistent development environment without requiring manual dependency installation.

---

### 3. Build the Project

Platform-specific build scripts are provided for different configurations:

* **Release** - for testing and showcasing the renderer
* **Debug** - for active development and debugging

Run the appropriate script for your platform:

* `platform/<os>/build_release.*`
* `platform/<os>/build_debug.*`

These scripts are located in the corresponding `platform/<os>/` directory.

**Targets you can specify**:

* `vulkron` – main renderer

If no target is specified, the scripts build **all executables** in `tools/`. Each executable links to the shared `core` library, which is automatically built first.

## Notes
* Ensure your **GPU drivers** and the **Vulkan SDK** are properly installed.
* The Conda environment supplies **SDL3 and build tools**; Vulkan runtime components are provided by the Vulkan SDK.

