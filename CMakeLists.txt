cmake_minimum_required(VERSION 3.25)

include(pico_sdk_import.cmake)

project(pico-keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico-keypad
    ${PICO_KEYPAD_PATH}/src/pico-keypad.c
    ${PICO_KEYPAD_PATH}/lib/debounce/debounce.c
    ${PICO_KEYPAD_PATH}/lib/keyboard/keyboard.c
    ${PICO_KEYPAD_PATH}/lib/usb/usb.c
    ${PICO_KEYPAD_PATH}/lib/usb/usb_hid.c
    ${PICO_KEYPAD_PATH}/lib/usb/usb_config.c
    ${PICO_KEYPAD_PATH}/lib/rotary_encoder/rotary_encoder.c
    ${PICO_KEYPAD_PATH}/lib/ws2812b/ws2812b.c
    ${PICO_KEYPAD_PATH}/lib/ssd1331/ssd1331.c
    ${PICO_KEYPAD_PATH}/lib/button/button.c
    ${PICO_KEYPAD_PATH}/lib/joystick8way/joystick8way.c
    ${PICO_KEYPAD_PATH}/lib/macro/macro.c
)

target_sources(pico-keypad PUBLIC
    ${PICO_KEYPAD_PATH}/src/pico-keypad.c
    ${PICO_KEYPAD_PATH}/lib/debounce/debounce.c
    ${PICO_KEYPAD_PATH}/lib/keyboard/keyboard.c
    ${PICO_KEYPAD_PATH}/lib/usb/usb.c
    ${PICO_KEYPAD_PATH}/lib/usb/usb_hid.c
    ${PICO_KEYPAD_PATH}/lib/usb/usb_config.c
    ${PICO_KEYPAD_PATH}/lib/rotary_encoder/rotary_encoder.c
    ${PICO_KEYPAD_PATH}/lib/ws2812b/ws2812b.c
    ${PICO_KEYPAD_PATH}/lib/ssd1331/ssd1331.c
    ${PICO_KEYPAD_PATH}/lib/button/button.c
    ${PICO_KEYPAD_PATH}/lib/joystick8way/joystick8way.c
    ${PICO_KEYPAD_PATH}/lib/macro/macro.c
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
