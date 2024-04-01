#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

    #include "pico/stdlib.h"

    struct led_indicator {
        uint8_t pin;
        uint8_t mask;
    };

    #define CAPS_LOCK_PIN PICO_DEFAULT_LED_PIN

    void init_led_indicator(struct led_indicator *led_indicator);
    void toggle_led_indicator(struct led_indicator *led_indicator);

#endif