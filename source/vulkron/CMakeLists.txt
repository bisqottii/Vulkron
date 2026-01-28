add_executable(vulkron
    main.cpp
)

target_link_libraries(vulkron 
    PRIVATE 
        core
)

install(TARGETS vulkron
    RUNTIME DESTINATION 
        bin
)