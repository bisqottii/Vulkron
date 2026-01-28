if(NOT DEFINED ENV{CONDA_PREFIX})
    message(FATAL_ERROR "Conda environment not activated! Please activate your vk-env.")
endif()

add_library(core STATIC
    gpu_context.cpp

    vulkan/vk_backend.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vulkan
        ${CMAKE_CURRENT_SOURCE_DIR}/graphs
        ${CMAKE_CURRENT_SOURCE_DIR}/renderdoc
        $ENV{CONDA_PREFIX}/include
)

find_library(SDL3_LIBRARY SDL3 PATHS $ENV{CONDA_PREFIX}/lib)
find_library(SLANG_LIBRARY slang PATHS $ENV{CONDA_PREFIX}/lib)

target_link_libraries(core 
    PRIVATE 
        ${SDL3_LIBRARY} 
        ${SLANG_LIBRARY}
)