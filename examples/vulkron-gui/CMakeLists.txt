# if(WIN32)
#     # Windows settings
#     message(STATUS "Configuring for Windows")
    
#     # Define platform macro (for main.cpp)
#     add_compile_definitions(VULKRON_PLATFORM_WINDOWS=1)
    
#     # Optional console mode for debugging
#     option(VULKRON_WINDOWS_CONSOLE "Build Windows app with console window" OFF)
#     if(VULKRON_WINDOWS_CONSOLE)
#         message(STATUS "  Building with console window")
#         add_compile_definitions(VULKRON_WINDOWS_CONSOLE=1)
#         add_executable(vulkron main.cpp)
#     else()
#         message(STATUS "  Building as GUI application (no console)")
#         add_executable(vulkron WIN32 main.cpp)
#     endif()
# endif()


find_package(Vulkron REQUIRED)

add_executable(vulkron-gui
    main.cpp
)

target_link_libraries(vulkron-gui
    PRIVATE
        Vulkron::UI
)

install(TARGETS vulkron-gui
    RUNTIME DESTINATION "bin/examples"
)