cmake_minimum_required(VERSION 3.25)

include(pico_sdk_import.cmake)

project(pico-keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico-keypad
    ${PICO_KEYPAD_PATH}/src/pico-keypad.c
    ${PICO_KEYPAD_PATH}/src/keyboard/keyboard.c
    ${PICO_KEYPAD_PATH}/src/usb/usb.c
    ${PICO_KEYPAD_PATH}/src/usb/usb_hid.c
    ${PICO_KEYPAD_PATH}/src/hw040/hw040.c
    ${PICO_KEYPAD_PATH}/src/ws2812b/ws2812b.c
    ${PICO_KEYPAD_PATH}/src/ssd1331/ssd1331.c
    ${PICO_KEYPAD_PATH}/src/button/button.c
    ${PICO_KEYPAD_PATH}/src/joystick/joystick.c
    ${PICO_KEYPAD_PATH}/src/macro/macro.c
)

target_sources(pico-keypad PUBLIC
    ${PICO_KEYPAD_PATH}/src/pico-keypad.c
    ${PICO_KEYPAD_PATH}/src/keyboard/keyboard.c
    ${PICO_KEYPAD_PATH}/src/usb/usb.c
    ${PICO_KEYPAD_PATH}/src/usb/usb_hid.c
    ${PICO_KEYPAD_PATH}/src/hw040/hw040.c
    ${PICO_KEYPAD_PATH}/src/ws2812b/ws2812b.c
    ${PICO_KEYPAD_PATH}/src/ssd1331/ssd1331.c
    ${PICO_KEYPAD_PATH}/src/button/button.c
    ${PICO_KEYPAD_PATH}/src/joystick/joystick.c
    ${PICO_KEYPAD_PATH}/src/macro/macro.c
)

target_link_libraries(pico-keypad
    pico_stdlib
    pico_multicore
    hardware_pio
    hardware_resets
    hardware_irq
    hardware_pwm
    hardware_spi
)

pico_add_extra_outputs(pico-keypad)

message(${CMAKE_C_COMPILER})
message(${CMAKE_CXX_COMPILER})
message(${CMAKE_CXX_COMPILER_AR})
message(${CMAKE_CXX_COMPILER_RANLIB})