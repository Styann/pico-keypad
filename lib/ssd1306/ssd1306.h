/**
 * @link https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */

#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

typedef struct {
    const uint8_t pin_SCK;
    const uint8_t pin_SDA;
    i2c_inst_t *i2c_inst;
    volatile bool is_on;
    uint8_t page;
    uint8_t column;
} ssd1306_t;

void ssd1306_init(ssd1306_t *const self, const uint baudrate);

void ssd1306_deinit(ssd1306_t *const self);

void ssd1306_write(const ssd1306_t *const self, const uint8_t *const buffer, const uint len);

void ssd1306_write_command(const ssd1306_t *const self, const uint8_t command);

void ssd1306_write_commands(const ssd1306_t *const self, const uint8_t *const commands, const uint len);

void ssd1306_write_all(const ssd1306_t *const self, const uint8_t *const buffer);

void ssd1306_clear(ssd1306_t *const self);

void push_col_in_page(uint8_t *const col, const uint8_t value);

uint ssd1306_fill_buffer(const ssd1306_t *const self, uint8_t *const buffer, const char *const string, uint *const nb_chars);

void ssd1306_set_addr(ssd1306_t *const self, uint8_t page, uint8_t column);

static void ssd1306_newline(ssd1306_t *const self);

static int ssd1306_remaining_columns(const ssd1306_t *const self);

void ssd1306_print(ssd1306_t *const self, const char string[]);

void ssd1306_println(ssd1306_t *const self, const char string[]);

#endif
