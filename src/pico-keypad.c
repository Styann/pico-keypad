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

#define USE_HW40
#define USE_WS2812B
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

#ifdef USE_GAMEPAD
struct usb_gamepad_report report = { 0x03, 127, 127, 0b11111111 };
usb_send_gamepad_report(&report);
#endif

void keyboard_task(void) {
    static uint32_t timer = 0;
    static bool can_release = false;
    static struct usb_keyboard_report previous_report = {};

    if (millis() - timer > DEBOUNCE_MS) {
        struct usb_keyboard_report keyboard_report = { 0x01, 0, 0, { 0, 0, 0, 0, 0, 0 } };
        keyboard_matrix_scan(&keyboard_report);

        if (!has_keyboard_report_changed(&keyboard_report, &previous_report)) {
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
}

int main(void) {
    stdio_init_all();

    multicore_launch_core1(main_core1);

    gpio_init(GPIO25);
    gpio_set_dir(GPIO25, GPIO_OUT);

    usb_device_init();

    keyboard_matrix_init();

    #ifdef USE_HW40
    hw040_init(&volume_knob);
    gpio_set_irq_callback(&gpio_core0_irq_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
    #endif

    while (!is_configured()) {
        tight_loop_contents();
    }

    while (true) {
        keyboard_task();

        #ifdef USE_HW40
        hw040_task(&volume_knob);
        #endif
    }

    return 0;
}
