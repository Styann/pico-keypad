#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "pico_extra.h"

#include "usb/usb.h"
#include "keys/keys.h"
//#include "rgb_led_strip/rgb_led_strip.h"
#include "hw040/hw040.h"
#include "ws2812b/ws2812b.h"

void keyboard_task(void);

void volume_knob_cw_callback(void) {
    send_consumer_control(KC_MEDIA_VOLUME_INCREMENT);
    release();
}

void volume_knob_ccw_callback(void) {
    send_consumer_control(KC_MEDIA_VOLUME_DECREMENT);
    release();
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

struct ws2812b led_strip = {
    .num_leds = 30,
    .spi_inst = spi0,
    .spi_mosi_pin = GPIO19,
    .buffer = NULL
};

void gpio_core0_irq_callback(uint gpio, uint32_t events) {
    if ((gpio == volume_knob.pin_SW) && (events & GPIO_IRQ_EDGE_RISE)) {
        //struct usb_keyboard_report report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, KC_MEDIA_PLAY_PAUSE };
        //usb_send_keyboard_report(&report);
        static uint32_t timer = 0;
        const uint32_t interval_ms = 10;

        if (millis() - timer > interval_ms) {
            send_consumer_control(KC_MEDIA_PLAY_PAUSE);
            timer = millis();
        }
    }
}

void keyboard_task(void) {
    static uint32_t timer = 0;
    const uint32_t keys_debounce_ms = 10;

    if (millis() - timer > keys_debounce_ms) {
        struct usb_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, 0 };
        scan_keyboard(&keyboard_report);
        usb_send_keyboard_report(&keyboard_report);
        timer = millis();
    }
}

void main_core1(void) {

}

int main(void) {
    stdio_init_all();

    // multicore_launch_core1(main_core1);

    gpio_init(GPIO25);
    gpio_set_dir(GPIO25, GPIO_OUT);

    usb_device_init();

    keys_init();
    hw040_init(&volume_knob);

    //ws2812b_test_init();
    //ws2812b_test();
    //ws2812b_init(&led_strip);
    //grb_t color = { 0, 0xFF, 0 };
    //ws2812b_set_all(&led_strip, &color);
    //ws2812b_write(&led_strip);

    // set irq
    gpio_set_irq_callback(&gpio_core0_irq_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);

    while (!is_configured()) {
        tight_loop_contents();
    }

    while (true) {
        //keyboard_task();
        hw040_task(&volume_knob);
        tight_loop_contents();
    }

    return 0;
}
