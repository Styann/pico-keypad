/**
 * @author Styann
 */

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "../lib/pico_extra/pico_extra.h"

#include "../lib/usb/usb.h"
#include "../lib/usb/usb_hid.h"

#include "../lib/debounce/debounce.h"
#include "../lib/keyboard/keyboard.h"
#include "../lib/rotary_encoder/rotary_encoder.h"
#include "../lib/ws2812b/ws2812b.h"
#include "../lib/joystick8way/joystick8way.h"
#include "../lib/button/button.h"
#include "../lib/macro/macro.h"

#include "../lib/ssd1331/ssd1331.h"
#include "../lib/ssd1306/ssd1306.h"
#include "../lib/st7735s/st7735s.h"

#include <stdio.h>

#define USE_KEYBOARD
#define USE_KNOB
// #define USE_FIGHTSTICK
// #define USE_WS2812B

// #include "../include/blaziken.h"
// #define USE_SSD1331
// #define USE_SSD1306
#define USE_ST7735S

#define DEBOUNCE_MS 10

volatile uint8_t modifiers = 0b00000000;
volatile bool is_fn_pressed = false;

#ifdef USE_KNOB
    void knob_cw_callback(uint32_t state) {
        if (is_fn_pressed) {
            usb_send_consumer_control(CC_NEXT_TRACK);
        }
        else if (modifiers & MOD_CTRL_LEFT) {
            usb_send_single_keycode(KC_ARROW_DOWN);
        }
        else {
            usb_send_consumer_control(CC_VOLUME_INCREMENT);
        }
    }

    void knob_ccw_callback(uint32_t state) {
        if (is_fn_pressed) {
            usb_send_consumer_control(CC_PREVIOUS_TRACK);
        }
        else if (modifiers & MOD_CTRL_LEFT) {
            usb_send_single_keycode(KC_ARROW_UP);
        }
        else {
            usb_send_consumer_control(CC_VOLUME_DECREMENT);
        }
    }

    rotary_encoder_t volume_knob = {
        .pin_DT = GPIO26,
        .pin_CLK = GPIO27,
        .cw_callback = &knob_cw_callback,
        .ccw_callback = &knob_ccw_callback
    };

    button_t knob_btn = { .pin = GPIO28, .debounce_ms = 10};
#endif

