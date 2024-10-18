/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/WS2812.pdf
 */

#ifndef WS2812B_H
#define WS2812B_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../graphics/color.h"

// T -> transfer time
// L -> LOW, H -> HIGH
// RES -> RESET SIGNAL
// bit 0 = T0_HIGH + T0_LOW
// T0H 0.4us
// T0L 0.85us
// T1H 0.8us
// T1L 0.45us
#define T0 0b10000000
#define T1 0b11111110
#define RES 60 // us low voltage time, datasheets say Above 50us

#define WS2812B_BAUD_RATE 2500000

struct ws2812b {
    spi_inst_t *spi_inst;
    uint8_t pin_Din;
    struct grb *leds_buffer;
    uint16_t num_leds;
    uint8_t *spi_buffer;
};

void ws2812b_init(struct ws2812b *self);

void ws2812b_set_all(struct ws2812b *self, struct grb color);

void ws2812b_set_one(struct ws2812b *self, uint16_t led, struct grb color);

void ws2812b_set_off(struct ws2812b *self);

void ws2812b_set_brightness(struct ws2812b *self, float factor);

void ws2812b_breathing(struct ws2812b *self, struct grb color);

bool ws2812b_write(struct ws2812b *self);

static void ws2812b_update_spi_buffer(struct ws2812b *self);

#endif
