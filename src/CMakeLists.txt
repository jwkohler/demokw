## default project - blinkh
## targets picoh board


add_executable(${ProjectName}
    main.cpp
    valve.cpp
    command.cpp
    utils/hw_claims.cpp
    utils/utils.cpp
    utils/allocators.cpp
)

include_directories(${ProjectName}
    utils
)

target_compile_definitions(${ProjectName} PRIVATE 
    IO_RUN=16
    IO_BUSY=18
    IO_VALVE=19
    IO_DEBUG_CLK=17
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(${ProjectName} PRIVATE 
    PICO_BOOTSEL_VIA_DOUBLE_RESET_ACTIVITY_LED=25
    PICO_BOOTSEL_VIA_DOUBLE_RESET_TIMEOUT_MS=700
    PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=20000
)

target_link_libraries(${ProjectName} 
    pico_stdlib 
    pico_bootsel_via_double_reset   # double tap reset to go BOOTSEL
    FreeRTOS-Kernel-Heap3
    )

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(${ProjectName} 
        pico_cyw43_arch_none
    )
endif()


# additional build outputs: .map, .dis, .uf2, etc.
pico_add_extra_outputs(${ProjectName})
# add USB stdio driver - enabled
pico_enable_stdio_usb( ${ProjectName} 1)
# add UART stdio driver - disabled
pico_enable_stdio_uart( ${ProjectName} 0)

pico_set_linker_script(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/memmap_custom.ld)

## ----------------------------------------------------------------------
## 1) copy 'load' script to the output dir (build)
## 2) update the magic word (ProjectName) with the current ${ProjectName}
add_custom_command( 
    TARGET ${ProjectName} POST_BUILD
    COMMAND cp -v ${CMAKE_CURRENT_SOURCE_DIR}/../load ${CMAKE_BINARY_DIR}
    COMMAND sed --in-place s/LOAD_TARGET/${ProjectName}/ ${CMAKE_BINARY_DIR}/load
)

