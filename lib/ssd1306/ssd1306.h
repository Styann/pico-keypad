/**
 * @link https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */

#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

typedef struct {
    uint8_t addr;
    const uint8_t pin_SCK;
    const uint8_t pin_SDA;
    i2c_inst_t *i2c_inst;
    volatile bool is_on;
    uint8_t buffer[128][32];
} ssd1306_128x32_t;

void ssd1306_init(ssd1306_128x32_t *self, uint baudrate);

void ssd1306_turn_on(const ssd1306_128x32_t *self);

void ssd1306_write_command(const ssd1306_128x32_t *self, const uint8_t command);

void ssd1306_write_commands(const ssd1306_128x32_t *self, const uint8_t *commands, const size_t size);

enum ssd1306_commands {
    SSD1306_SET_DISPLAY_OFF				   = 0xAE, // sleep mode
    SSD1306_SET_DISPLAY_ON  			   = 0xAF,

    SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO = 0xD5,
    SSD1306_SET_MULTIPLEX_RATIO            = 0xA8,
    SSD1306_SET_DISPLAY_OFFSET             = 0xD3,
    SSD1306_SET_START_LINE                 = 0x40,

    SSD1306_CHARGE_PUMP_SETTING            = 0x8D,
    SSD1306_ENABLE_CHARGE_PUMP             = 0x14,

    SSD1306_MEMORY_ADDRESSING_MODE         = 0x20,
    SSD1306_SET_SEGMENT_REMAP              = 0xA0,
    SSD1306_SET_COM_SCAN_DIRECTION         = 0xC0,
    SSD1306_SET_COM_PINS                   = 0xDA,
    SSD1306_SET_CONTRAST_CONTROL_BANK_0    = 0x81,
    SSD1306_SET_PRECHARGE_PERIOD           = 0xD9,
    SSD1306_SET_Vcomh_DESELECT_LEVEL       = 0xDB,
    SSD1306_ENTIRE_DISPLAY_ON              = 0xA5,

    SSD1306_SET_NORMAL_DISPLAY             = 0xA6,
    SSD1306_SET_INVERSE_DISPLAY            = 0xA7,

    SSD1306_DEACTIVATE_SCROLL              = 0x2E,
    SSD1306_ACTIVATE_SCROLL                = 0x2F,

};

#endif
