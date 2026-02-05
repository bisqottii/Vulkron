import subprocess
import shutil
from pathlib import Path

# Setup paths
project = Path.cwd()
build = project.parent / "vulkron-build" / "build" / "release"
install = project.parent / "vulkron-build" / "install"

# Build and install
subprocess.run([
    "cmake", 
    "-G", "Ninja", 
    "-B", str(build), 
    "-S", str(project), 
    "-DCMAKE_BUILD_TYPE=release"
], check=True)

subprocess.run([
    "ninja", 
    "-C", str(build)
], check=True)

subprocess.run([
    "cmake", 
    "--install", str(build), 
    "--prefix", str(install)
], check=True)

# Copy documentation if it exists
if (project / "docs").exists():
    shutil.copytree(project / "docs", install / "docs", dirs_exist_ok=True)

print(f"Installed to: {install}")