cmake_minimum_required(VERSION 3.25)

include(pico_sdk_import.cmake)

#include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(pico-numpad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico-numpad
    ${PICO_NUMPAD_PATH}/pico-numpad.c
    ${PICO_NUMPAD_PATH}/lib/tinyusb/usb_descriptors.c
    ${PICO_NUMPAD_PATH}/Keys/Keys.c
)

target_sources(pico-numpad PUBLIC
    ${PICO_NUMPAD_PATH}/pico-numpad.c
    ${PICO_NUMPAD_PATH}/lib/tinyusb/usb_descriptors.c
    ${PICO_NUMPAD_PATH}/Keys/Keys.c
)

target_include_directories(pico-numpad PUBLIC
    ${PICO_SDK_PATH}/src/lib/tinyusb
)

#FreeRTOS include
#add_library(FreeRTOS STATIC
#    ${FREERTOS_KERNEL_PATH}/tasks.c
#    ${FREERTOS_KERNEL_PATH}/queue.c
#    ${FREERTOS_KERNEL_PATH}/list.c
#)

target_link_libraries(pico-numpad
    pico_stdlib
    pico_multicore
    hardware_pio
    hardware_adc
    #FreeRTOS-Kernel
    #FreeRTOS-Kernel-Heap4
    tinyusb_device
    tinyusb_board
)

pico_add_extra_outputs(pico-numpad)
pico_enable_stdio_usb(pico-numpad 1)