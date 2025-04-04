add_library(cc1101 INTERFACE)

target_include_directories(cc1101 INTERFACE 
    # As CMake processes the listfiles in your project this
    # variable will always be set to the directory where the 
    # listfile which is currently being processed 
    # (CMAKE_CURRENT_LIST_FILE) is located.
    ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(cc1101 INTERFACE
    cc1101.c
    cc1101.h
)

target_link_libraries(cc1101 INTERFACE
    pico_stdlib
    hardware_spi
    pico_cyw43_arch_none
)
