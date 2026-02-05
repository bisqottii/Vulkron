import argparse
import sys
import subprocess
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(
        description="Build script for Vulkron project",
        add_help=False
    )
    parser.add_argument("--config", 
                       choices=["debug", "release"],
                       help="Build configuration (debug or release)")
    parser.add_argument("--target", 
                       default="all",
                       help="Build target (default: all)")
    parser.add_argument("--help", 
                       action="store_true",
                       help="Show this help message")
    
    args = parser.parse_args()
    
    if args.help:
        print("\nUsage: build.py --config <debug|release> [--target <target>]")
        print("")
        sys.exit(0)
    
    if not args.config:
        print("Missing required parameter: --config <configuration>")
        print("\nUsage: build.py --config <debug|release> [--target <target>]")
        print("")
        sys.exit(1)
    
    # Setup directories
    project_root = Path.cwd()
    source_root = project_root.parent
    build_root = source_root / "vulkron-build"
    build_dir = build_root / "build" / args.config
    
    # Create build directory
    build_dir.mkdir(parents=True, exist_ok=True)
    
    # Configure CMake - let it find compilers from PATH automatically
    print(f"Configuring CMake with {args.config} configuration...")
    
    cmake_cmd = [
        "cmake",
        "-G", "Ninja",
        "-B", str(build_dir),
        "-S", str(project_root),
        "-DCMAKE_BUILD_TYPE=" + args.config,
    ]
    
    try:
        subprocess.run(cmake_cmd, check=True)
    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        print(f"CMake configuration failed: {e}")
        sys.exit(1)
    
    # Build with Ninja
    print("Building Vulkron...")
    
    ninja_cmd = ["ninja", "-C", str(build_dir)]
    if args.target != "all":
        ninja_cmd.append(args.target)
    
    try:
        subprocess.run(ninja_cmd, check=True)
    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        print(f"Build failed: {e}")
        sys.exit(1)
    
    print("\nBuild completed successfully!")

if __name__ == "__main__":
    main()