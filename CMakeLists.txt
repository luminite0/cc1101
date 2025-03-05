target_sources(cc1101
    PRIVATE
        cc1101.c
        cc1101.h
)

target_link_libraries(cc1101
    pico_stdlib
    hardware_spi
    pico_cyw43_arch_none
)
