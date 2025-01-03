/**
 * @link https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */

#include "ssd1306.h"
#include "../pico_extra/pico_extra.h"
#include <string.h>
#include "font.h"

#include <stdio.h>
#include <stdlib.h>
#include "hardware/uart.h"

#define SSD1306_I2C_ADDR 0x3C
#define BUFFER_SIZE 512
#define PAGE_SIZE 128

enum ssd1306_ids { DATA = 0x40, COMMAND = 0x80 };

enum ssd1306_addressing_modes {
    HORIZONTAL_ADDRESSING_MODE = 0x00,
    VERTICAL_ADDRESSING_MODE = 0x01,
    PAGE_ADDRESSING_MODE = 0x02
};

enum ssd1306_pages {
    PAGE0,
    PAGE1,
    PAGE2,
    PAGE3,
    // PAGE4,
    // PAGE5,
    // PAGE6,
    // PAGE7
};

enum ssd1306_commands {
    MEMORY_ADDRESSING_MODE         = 0x20,
    SET_COLUMN_ADDRESS             = 0x21,
    SET_PAGE_ADDRESS               = 0x22,
    SET_START_LINE                 = 0x40,
    SET_CONTRAST_CONTROL_BANK_0    = 0x81,
    SET_SEGMENT_REMAP              = 0xA0,
    RESUME_ENTIRE_DISPLAY_ON       = 0xA4,
    FORCE_ENTIRE_DISPLAY_ON        = 0xA5,
    SET_NORMAL_DISPLAY             = 0xA6,
    SET_INVERSE_DISPLAY            = 0xA7,
    SET_MULTIPLEX_RATIO            = 0xA8,
    SET_DISPLAY_OFF				   = 0xAE, // sleep mode
    SET_DISPLAY_ON  			   = 0xAF,

    CHARGE_PUMP_SETTING            = 0x8D,
    ENABLE_CHARGE_PUMP             = 0x14,
    SET_COM_PINS                   = 0xDA,

    SET_COM_SCAN_DIRECTION         = 0xC0,
    SET_DISPLAY_OFFSET             = 0xD3,
    SET_DISPLAY_CLOCK_DIVIDE_RATIO = 0xD5,
    SET_PRECHARGE_PERIOD           = 0xD9,
    SET_COM_PINS_HARDWARE_CONFIG   = 0xDA,
    SET_Vcomh_DESELECT_LEVEL       = 0xDB,
    NOP                            = 0xE3,
    RIGHT_HORIZONTAL_SCROLL_SETUP  = 0x26,
    LEFT_HORIZONTAL_SCROLL_SETUP   = 0x27,
    // CONTINUOUS_VERTICAL_AND_HORIZONTAL_SCROLL_SETUP = 0x
    DEACTIVATE_SCROLL              = 0x2E,
    ACTIVATE_SCROLL                = 0x2F,
    SET_VERTICAL_SCROLL_AREA       = 0xA3
};

enum ssd1306_page_mode_commands {
    SET_START_PAGE_ADDRESS          = 0xB0, // | page (0b000, 0b111)
    SET_LOWER_COLUMN_START_ADDRESS  = 0x00, // | nibble (0b0000, 0b1111)
    SET_HIGHER_COLUMN_START_ADDRESS = 0x10, // | nibble (0b0000, 0b1111)
};

