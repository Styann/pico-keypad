#ifndef WS2812B_H
#define WS2812B_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <string.h>

#define GRB_BIT_LEN 24

typedef struct {
    uint8_t g;
    uint8_t r;
    uint8_t b;
} grb_t;

// T -> transfer time
// L -> LOW, H -> HIGH
// RES -> RESET SIGNAL
// bit 0 = T0_HIGH + T0_LOW
// T0H 0.4us
// T0L 0.85us
// T1H 0.8us
// T1L 0.45us
#define RES 60 // us low voltage time, datasheets say Above 50us

#define WS2812B_BAUD_RATE 2500000

struct ws2812b {
    uint16_t num_leds;

    spi_inst_t *spi_inst;
    uint8_t spi_mosi_pin;

    uint8_t *buffer;
    uint16_t buffer_size;
    // uint8_t brightness;
};

void ws2812b_init(struct ws2812b *ws2812b);

void ws2812b_set_all(struct ws2812b *ws2812b, grb_t *color);

bool ws2812b_write(struct ws2812b *ws2812b);

uint32_t grb_to_uint32(grb_t *color);

void ws2812b_test_init(void);

void ws2812b_test(void);

#endif
