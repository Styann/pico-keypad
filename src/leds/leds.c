#include "leds.h"
#include "hardware/pwm.h"

void pwm_pin_init(struct pwm_pin *pwm_pin) {

    gpio_set_function(pwm_pin->pin, GPIO_FUNC_PWM);
    pwm_pin->slice = pwm_gpio_to_slice_num(pwm_pin->pin);
    
    // set max acceptable value to 255
    pwm_set_wrap(pwm_pin->slice, 0xFF);

    pwm_set_chan_level(pwm_pin->slice, pwm_pin->channel, 0);
    pwm_set_enabled(pwm_pin->slice, true);

    return;
}

void leds_init(struct leds *leds) {
    pwm_pin_init(&leds->r);
    pwm_pin_init(&leds->g);
    pwm_pin_init(&leds->b);
    return;
}

void leds_put_color(struct leds *leds, rgb_t color) {
    uint8_t r = RGB_R(color);
    uint8_t g = RGB_G(color);
    uint8_t b = RGB_B(color);

    pwm_set_chan_level(leds->r.slice, leds->r.channel, r);
    pwm_set_chan_level(leds->r.slice, leds->r.channel, g);
    pwm_set_chan_level(leds->r.slice, leds->r.channel, b);

    return;
}

void leds_set_brightness(struct leds *leds, uint8_t brightness) {
    return;
}

void leds_off(struct leds *leds) {
    leds_put_color(leds, LED_OFF);
    return;
}

uint8_t byte_to_analog(uint8_t byte) {
    return (uint8_t)((byte / 255) * 100);
}