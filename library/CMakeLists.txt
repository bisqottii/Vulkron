find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED)

add_library(core STATIC
    gpu_context.cpp

    vulkan/backend.cpp
    vulkan/device.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    # PRIVATE # Temp commented out so I can test it in main
        ${Vulkan_INCLUDE_DIRS}
        $ENV{CONDA_PREFIX}/include

        ${CMAKE_CURRENT_SOURCE_DIR}/vulkan
        ${CMAKE_CURRENT_SOURCE_DIR}/graphs
        ${CMAKE_CURRENT_SOURCE_DIR}/renderdoc
        ${CMAKE_CURRENT_SOURCE_DIR}/support
        
)

target_link_libraries(core
    PRIVATE
        Vulkan::Vulkan
        SDL3::SDL3
        ${SLANG_LIBRARY}
)
