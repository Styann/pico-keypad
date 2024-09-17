#include "ssd1306.h"
#include "../pico_extra/pico_extra.h"

void ssd1306_init(ssd1306_128x32_t *self, uint baudrate) {
    i2c_init(self->i2c_inst, baudrate * 1000);

    gpio_init(self->pin_SCK);
    gpio_set_function(self->pin_SCK, GPIO_FUNC_I2C);
    gpio_pull_up(self->pin_SCK);

    gpio_init(self->pin_SDA);
    gpio_set_function(self->pin_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(self->pin_SDA);

    const uint8_t commands[26] = {
        SSD1306_SET_DISPLAY_OFF,
        SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO, 0x80,
        SSD1306_SET_MULTIPLEX_RATIO, 32 - 1,
        SSD1306_SET_DISPLAY_OFFSET, 0x00,
        SSD1306_SET_START_LINE | 0x00,
        SSD1306_CHARGE_PUMP_SETTING,
        SSD1306_ENABLE_CHARGE_PUMP,
        SSD1306_MEMORY_ADDRESSING_MODE, 0x00,
        SSD1306_SET_SEGMENT_REMAP | 0x01,
        SSD1306_SET_COM_SCAN_DIRECTION,
        SSD1306_SET_COM_PINS, 0x02,
        SSD1306_SET_CONTRAST_CONTROL_BANK_0, 0x8F,
        SSD1306_SET_PRECHARGE_PERIOD, 0xF1,
        SSD1306_SET_Vcomh_DESELECT_LEVEL, 0x40,
        SSD1306_ENTIRE_DISPLAY_ON,
        SSD1306_SET_NORMAL_DISPLAY,
        SSD1306_DEACTIVATE_SCROLL,
        SSD1306_SET_DISPLAY_ON
    };

    ssd1306_write_commands(self, commands, 26);

    self->is_on = true;
}

void ssd1306_turn_on(const ssd1306_128x32_t *self) {
    // ssd1306_write_byte(self, SSD1306_ENTIRE_DISPLAY_ON);
}

void ssd1306_write_command(const ssd1306_128x32_t *self, const uint8_t command) {
    const uint8_t buffer[2] = { 0x80, command };
    i2c_write_blocking(self->i2c_inst, self->addr, buffer, 2, false);
}

void ssd1306_write_commands(const ssd1306_128x32_t *self, const uint8_t *commands, const size_t size) {
    for (size_t i = 0; i < size; i++) {
        built_in_led_on();
        ssd1306_write_command(self, commands[i]);
    }
}