#ifdef USE_KEYBOARD
    #define LAYOUT_COLUMN_SIZE 8
    #define LAYOUT_ROW_SIZE 8

    const uint8_t columns_pins[LAYOUT_COLUMN_SIZE] = { GPIO22, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7 };
    const uint8_t rows_pins[LAYOUT_ROW_SIZE] = { GPIO8, GPIO9, GPIO10, GPIO11, GPIO12, GPIO13, GPIO14, GPIO15 };

    const uint8_t layout[8][8] = {
        { KC_ESCAPE,     KC_1,        KC_2,        KC_3,                   KC_4,                    KC_5,          KC_6,           KC_7           },
        { KC_TAB,        KC_A,        KC_Z,        KC_E,                   KC_R,                    KC_T,          KC_Y,           KC_U           },
        { KC_CAPS_LOCK,  KC_Q,        KC_S,        KC_D,                   KC_F,                    KC_G,          KC_H,           KC_J           },
        { KC_SHIFT_LEFT, KC_W,        KC_X,        KC_C,                   KC_V,                    KC_B,          KC_N,           KC_COMMA       },
        { KC_CTRL_LEFT,  KC_GUI_LEFT, KC_ALT_LEFT, KC_SPACE,               KC_FN,                   KC_CTRL_RIGHT, KC_ARROW_LEFT,  KC_ARROW_DOWN  },
        { KC_8,          KC_9,        KC_0,        KC_MINUS,               KC_EQUAL,                KC_BACKSPACE,  KC_DELETE,      KC_ARROW_RIGHT },
        { KC_I,          KC_O,        KC_P,        KC_SQUARE_BRACKET_LEFT, KC_SQUARE_BRACKET_RIGHT, KC_BACKSLASH,  KC_SHIFT_RIGHT, KC_ARROW_UP    },
        { KC_K,          KC_L,        KC_M,        KC_SEMICOLON,           KC_APOSTROPHE,           KC_ENTER,      KC_PERIOD,      KC_SLASH       }
    };

    const uint8_t fn_layout[8][8] = {
        { KC_ESCAPE,     KC_F1,       KC_F2,       KC_F3,                  KC_F4,                   KC_F5,         KC_F6,          KC_F7          },
        { KC_TAB,        KC_A,        KC_Z,        KC_E,                   KC_R,                    KC_T,          KC_Y,           KC_U           },
        { KC_CAPS_LOCK,  KC_Q,        KC_PRINT_SCREEN, KC_D,               KC_F11,                  KC_G,          KC_H,           KC_J           },
        { KC_SHIFT_LEFT, KC_W,        KC_X,        KC_C,                   KC_V,                    KC_B,          KC_GRAVE,       KC_COMMA       },
        { KC_CTRL_LEFT,  KC_GUI_LEFT, KC_ALT_LEFT, KC_SPACE,               KC_FN,                   KC_CTRL_RIGHT, KC_ARROW_LEFT,  KC_ARROW_DOWN  },
        { KC_F8,         KC_F9,       KC_F10,      KC_MINUS,               KC_EQUAL,                KC_BACKSPACE,  KC_DELETE,      KC_ARROW_RIGHT },
        { KC_F12,        KC_O,        KC_P,        KC_SQUARE_BRACKET_LEFT, KC_SQUARE_BRACKET_RIGHT, KC_BACKSLASH,  KC_SHIFT_RIGHT, KC_ARROW_UP    },
        { KC_K,          KC_L,        KC_M,        KC_SEMICOLON,           KC_APOSTROPHE,           KC_ENTER,      KC_PERIOD,      KC_SLASH       }
    };

    keyboard_matrix_t keyboard_matrix = {
        .layout = &layout[0][0],
        .fn_layout = &fn_layout[0][0],
        .rows_pins = rows_pins,
        .columns_pins = columns_pins,
        .row_size = LAYOUT_ROW_SIZE,
        .column_size = LAYOUT_COLUMN_SIZE
    };

    bool macro_task(struct usb_keyboard_report *report) {
        bool match = false;

        modifiers = report->modifiers;
        is_fn_pressed = false;

        if (report->keycodes[0] == KC_FN) {
            is_fn_pressed = true;

            if (report->keycodes[1] == KC_O) {
                if (multicore_fifo_wready()) multicore_fifo_push_blocking(0x0FF);
                match = true;
            }
        }
        else if (report->modifiers == MOD_CTRL_LEFT | MOD_CTRL_RIGHT) {
            switch (report->keycodes[0]) {
                case KC_ARROW_LEFT:
                    usb_send_single_keycode(KC_HOME);
                    match = true;
                    break;
                case KC_ARROW_RIGHT:
                    usb_send_single_keycode(KC_END);
                    match = true;
                    break;
            }
        }

        return match;
    }

    void keyboard_task(void) {
        static uint32_t timer = 0;

        if (debounce(&timer, DEBOUNCE_MS)) {
            static bool can_release = false;
            static struct usb_keyboard_report previous_report = {};

            struct usb_keyboard_report keyboard_report = { 0x01, 0, 0, { 0, 0, 0, 0, 0, 0 } };
            keyboard_matrix_scan(&keyboard_matrix, &keyboard_report);

            // if there is a change between actual and previous report
            if (!keyboard_report_cmp(&keyboard_report, &previous_report)) {
                if (!macro_task(&keyboard_report)) {
                    if (!is_keyboard_report_empty(&keyboard_report)) {
                        usb_send_keyboard_report(&keyboard_report);
                        can_release = true;
                    }
                    else if (can_release) {
                        release_keyboard();
                        can_release = false;
                    }
                }

                previous_report = keyboard_report;
            }
        }
    }
#endif

#ifdef USE_FIGHTSTICK
    struct joystick8way stick = {
        .up_pin = GPIO2,
        .right_pin = GPIO3,
        .down_pin = GPIO4,
        .left_pin = GPIO5,
    };

    button_t a = { .pin = GPIO6, .debounce_ms = 10 };
    button_t b = { .pin = GPIO7, .debounce_ms = 10 };
    button_t x = { .pin = GPIO8, .debounce_ms = 10 };
    button_t y = { .pin = GPIO9, .debounce_ms = 10 };
    button_t lb = { .pin = GPIO10, .debounce_ms = 10 };
    button_t lt = { .pin = GPIO11, .debounce_ms = 10 };
    button_t rb = { .pin = GPIO12, .debounce_ms = 10 };
    button_t rt = { .pin = GPIO13, .debounce_ms = 10 };

    button_init(&a, true);
    button_init(&b, true);
    button_init(&x, true);
    button_init(&y, true);
    button_init(&lb, true);
    button_init(&lt, true);
    button_init(&rb, true);
    button_init(&rt, true);

    void fightstick_task(void) {
        static uint32_t timer = 0;
        static bool can_release = false;

        static struct usb_gamepad_report previous_report = {};

        if (debounce(&timer, DEBOUNCE_MS)) {
            struct usb_gamepad_report gamepad_report = {};

            gamepad_report.x = joystick8way_read_x_axis(&stick);
            gamepad_report.y = joystick8way_read_y_axis(&stick);
            gamepad_report.buttons |= button_read(&a);
            gamepad_report.buttons |= button_read(&b) << 1;
            gamepad_report.buttons |= button_read(&x) << 2;
            gamepad_report.buttons |= button_read(&y) << 3;
            gamepad_report.buttons |= button_read(&lb) << 4;
            gamepad_report.buttons |= button_read(&lt) << 5;
            gamepad_report.buttons |= button_read(&rb) << 6;
            gamepad_report.buttons |= button_read(&rt) << 7;

            if (memcmp(&gamepad_report, &previous_report, sizeof(struct usb_gamepad_report)) != 0) {
                if (!is_gamepad_report_empty(&gamepad_report)) {
                    usb_send_gamepad_report(&gamepad_report);
                    can_release = true;
                }
                else {
                    if (can_release) {
                        usb_send_gamepad_report(&gamepad_report);
                        can_release = false;
                    }
                }

                previous_report = gamepad_report;
            }
        }
    }