void ssd1306_init(ssd1306_t *const self, const uint baudrate) {
    i2c_init(self->i2c_inst, baudrate);

    gpio_init(self->pin_SCK);
    gpio_set_function(self->pin_SCK, GPIO_FUNC_I2C);
    gpio_pull_up(self->pin_SCK);

    gpio_init(self->pin_SDA);
    gpio_set_function(self->pin_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(self->pin_SDA);

    self->page = PAGE3;
    self->column = 0;

    self->is_on = true;

    const uint8_t commands[] = {
        SET_DISPLAY_OFF,
        SET_DISPLAY_CLOCK_DIVIDE_RATIO, 0x80,
        SET_MULTIPLEX_RATIO, 32 - 1,
        SET_DISPLAY_OFFSET, 0x00,
        SET_START_LINE | 0x00,
        CHARGE_PUMP_SETTING, ENABLE_CHARGE_PUMP,
        SET_SEGMENT_REMAP | 0x01,
        SET_COM_SCAN_DIRECTION,
        SET_COM_PINS, 0x02,
        SET_CONTRAST_CONTROL_BANK_0, 0x8F,
        SET_PRECHARGE_PERIOD, 0xF1,
        SET_Vcomh_DESELECT_LEVEL, 0x40,
        SET_NORMAL_DISPLAY,
        DEACTIVATE_SCROLL,
        SET_DISPLAY_ON
    };

    ssd1306_write_commands(self, commands, sizeof(commands));

    ssd1306_clear(self);
}

void ssd1306_deinit(ssd1306_t *const self) {
    // TODO: disbable charge pump
    ssd1306_write_command(self, SET_DISPLAY_OFF);

    self->is_on = false;

    gpio_disable_pulls(self->pin_SCK);
    gpio_set_function(self->pin_SCK, GPIO_FUNC_NULL);
    gpio_deinit(self->pin_SCK);

    gpio_disable_pulls(self->pin_SDA);
    gpio_set_function(self->pin_SDA, GPIO_FUNC_NULL);
    gpio_deinit(self->pin_SDA);

    i2c_deinit(self->i2c_inst);
}

void ssd1306_write(const ssd1306_t *const self, const uint8_t *const buffer, const uint len) {
    if (!self->is_on) return;
    i2c_write_blocking(self->i2c_inst, SSD1306_I2C_ADDR, buffer, len, false);
}

void ssd1306_write_command(const ssd1306_t *const self, const uint8_t command) {
    const uint8_t buffer[2] = { COMMAND, command };
    ssd1306_write(self, buffer, 2);
}

void ssd1306_write_commands(const ssd1306_t *const self, const uint8_t *const commands, const uint len) {
    for (uint i = 0; i < len; i++) {
        ssd1306_write_command(self, commands[i]);
    }
}

/**
 * @brief used to write pixels
 * @param buffer
 */
void ssd1306_write_all(const ssd1306_t *const self, const uint8_t *const buffer) {
    const uint8_t commands[] = {
        MEMORY_ADDRESSING_MODE, HORIZONTAL_ADDRESSING_MODE,
        SET_PAGE_ADDRESS, 0, 3,
        SET_COLUMN_ADDRESS, 0, 127
    };

    uint8_t final_buffer[1 + BUFFER_SIZE];
    final_buffer[0] = DATA;
    memcpy(final_buffer + 1, buffer, BUFFER_SIZE);

    ssd1306_write_commands(self, commands, sizeof(commands));
    ssd1306_write(self, final_buffer, sizeof(final_buffer));
}

void ssd1306_clear(ssd1306_t *const self) {
    uint8_t buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    ssd1306_write_all(self, buffer);

    self->column = 0;
    self->page = PAGE3;
}

void push_col_in_page(uint8_t *const col, const uint8_t value) {
    if (value != 0x00) {
        *col = value;
    }
}

/**
 * @brief fill buffer from string
 * @param length - string length
 * @returns buffer length
 */
uint ssd1306_fill_buffer(const ssd1306_t *const self, uint8_t *const buffer, const char *const string, uint *const nb_chars) {
    buffer[0] = DATA;
    uint buf_i = 1;

    uint length = MIN(strlen(string), PAGE_SIZE);

    for (uint i = 0; i < length; i++) {
        const uint64_t *ch = &font[string[i]];
        uint8_t start = 0;
        uint8_t end = 7;

        if (string[i] != ' ') {
            for (uint j = 0; j < 7; j++) {
                const uint8_t col = (uint8_t)(*ch >> (7 - j) * 8);
                start = j;
                if (col != 0x00) break;
            }

            for (uint j = 7; j >= start; j--) {
                const uint8_t col = (uint8_t)(*ch >> (7 - j) * 8);
                if (col != 0x00) break;
                end = j;
            }
        }

        if (((end - start + 1) + 2) > ssd1306_remaining_columns(self)) {
            break;
        }

        buffer[buf_i] = 0x00;
        buffer[buf_i + 1] = 0x00;
        buf_i += 2;

        for (uint j = start; j < end; j++) {
            const uint offset = (7 - j) * 8;
            buffer[buf_i] = (uint8_t)(*ch >> offset);
            buf_i++;
        }

        (*nb_chars)++;
    }

    return buf_i;
}

void ssd1306_set_addr(ssd1306_t *const self, uint8_t page, uint8_t column) {
    self->page = page;
    self->column = column;

    const uint8_t commands[] = {
        MEMORY_ADDRESSING_MODE, PAGE_ADDRESSING_MODE,
        SET_START_PAGE_ADDRESS | self->page,
        SET_LOWER_COLUMN_START_ADDRESS | (self->column & 0x0F),
        SET_HIGHER_COLUMN_START_ADDRESS | (self->column & 0xF0)
    };

    ssd1306_write_commands(self, commands, sizeof(commands));
}

static void ssd1306_newline(ssd1306_t *const self) {
    if (self->page == PAGE0) {
        self->page = PAGE3;
    }
    else {
        self->page--;
    }

    self->column = 0;

    ssd1306_set_addr(self, self->page, self->column);
}

static int ssd1306_remaining_columns(const ssd1306_t *const self) {
    return (PAGE_SIZE - self->column);
}

void ssd1306_print(ssd1306_t *const self, const char string[]) {
    uint8_t buffer[1 + PAGE_SIZE];

    // copy the string to avoid string literal error
    char *const string_cpy = strdup(string);
    bool contain_newline = (strchr(string_cpy, '\n') != NULL);
    char *chunk = strtok(string_cpy, "\n");

    while (chunk != NULL) {
        memset(buffer, 0, sizeof(buffer));

        uint nb_chars = 0;
        const uint pkt_len = ssd1306_fill_buffer(self, buffer, chunk, &nb_chars);

        uint xferred_sum = 0;

        const uint8_t commands[] = {
            MEMORY_ADDRESSING_MODE, PAGE_ADDRESSING_MODE,
            SET_START_PAGE_ADDRESS | self->page,
            // SET_LOWER_COLUMN_START_ADDRESS | (self->column & 0x0F),
            // SET_HIGHER_COLUMN_START_ADDRESS | (self->column & 0xF0)
        };

        ssd1306_write_commands(self, commands, sizeof(commands));
        ssd1306_write(self, buffer, MIN(ssd1306_remaining_columns(self), pkt_len));

        self->column += pkt_len;

        if (!contain_newline) break;

        ssd1306_newline(self);

        chunk = strtok(NULL, "\n");
    }

    free(string_cpy);
}

void ssd1306_println(ssd1306_t *const self, const char string[]) {
    ssd1306_print(self, string);
    ssd1306_newline(self);
}
