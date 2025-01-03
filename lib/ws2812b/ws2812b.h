/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/WS2812.pdf
 */

#ifndef WS2812B_H
#define WS2812B_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../graphics/color.h"

struct ws2812b {
    spi_inst_t *spi_inst;
    uint8_t pin_Din;
    struct grb *leds_buffer;
    uint16_t num_leds;
    uint8_t *spi_buffer;
};

void ws2812b_init(struct ws2812b *const self);

void ws2812b_deinit(struct ws2812b *const self);

void ws2812b_set_all(struct ws2812b *const self, struct grb color);

void ws2812b_set_one(struct ws2812b *const self, uint16_t led, struct grb color);

void ws2812b_set_off(struct ws2812b *const self);

void ws2812b_set_brightness(struct ws2812b *const self, const float factor);

bool ws2812b_write(struct ws2812b *const self);

static void ws2812b_update_spi_buffer(struct ws2812b *const self);

#endif
