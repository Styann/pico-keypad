/**
 * @author Styann
 */

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "pico_extra.h"

#include "usb/usb.h"
#include "usb/usb_hid.h"

#include "keyboard_matrix/keyboard_matrix.h"
#include "hw040/hw040.h"
#include "ws2812b/ws2812b.h"
#include "fightstick/joystick/joystick.h"
#include "fightstick/button/button.h"
#include "ssd1331/ssd1331.h"
#include "macro/macro.h"

#include <stdio.h>

#define USE_KEYBOARD
// #define USE_HW40
// #define USE_FIGHTSTICK
#define USE_WS2812B

// #include "cow.h"
// #define USE_SSD1331

#define DEBOUNCE_MS 10

#ifdef USE_HW40
    void volume_knob_cw_callback(void) {
        usb_send_consumer_control(KC_MEDIA_VOLUME_INCREMENT);
    }

    void volume_knob_ccw_callback(void) {
        usb_send_consumer_control(KC_MEDIA_VOLUME_DECREMENT);
    }

    struct hw040 volume_knob = {
        .pin_SW = GPIO28,
        .pin_DT = GPIO27,
        .pin_CLK = GPIO26,
        .state_CLK = LOW,
        .last_state_CLK = LOW,
        .cw_callback = &volume_knob_cw_callback,
        .ccw_callback = &volume_knob_ccw_callback
    };

    void gpio_core0_irq_callback(uint gpio, uint32_t events) {
        if ((gpio == volume_knob.pin_SW) && (events & GPIO_IRQ_EDGE_FALL)) {
            static uint32_t timer = 0;

            if (millis() - timer > DEBOUNCE_MS) {
                usb_send_consumer_control(KC_MEDIA_PLAY_PAUSE);
                timer = millis();
            }
        }
    }
#endif

#ifdef USE_KEYBOARD
    #define LAYOUT_COLUMN_SIZE 4
    #define LAYOUT_ROW_SIZE 4

    static const uint8_t columns_pins[LAYOUT_COLUMN_SIZE] = { GPIO0, GPIO1, GPIO2, GPIO3 };
    static const uint8_t rows_pins[LAYOUT_ROW_SIZE] = { GPIO4, GPIO5, GPIO6, GPIO7 };

    // static const uint8_t layout[LAYOUT_ROW_SIZE][LAYOUT_COLUMN_SIZE] = {
    //     { KC_NUM_LOCK, KC_KEYPAD_DIVIDE, KC_KEYPAD_MULTIPLY, KC_KEYPAD_SUBTRACT },
    //     { KC_KEYPAD_7, KC_KEYPAD_8,      KC_KEYPAD_9,        KC_KEYPAD_ADD      },
    //     { KC_KEYPAD_4, KC_KEYPAD_5,      KC_KEYPAD_6,        KC_KEYPAD_ENTER    },
    //     { KC_KEYPAD_1, KC_KEYPAD_2,      KC_KEYPAD_3,        KC_KEYPAD_PERIOD   },
    // };

    static const uint8_t layout[LAYOUT_ROW_SIZE][LAYOUT_COLUMN_SIZE] = {
        { KC_CTRL_LEFT, KC_GUI_LEFT, KC_ARROW_LEFT, KC_ARROW_RIGHT },
        { KC_KEYPAD_7,  KC_KEYPAD_8,    KC_KEYPAD_9,        KC_KEYPAD_ADD      },
        { KC_KEYPAD_4,  KC_KEYPAD_5,    KC_KEYPAD_6,        KC_KEYPAD_ENTER    },
        { KC_KEYPAD_1,  KC_KEYPAD_2,    KC_KEYPAD_3,        KC_KEYPAD_PERIOD   },
    };

    keyboard_matrix_t keyboard_matrix = {
        .layout = &layout[0][0],
        .rows_pins = rows_pins,
        .columns_pins = columns_pins,
        .row_size = LAYOUT_ROW_SIZE,
        .column_size = LAYOUT_COLUMN_SIZE
    };

    void keyboard_task(void) {
        static uint32_t timer = 0;
        static bool can_release = false;
        static struct usb_keyboard_report previous_report = {};

        if (millis() - timer > DEBOUNCE_MS) {
            struct usb_keyboard_report keyboard_report = { 0x01, 0, 0, { 0, 0, 0, 0, 0, 0 } };
            keyboard_matrix_scan(&keyboard_matrix, &keyboard_report);

            const macro_t macro_home = {
                { 0x01, KC_MOD_CTRL_LEFT | KC_MOD_GUI_LEFT, 0, { KC_ARROW_LEFT, 0, 0, 0, 0, 0 }},
                { 0x01, 0, 0, { KC_HOME, 0, 0, 0, 0, 0 } }
            };

            const macro_t macro_end = {
                { 0x01, KC_MOD_CTRL_LEFT | KC_MOD_GUI_LEFT, 0, { KC_ARROW_RIGHT, 0, 0, 0, 0, 0 } },
                { 0x01, 0, 0, { KC_END, 0, 0, 0, 0, 0 } }
            };

            macro_parse_keyboard(&keyboard_report, &macro_home);
            macro_parse_keyboard(&keyboard_report, &macro_end);

            // if there is a change between actual and previous report
            if (!keyboard_report_cmp(&keyboard_report, &previous_report)) {
                if (!is_keyboard_report_empty(&keyboard_report)) {
                    usb_send_keyboard_report(&keyboard_report);
                    can_release = true;
                }
                else {
                    if (can_release) {
                        release_keyboard();
                        can_release = false;
                    }
                }

                previous_report = keyboard_report;
            }

            timer = millis();
        }
    }
