#!/bin/bash

# --- Colors ---
RED='\033[0;31m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color


# --- Check if the script is sourced ---
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo -e "${YELLOW}Please run this script as:"
    echo -e "    ${NC}source $(basename "${BASH_SOURCE[0]}")"
    echo -e "Otherwise, '${YELLOW}conda activate${NC}' won't persist in your current shell."
    return 0 2>/dev/null || exit 0
fi

# --- Check if Miniconda exists ---
if [ ! -f "$HOME/miniconda3/etc/profile.d/conda.sh" ]; then
    echo -e "${RED}Miniconda not found at ~/miniconda3.${NC}"
    echo -e "Please install Miniconda before running this script"
    echo -e "${YELLOW}Important:${NC} During installation, decline any PATH modifications."
    echo -e "This script activates the environment per session using 'source'."
    return 0 2>/dev/null || exit 0
else
    # Load Conda
    source ~/miniconda3/etc/profile.d/conda.sh

    # Create Vulkan environment if it doesn't exist
    if ! conda info --envs | grep -q "vk-env"; then
        echo "Creating vk-env Conda environment..."
        conda create -y -n vk-env
        
        # Activate the environment
        conda activate vk-env

        # Install dependencies
        echo "Installing dependencies in vk-env..."
        conda install -c schrodinger -c conda-forge \
            cmake \
            ninja \
            clang \
            shader-slang

    else
        echo -e "${GREEN}vk-env environment already exists.${NC}"
        
        # Activate the environment
        conda activate vk-env
    fi

fi

# --- Final messages ---
echo -e "${GREEN}Setup complete. Remember to have your GPU drivers installed!${NC}"
echo -e "${YELLOW}Important:${NC} Make sure you have the Vulkan SDK downloaded and installed before using Vulkan applications."
echo -e "You can get it from: https://vulkan.lunarg.com/sdk/home"
echo -e ""