cmake_minimum_required(VERSION 3.28)
project(Vulkron LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# cache variables for installation destinations
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(source/gpu)
add_subdirectory(source/ui)
# add_subdirectory(source/support)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT VulkronTargets
    FILE VulkronTargets.cmake
    NAMESPACE Vulkron::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vulkron
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/VulkronConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/VulkronConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vulkron
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/VulkronConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vulkron
)