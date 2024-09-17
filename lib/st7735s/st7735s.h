#ifndef ST7735S_H
#define ST7735S_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

typedef struct {
    const uint8_t pin_SCL;
    const uint8_t pin_SDA;
    const uint8_t pin_RES;
    const uint8_t pin_DC;
    const uint8_t pin_CS;
    const uint8_t pin_BLK;
    spi_inst_t *spi_inst;
    volatile bool is_on;
} st7735s_t;

void st7735s_init(st7735s_t *const self, uint baudrate);

void st7735s_command_mode(const st7735s_t *const self);

void st7735s_data_mode(const st7735s_t *const self);

void st7735s_write_command_with_data(const st7735s_t *const self, const uint8_t *const buffer, const size_t size);

void st7735s_write_command(const st7735s_t *const self, const uint8_t command);

void st7735s_write_data(const st7735s_t *const self, const uint8_t *const data, const size_t size);

void st7735s_write(const st7735s_t *const self, const uint8_t *const bytes, const size_t size, const bool is_command);

void st7735s_hard_reset(const st7735s_t *const self);

#endif