#endif

#ifdef USE_SSD1331
    struct ssd1331 display = {
        .pin_DC = GPIO20,
        .pin_SDA = GPIO19,
        .pin_SCL = GPIO18,
        .pin_CS = GPIO17,
        .pin_RES = GPIO16,
        .spi_inst = spi0
    };
#endif

#ifdef USE_SSD1306
    ssd1306_128x32_t display2 = {
        .addr = 0x3C,
        .pin_SCK = GPIO18,
        .pin_SDA = GPIO19,
        .i2c_inst = i2c1
    };
#endif

#ifdef USE_ST7735S
    st7735s_t display3 = {
        .pin_SCL = GPIO18,
        .pin_SDA = GPIO19,
        .pin_RES = GPIO16,
        .pin_DC = GPIO20,
        .pin_CS = GPIO17,
        .pin_BLK = GPIO21,
        .spi_inst = spi0
    };
#endif

void main_core1(void) {
    #ifdef USE_WS2812B
        grb32_t leds_buffer[30];
        uint8_t leds_spi_buffer[30][GRB_BIT_SIZE];

        struct ws2812b led_strip = {
            .pin_mosi = GPIO19,
            .spi_inst = spi0,
            .num_leds = 30,
            .leds_buffer = leds_buffer,
            .spi_buffer = &leds_spi_buffer[0][0],
        };

        ws2812b_init(&led_strip);

        while (true) {
            ws2812b_set_all(&led_strip, GRB_ORANGE);
            ws2812b_set_brightness(&led_strip, 0.8);
            ws2812b_write(&led_strip);
            sleep_ms(1000);

            ws2812b_set_brightness(&led_strip, 0.5);
            // ws2812b_set_all(&led_strip, GRB_YELLOW);
            ws2812b_write(&led_strip);
            sleep_ms(1000);

            // ws2812b_set_all(&led_strip, GRB_MAGENTA);
            ws2812b_set_brightness(&led_strip, 0.1);
            ws2812b_write(&led_strip);
            sleep_ms(1000);
        }
    #endif

    #ifdef USE_SSD1331
        ssd1331_fill_screen(&display, 0);

        // unsigned int input = 1235467890;
        // char input_str[10];
        // sprintf(input_str, "%d", input);
        // ssd1331_print(&display, 0, 0, input_str);

        ssd1331_println(&display, "arquebus keyboard");

        // while(true) {
        //     for (uint8_t i = 0; i < 87; i++) {
        //         ssd1331_write_data(&display, (uint16_t*)blaziken_framebuffer[i], SSD1331_RESOLUTION);
        //         sleep_ms(40);
        //     }
        // }

        while (true) {
            // if (display.is_on) {}

            if (multicore_fifo_rvalid()) {
                uint32_t signal = multicore_fifo_pop_blocking();

                if (signal == 0x0FF) {
                    ssd1331_turn(&display, !display.is_on);
                }

                multicore_fifo_drain();
            }

            sleep_ms(60);
        }
    #endif

    #ifdef USE_ST7735S
        st7735s_init(&display3, 8000000);

        while (true) {
            asm volatile("nop");
        }
    #endif

    #ifdef USE_SSD1306
        ssd1306_init(&display2, 400);

        while (true) {
            asm volatile("nop");
        }
    #endif
}

void set_report_callback(volatile uint8_t *buf, uint16_t len) {
    // printf("0x%x 0x%x\n", buf[0], buf[1]);
    release_keyboard();
}

int main(void) {
    stdio_init_all();

    printf("Hello, World! from pi pico.\n");

    usb_device_init();
    // ssd1331_init(&display);

    multicore_launch_core1(main_core1);

    built_in_led_init();

    #ifdef USE_KEYBOARD
        keyboard_matrix_init(&keyboard_matrix);
    #endif

    #ifdef USE_KNOB
        rotary_encoder_init(&volume_knob, true);
        button_init(&knob_btn, true);
    #endif

    #ifdef USE_FIGHTSTICK
        joystick8way_init(&stick);
        button_init(a);
        button_init(b);
        button_init(x);
        button_init(y);
        button_init(lb);
        button_init(lt);
        button_init(rb);
        button_init(rt);
    #endif

    while (!is_configured()) {
        tight_loop_contents();
    }

    while (true) {
        #ifdef USE_KEYBOARD
            keyboard_task();
        #endif

        #ifdef USE_KNOB
            rotary_encoder_task(&volume_knob);

            if (button_debounce(&knob_btn) && button_is_pressed_and_state_changed(&knob_btn)) {
                if (is_fn_pressed) {
                    usb_send_consumer_control(CC_MUTE);
                }
                else {
                    usb_send_consumer_control(CC_PLAY_PAUSE);
                }
            }
        #endif

        #ifdef USE_FIGHTSTICK
            fightstick_task();
        #endif
    }

    return 0;
}
