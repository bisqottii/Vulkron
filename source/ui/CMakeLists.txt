
add_library(vulkron-ui)

set_target_properties(vulkron-ui PROPERTIES
    EXPORT_NAME UI
    OUTPUT_NAME libvulkron-ui
)

if(WIN32)
    target_sources(vulkron-ui
        PRIVATE
            window.cpp
            platform/win32.cpp
    )
elseif(UNIX AND NOT APPLE)
    target_sources(vulkron-ui
        PRIVATE
            window.cpp
    )
endif()

target_include_directories(vulkron-ui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $ENV{CONDA_PREFIX}/include

        ${CMAKE_CURRENT_SOURCE_DIR}/platform
)

install(TARGETS vulkron-ui
    EXPORT VulkronTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)