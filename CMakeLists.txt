cmake_minimum_required(VERSION 3.12)

#
# sdk setup
include(lib/pico-sdk/pico_sdk_init.cmake)
if (PICO_SDK_VERSION_STRING VERSION_LESS "2.1.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.1.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Name project
SET(ProjectName blink)
SET(PICO_BOARD pico)
#SET(PICO_BOARD pico_w)

# point to FreeRTOS library
# set include path
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName})

# init sdk
pico_sdk_init()


# Add subdirectories
add_subdirectory(src)
#add_subdirectory(lib)