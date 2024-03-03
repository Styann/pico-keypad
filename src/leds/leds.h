#ifndef LEDS_H
#define LEDS_H

#include "pico/stdlib.h"

#define LEDS_STRIP_PIN 21
#define LEDS_STRIP_LENGTH 30

/*struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};*/

typedef uint32_t rgb_t;

#define RGB_R(rgb) (rgb & 0xFF0000) >> 16 // high
#define RGB_G(rgb) (rgb & 0x00FF00) >> 8 // mid
#define RGB_B(rgb) (rgb & 0x0000FF) // low

#define LED_OFF   0x000000
#define LED_RED   0xFF0000
#define LED_GREEN 0x00FF00
#define LED_BLUE  0x0000FF

struct pwm_pin {
    uint8_t pin;
    uint8_t slice;
    uint8_t channel;
};

struct leds {
    struct pwm_pin r;
    struct pwm_pin g;
    struct pwm_pin b;
    uint8_t length;
};

void pwm_pin_init(struct pwm_pin *pwm_pin);

void leds_init(struct leds *leds);

void leds_put_color(struct leds *leds, rgb_t color);

void leds_set_brightness(struct leds *leds, uint8_t brightness);

void leds_off(struct leds *leds);

uint8_t byte_to_analog(uint8_t byte);

#endif