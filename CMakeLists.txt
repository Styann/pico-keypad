cmake_minimum_required(VERSION 3.25)

include(pico_sdk_import.cmake)

project(pico-keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico-keypad
    ${PICO_KEYPAD_PATH}/src/pico-keypad.c
    ${PICO_KEYPAD_PATH}/src/lib/tinyusb/usb_descriptors.c
    ${PICO_KEYPAD_PATH}/src/Keys/Keys.c
    ${PICO_KEYPAD_PATH}/src/usb/usb_device.c
)

target_sources(pico-keypad PUBLIC
    ${PICO_KEYPAD_PATH}/src/pico-keypad.c
    ${PICO_KEYPAD_PATH}/src/lib/tinyusb/usb_descriptors.c
    ${PICO_KEYPAD_PATH}/src/Keys/Keys.c
    ${PICO_KEYPAD_PATH}/src/usb/usb_device.c
)

target_include_directories(pico-keypad PUBLIC
    ${PICO_SDK_PATH}/src/lib/tinyusb
)

target_link_libraries(pico-keypad
    pico_stdlib
    pico_multicore
    hardware_pio
    hardware_adc
    tinyusb_device
    tinyusb_board
)

pico_add_extra_outputs(pico-keypad)
pico_enable_stdio_usb(pico-keypad 1)


message(${CMAKE_C_COMPILER})
message(${CMAKE_CXX_COMPILER})
message(${CMAKE_CXX_COMPILER_AR})
message(${CMAKE_CXX_COMPILER_RANLIB})