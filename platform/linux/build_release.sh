#!/bin/bash

set -e
TARGET=${1:-all}  # defaults to all if no argument is provided

# --- Get Conda env prefix ---
CONDA_PREFIX=$(conda info --base)/envs/vk-env
if [ ! -d "$CONDA_PREFIX" ]; then
    echo "Error: Conda environment 'vk-env' not found at $CONDA_PREFIX"
    exit 1
fi

# --- Out-of-source build ---
BUILD_DIR=../build/release
mkdir -p "$BUILD_DIR"

# --- Configure CMake using Clang/Ninja in the Conda environment ---
cmake -G Ninja \
      -B "$BUILD_DIR" \
      -S . \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER="$CONDA_PREFIX/bin/clang" \
      -DCMAKE_CXX_COMPILER="$CONDA_PREFIX/bin/clang++"

# --- Build with Ninja explicitly ---
ninja -C "$BUILD_DIR" "$TARGET"
