#ifndef WS2812B_H
#define WS2812B_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <stdlib.h>
#include <string.h>

#define GRB_GREEN   0xFF0000
#define GRB_RED     0x00FF00
#define GRB_BLUE    0x0000FF
#define GRB_CYAN    0xFF00FF
#define GRB_YELLOW  0xFFFF00
#define GRB_MAGENTA 0x00FFFF
#define GRB_ORANGE  0x17FF00
#define GRB_WHITE   0xFFFFFF

typedef uint32_t grb_t;

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
    uint16_t num_leds;

    spi_inst_t *spi_inst;
    uint8_t pin_mosi;

    bool buffer_alloc_status;
    uint8_t *buffer;
    uint16_t buffer_size;
};

void ws2812b_init(struct ws2812b *this);

void ws2812b_set_all(struct ws2812b *this, grb_t color);

void ws2812b_set_one(struct ws2812b *this, uint16_t led, grb_t color);

void ws2812b_set_off(struct ws2812b *this);

void ws2812b_set_brightness(struct ws2812b *this);

bool ws2812b_write(struct ws2812b *this);

grb_t rgb_to_grb(uint8_t r, uint8_t g, uint8_t b);

#endif
