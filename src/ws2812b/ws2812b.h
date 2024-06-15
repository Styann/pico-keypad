/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/WS2812.pdf
 */

#ifndef WS2812B_H
#define WS2812B_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

#define GRB_GREEN   0x00FF0000u
#define GRB_RED     0x0000FF00u
#define GRB_BLUE    0x000000FFu
#define GRB_CYAN    0x00FF00FFu
#define GRB_YELLOW  0x00FFFF00u
#define GRB_MAGENTA 0x0000FFFFu
#define GRB_ORANGE  0x0017FF00u
#define GRB_WHITE   0x00FFFFFFu
#define GRB_OFF     0x00000000u

#define GRB_G_MASK 0x00FF0000u
#define GRB_R_MASK 0x0000FF00u
#define GRB_B_MASK 0x000000FFu

typedef uint32_t grb32_t;

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
#define GRB_BIT_SIZE 24

#define WS2812B_BAUD_RATE 2500000

struct ws2812b {
    spi_inst_t *spi_inst;
    uint8_t pin_mosi;

    uint32_t *leds_buffer;
    uint16_t num_leds;
    uint8_t *spi_buffer;
};

void ws2812b_init(struct ws2812b *this);

void ws2812b_set_all(struct ws2812b *this, grb32_t color);

void ws2812b_set_one(struct ws2812b *this, uint16_t led, grb32_t color);

void ws2812b_set_off(struct ws2812b *this);

void ws2812b_set_brightness(struct ws2812b *this, float factor);

bool ws2812b_write(struct ws2812b *this);

grb32_t rgb_to_grb(uint8_t r, uint8_t g, uint8_t b);

static void ws2812b_update_spi_buffer(struct ws2812b *this);

#endif