#endif

#ifdef USE_FIGHTSTICK
    struct joystick stick = {
        .up_pin = GPIO2,
        .right_pin = GPIO3,
        .down_pin = GPIO4,
        .left_pin = GPIO5,
    };

    const uint8_t a = GPIO6;
    const uint8_t b = GPIO7;
    const uint8_t x = GPIO8;
    const uint8_t y = GPIO9;
    const uint8_t lb = GPIO10;
    const uint8_t lt = GPIO11;
    const uint8_t rb = GPIO12;
    const uint8_t rt = GPIO13;

    void fightstick_task(void) {
        static uint32_t timer = 0;
        static bool can_release = false;

        static struct usb_gamepad_report previous_report = {};

        if (millis() - timer > DEBOUNCE_MS) {
            struct usb_gamepad_report gamepad_report = {};

            gamepad_report.x = joystick_read_x_axis(&stick);
            gamepad_report.y = joystick_read_y_axis(&stick);
            gamepad_report.buttons |= button_read(a);
            gamepad_report.buttons |= button_read(b) << 1;
            gamepad_report.buttons |= button_read(x) << 2;
            gamepad_report.buttons |= button_read(y) << 3;
            gamepad_report.buttons |= button_read(lb) << 4;
            gamepad_report.buttons |= button_read(lt) << 5;
            gamepad_report.buttons |= button_read(rb) << 6;
            gamepad_report.buttons |= button_read(rt) << 7;

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

            timer = millis();
        }
    }
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
        struct ssd1331 display = {
            .pin_DC = GPIO20,
            .pin_SDA = GPIO19,
            .pin_SCL = GPIO18,
            .pin_CS = GPIO17,
            .pin_RES = GPIO16,
            .spi_inst = spi0
        };

        // built_in_led_init();
        ssd1331_init(&display);

        while(true) {
            for (uint8_t i = 0; i < 39; i++) {
                ssd1331_write_data(&display, (uint16_t*)cow[i], SSD1331_RESOLUTION);
                sleep_ms(80);
            }
            // asm volatile("nop");
        }
    #endif
}

void set_report_callback(uint8_t const *buf, uint16_t len) {
    if (len == 2) {
        printf("-> %d %d\n", buf[0], buf[1]);

        if (*buf == 0x67) {
            static bool state = HIGH;
            // built_in_led_put(state);
            state = !state;
        }
    }
}

int main(void) {
    stdio_init_all();

    multicore_launch_core1(main_core1);

    usb_device_init();

    #ifdef USE_KEYBOARD
        keyboard_matrix_init(&keyboard_matrix);
    #endif

    #ifdef USE_HW40
        hw040_init(&volume_knob, true);
        gpio_set_irq_callback(&gpio_core0_irq_callback);
        irq_set_enabled(IO_IRQ_BANK0, true);
    #endif

    #ifdef USE_FIGHTSTICK
        joystick_init(&stick);
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

        #ifdef USE_HW40
            hw040_task(&volume_knob);
        #endif

        #ifdef USE_FIGHTSTICK
            fightstick_task();
        #endif
    }

    return 0;
}
