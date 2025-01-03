cmake_minimum_required(VERSION 3.25)

include(pico_sdk_import.cmake)

project(pico-keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

message("PICO_KEYPAD_PATH is $ENV{PICO_KEYPAD_PATH}")

add_executable(pico-keypad
    $ENV{PICO_KEYPAD_PATH}/src/pico-keypad.c
    $ENV{PICO_KEYPAD_PATH}/lib/debounce/debounce.c
    $ENV{PICO_KEYPAD_PATH}/lib/keyboard/keyboard.c
    $ENV{PICO_KEYPAD_PATH}/lib/usb/usb.c
    $ENV{PICO_KEYPAD_PATH}/lib/usb/usb_hid.c
    $ENV{PICO_KEYPAD_PATH}/lib/rotary_encoder/rotary_encoder.c
    $ENV{PICO_KEYPAD_PATH}/lib/ws2812b/ws2812b.c
    $ENV{PICO_KEYPAD_PATH}/lib/ssd1331/ssd1331.c
    $ENV{PICO_KEYPAD_PATH}/lib/ssd1306/ssd1306.c
    $ENV{PICO_KEYPAD_PATH}/lib/st7735s/st7735s.c
    $ENV{PICO_KEYPAD_PATH}/lib/button/button.c
    $ENV{PICO_KEYPAD_PATH}/lib/joystick8way/joystick8way.c
    $ENV{PICO_KEYPAD_PATH}/lib/graphics/color.c
    $ENV{PICO_KEYPAD_PATH}/lib/led/led.c
)

target_sources(pico-keypad PUBLIC
    $ENV{PICO_KEYPAD_PATH}/src/pico-keypad.c
    $ENV{PICO_KEYPAD_PATH}/lib/debounce/debounce.c
    $ENV{PICO_KEYPAD_PATH}/lib/keyboard/keyboard.c
    $ENV{PICO_KEYPAD_PATH}/lib/usb/usb.c
    $ENV{PICO_KEYPAD_PATH}/lib/usb/usb_hid.c
    $ENV{PICO_KEYPAD_PATH}/lib/rotary_encoder/rotary_encoder.c
    $ENV{PICO_KEYPAD_PATH}/lib/ws2812b/ws2812b.c
    $ENV{PICO_KEYPAD_PATH}/lib/ssd1331/ssd1331.c
    $ENV{PICO_KEYPAD_PATH}/lib/ssd1306/ssd1306.c
    $ENV{PICO_KEYPAD_PATH}/lib/st7735s/st7735s.c
    $ENV{PICO_KEYPAD_PATH}/lib/button/button.c
    $ENV{PICO_KEYPAD_PATH}/lib/joystick8way/joystick8way.c
    $ENV{PICO_KEYPAD_PATH}/lib/graphics/color.c
    $ENV{PICO_KEYPAD_PATH}/lib/led/led.c
)

target_link_libraries(pico-keypad
    pico_stdlib
    pico_multicore
    hardware_pio
    hardware_resets
    hardware_irq
    hardware_pwm
    hardware_spi
    hardware_i2c
)

pico_add_extra_outputs(pico-keypad)

message(${CMAKE_C_COMPILER})
message(${CMAKE_CXX_COMPILER})
message(${CMAKE_CXX_COMPILER_AR})
message(${CMAKE_CXX_COMPILER_RANLIB})
