find_package(Vulkan REQUIRED)

add_library(vulkron-gpu)

set_target_properties(vulkron-gpu PROPERTIES
    EXPORT_NAME GPU
    OUTPUT_NAME libvulkron-gpu
)

target_sources(vulkron-gpu
    PRIVATE
        context.cpp

        vulkan/device.cpp
        vulkan/surface.cpp
)

target_include_directories(vulkron-gpu
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${Vulkan_INCLUDE_DIRS}
        $ENV{CONDA_PREFIX}/include

        ${CMAKE_CURRENT_SOURCE_DIR}/vulkan
        ${CMAKE_CURRENT_SOURCE_DIR}/graphs
)

target_link_libraries(vulkron-gpu 
    PRIVATE
        Vulkan::Vulkan
)

install(TARGETS vulkron-gpu
    EXPORT VulkronTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
