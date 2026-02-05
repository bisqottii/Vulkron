#!/usr/bin/env python3
"""
Build Vulkron examples using installed libraries.
"""

import subprocess
from pathlib import Path

def main():
    # Setup paths
    project = Path.cwd()
    install = project.parent / "vulkron-build" / "install"
    examples = project / "examples"
    build = project.parent / "vulkron-build" / "examples-build"
    
    print(f"Building examples using installed libraries...")
    print(f"Install directory: {install}")
    print(f"Examples source: {examples}")
    print(f"Build directory: {build}")
    
    # Create build directory
    build.mkdir(parents=True, exist_ok=True)
    
    # Configure
    subprocess.run([
        "cmake",
        "-G", "Ninja",
        "-B", str(build),
        "-S", str(examples),
        "-DCMAKE_BUILD_TYPE=release",
        f"-DCMAKE_PREFIX_PATH={install}",
        f"-DCMAKE_INSTALL_PREFIX={install}"
    ], check=True)
    
    # Build
    subprocess.run([
        "ninja",
        "-C", str(build)
    ], check=True)
    
    # Install
    subprocess.run([
        "cmake",
        "--install", str(build)
    ], check=True)
    
    print(f"\nExamples built and installed to: {install}")

if __name__ == "__main__":
    main()