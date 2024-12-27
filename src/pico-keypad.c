/**
 * @author Styann
 */

#include <stdio.h>

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
#include "../lib/graphics/color.h"
#include "../lib/ws2812b/ws2812b.h"
#include "../lib/ssd1306/ssd1306.h"
#include "../lib/led/led.h"

#define USE_CAPSLOCK_LED
#define USE_KEYBOARD
#define USE_KNOB
#define USE_WS2812B
#define USE_SSD1306

#define DEBOUNCE_MS 10

volatile uint8_t modifiers = 0b00000000;
volatile bool is_fn_pressed = false;

#ifdef USE_CAPSLOCK_LED
led_t capslock_led = { .pin = GPIO27, .state = LOW };
#endif

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
    .pin_DT = GPIO22,
    .pin_CLK = GPIO26,
    .cw_callback = &knob_cw_callback,
    .ccw_callback = &knob_ccw_callback
};
#endif

#ifdef USE_KEYBOARD
#define LAYOUT_COLUMN_SIZE 8
#define LAYOUT_ROW_SIZE 9

const uint8_t columns_pins[LAYOUT_COLUMN_SIZE] = { GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7 };
const uint8_t rows_pins[LAYOUT_ROW_SIZE] = { GPIO8, GPIO9, GPIO10, GPIO11, GPIO12, GPIO13, GPIO14, GPIO15, GPIO18 };

const uint8_t layout[LAYOUT_ROW_SIZE][LAYOUT_COLUMN_SIZE] = {
    { KC_ESCAPE,     KC_1,        KC_2,         KC_3,                   KC_4,                    KC_5,         KC_6,           KC_7           },
    { KC_TAB,        KC_Q,        KC_W,         KC_E,                   KC_R,                    KC_T,         KC_Y,           KC_U           },
    { KC_CAPS_LOCK,  KC_A,        KC_S,         KC_D,                   KC_F,                    KC_G,         KC_H,           KC_J           },
    { KC_SHIFT_LEFT, KC_Z,        KC_X,         KC_C,                   KC_V,                    KC_B,         KC_N,           KC_M           },
    { KC_CTRL_LEFT,  KC_GUI_LEFT, KC_ALT_LEFT,  KC_SPACE,               KC_ALT_RIGHT,            KC_FN,        KC_CTRL_RIGHT,  KC_ARROW_RIGHT },
    { KC_8,          KC_9,        KC_0,         KC_MINUS,               KC_EQUAL,                KC_BACKSPACE, KC_GRAVE,       KC_PAUSE       },
    { KC_I,          KC_O,        KC_P,         KC_SQUARE_BRACKET_LEFT, KC_SQUARE_BRACKET_RIGHT, KC_BACKSLASH, KC_DELETE,      KC_NONE        },
    { KC_K,          KC_L,        KC_SEMICOLON, KC_APOSTROPHE,          KC_ENTER,                KC_HOME,      KC_ARROW_RIGHT, KC_NONE        },
    { KC_COMMA,      KC_PERIOD,   KC_SLASH,     KC_SHIFT_RIGHT,         KC_ARROW_UP,             KC_END,       KC_ARROW_DOWN , KC_NONE        },
};

keyboard_matrix_t keyboard_matrix = {
    .layout = &layout[0][0],
    .rows_pins = rows_pins,
    .columns_pins = columns_pins,
    .row_length = LAYOUT_ROW_SIZE,
    .column_length = LAYOUT_COLUMN_SIZE
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

#ifdef USE_SSD1306
ssd1306_t output = {
    .pin_SCK = GPIO21,
    .pin_SDA = GPIO20,
    .i2c_inst = i2c0
};
#endif

#ifdef USE_WS2812B
struct grb leds_buffer[16];
uint8_t leds_spi_buffer[16][bitsizeof(struct grb)];

struct ws2812b led_strip = {
    .pin_Din = GPIO19,
    .spi_inst = spi0,
    .num_leds = 16,
    .leds_buffer = leds_buffer,
    .spi_buffer = &leds_spi_buffer[0][0],
};
#endif

void main_core1(void) {
    #ifdef USE_SSD1306
    ssd1306_init(&output, 400000);
    ssd1306_print(&output, "Mazda =^.^=\nToyota >_<\nNissan :D");
    #endif

    #ifdef USE_WS2812B
    ws2812b_init(&led_strip);
    #endif

    while (true) {
        #ifdef USE_WS2812B
        for (uint8_t i = 0; i < 100; i++) {
            ws2812b_set_all(&led_strip, grb(i, 0, 0));
            ws2812b_write(&led_strip);
            sleep_ms(10);
        }

        for (uint8_t i = 0; i < 128; i++) {
            ws2812b_set_all(&led_strip, grb(0, i, 0));
            ws2812b_write(&led_strip);
            sleep_ms(10);
        }

        for (uint8_t i = 0; i < 128; i++) {
            ws2812b_set_all(&led_strip, grb(0, 0, i));
            ws2812b_write(&led_strip);
            sleep_ms(10);
        }
        #endif
    }
}

void set_report_callback(volatile uint8_t *buf, uint16_t len) {
    release_keyboard();

    #ifdef USE_CAPSLOCK_LED
    led_toggle(&capslock_led);
    #endif
}

int main(void) {
    built_in_led_init();

    #ifdef USE_CAPSLOCK_LED
    led_init(&capslock_led);
    #endif

    usb_device_init();

    while (!is_configured()) {
        tight_loop_contents();
    }

    built_in_led_on();

    uart_init(uart0, 115200);
    gpio_set_function(GPIO16, UART_FUNCSEL_NUM(uart0, GPIO16));
    gpio_set_function(GPIO17, UART_FUNCSEL_NUM(uart0, GPIO17));
    uart_puts(uart0, "initing pico !\n");

    multicore_launch_core1(main_core1);

    #ifdef USE_KEYBOARD
    keyboard_matrix_init(&keyboard_matrix);
    #endif

    #ifdef USE_KNOB
    rotary_encoder_init(&volume_knob, true);
    #endif

    while (true) {
        #ifdef USE_KEYBOARD
        keyboard_task();
        #endif

        #ifdef USE_KNOB
        rotary_encoder_task(&volume_knob);
        #endif
    }

    return 0;
}
