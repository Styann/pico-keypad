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

#include <string.h>

#define USE_HW40
#define USE_FIGHTSTICK
// #define USE_WS2812B
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
    if ((gpio == volume_knob.pin_SW) && (events & GPIO_IRQ_EDGE_RISE)) {
        static uint32_t timer = 0;

        if (millis() - timer > DEBOUNCE_MS) {
            usb_send_consumer_control(KC_MEDIA_PLAY_PAUSE);
            timer = millis();
        }
    }
}
#endif

// KEYBOARD MATRIX CONFIGURATION
#define LAYOUT_COLUMN_SIZE 3
#define LAYOUT_ROW_SIZE 3

static const uint8_t columns_pins[LAYOUT_COLUMN_SIZE] = { GPIO3, GPIO4, GPIO5 };
static const uint8_t rows_pins[LAYOUT_ROW_SIZE] = { GPIO0, GPIO1, GPIO2 };

static const uint8_t layout[LAYOUT_COLUMN_SIZE][LAYOUT_ROW_SIZE] = {
    { KC_A, KC_B, KC_C },
    { KC_D, KC_E, KC_F },
    { KC_G, KC_H, KC_I }
};

#ifdef USE_KEYBOARD
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

        // if there is a change between actual and previous report
        if (memcmp(&keyboard_report, &previous_report, sizeof(struct usb_keyboard_report)) != 0) {
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
    struct ws2812b led_strip = {
        .num_leds = 30,
        .spi_inst = spi0,
        .spi_mosi_pin = GPIO19,
    };

    ws2812b_init(&led_strip);

    while (true) {
        ws2812b_set_all(&led_strip, GRB_ORANGE);
        ws2812b_write(&led_strip);
        sleep_ms(1000);

        ws2812b_set_all(&led_strip, GRB_YELLOW);
        ws2812b_write(&led_strip);
        sleep_ms(1000);

        ws2812b_set_all(&led_strip, GRB_MAGENTA);
        ws2812b_write(&led_strip);
        sleep_ms(1000);
    }
    #endif

    while (true) {

    }
}

int main(void) {
    stdio_init_all();

    // multicore_launch_core1(main_core1);

    gpio_init(GPIO25);
    gpio_set_dir(GPIO25, GPIO_OUT);

    usb_device_init();

    #ifdef USE_KEYBOARD
    keyboard_matrix_init(&keyboard_matrix);
    #endif

    #ifdef USE_HW40
    hw040_init(&volume_knob);
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
