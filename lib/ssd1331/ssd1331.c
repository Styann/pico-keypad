/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/SSD1331_1.2.pdf
 */

#include "ssd1331.h"
#include "../graphics/font.h"

#include <ctype.h> // for toupper()

void ssd1331_init(struct ssd1331 *self) {
    spi_init(self->spi_inst, SSD1331_BAUD_RATE);

    gpio_init(self->pin_SCL);
    gpio_set_function(self->pin_SCL, GPIO_FUNC_SPI);

    gpio_init(self->pin_SDA);
    gpio_set_function(self->pin_SDA, GPIO_FUNC_SPI);

    // init and set chip select to high
    gpio_init(self->pin_CS);
    gpio_set_function(self->pin_CS, GPIO_FUNC_SPI);
    gpio_set_dir(self->pin_CS, GPIO_OUT);
    spi_chip_deselect(self->pin_CS);

    gpio_init(self->pin_DC);
    gpio_set_dir(self->pin_DC, GPIO_OUT);
    ssd1331_data_mode(self);

    gpio_init(self->pin_RES);
    gpio_set_dir(self->pin_RES, GPIO_OUT);
    ssd1331_reset(self);

    const uint8_t commands[44] = {
        SET_DISPLAY_OFF,
        SET_ROW_ADDRESS, 0x00, 0x3F,
        SET_COLUMN_ADDRESS, 0x00, 0x5F,
        SET_REMAP_AND_DATA_FORMAT, 0x72,
        SET_DISPLAY_START_LINE, 0x00,
        SET_DISPLAY_OFFSET, 0x00,
        SET_DISPLAY_MODE_NORMAL,
        SET_MULTIPLEX_RATIO, 0x3F,
        SET_MASTER_CONFIGURATION, 0xBE,
        POWER_SAVE_MODE, 0x0B,
        PHASE_1_AND_2_PERIOD_ADJUSTMENT, 0x74,
        SET_DISPLAY_CLOCK_DIVIDE_RATIO, 0xF0,
        SET_SECOND_PRECHARGE_SPEED_COLOR_A, 0x64,
        SET_SECOND_PRECHARGE_SPEED_COLOR_B, 0x78,
        SET_SECOND_PRECHARGE_SPEED_COLOR_C, 0x64,
        SET_PRECHARGE_VOLTAGE, 0x3A,
        SET_Vcomh_VOLTAGE, 0x3E,
        MASTER_CURRENT_CONTROL, 0x06,
        SET_CONTRAST_COLOR_A, 0x91,
        SET_CONTRAST_COLOR_B, 0x50,
        SET_CONTRAST_COLOR_C, 0x7D,
        COLOR_ORDER_RGB,
        SET_DISPLAY_ON
    };

    ssd1331_write_commands(self, commands, sizeof(commands));

    self->is_on = true;
}

void ssd1331_reset(struct ssd1331 *self) {
    gpio_put(self->pin_RES, LOW);
    sleep_ms(50);
    gpio_put(self->pin_RES, HIGH);
    sleep_ms(50);
}

void ssd1331_turn(struct ssd1331 *self, const bool onoff) {
    if (self->is_on != onoff) {
        ssd1331_write_command(self, (onoff) ? SET_DISPLAY_ON : SET_DISPLAY_OFF);
        self->is_on = onoff;
    }
}

void ssd1331_turn_on(struct ssd1331 *self) {
    if (!(self->is_on)) {
        ssd1331_write_command(self, SET_DISPLAY_ON);
        self->is_on = true;
    }
}

void ssd1331_turn_off(struct ssd1331 *self) {
    if (self->is_on) {
        ssd1331_write_command(self, SET_DISPLAY_OFF);
        self->is_on = false;
    }
}

/**
 * @brief enter to command mode by set DC to low state
 */
static void ssd1331_command_mode(struct ssd1331 *self) {
    gpio_put(self->pin_DC, LOW);
}

/**
 * @brief enter to data mode by set DC to high state
 */
static void ssd1331_data_mode(struct ssd1331 *self) {
    gpio_put(self->pin_DC, HIGH);
}

/**
 * @param command
 */
void ssd1331_write_command(struct ssd1331 *self, const uint8_t command) {
    ssd1331_write_commands(self, &command, 1);
}

/**
 * @param commands
 * @param size
 */
void ssd1331_write_commands(struct ssd1331 *self, const uint8_t *commands, size_t size) {
    ssd1331_command_mode(self);
    spi_set_format_8(self->spi_inst);

    if (!spi_is_busy(self->spi_inst) && spi_is_writable(self->spi_inst)) {
        spi_chip_select(self->pin_CS);
        spi_write_blocking(self->spi_inst, commands, size);
        spi_chip_deselect(self->pin_CS);
    }
}

/**
 * @param data
 * @param size
 */
void ssd1331_write_data(struct ssd1331 *self, uint16_t *data, size_t size) {
    ssd1331_data_mode(self);
    spi_set_format_16(self->spi_inst);

    if (!spi_is_busy(self->spi_inst) && spi_is_writable(self->spi_inst)) {
        spi_chip_select(self->pin_CS);
        spi_write16_blocking(self->spi_inst, data, size);
        spi_chip_deselect(self->pin_CS);
    }
}

void ssd1331_draw_pixel(struct ssd1331 *self, uint8_t x, uint8_t y, uint16_t color) {
    const uint8_t commands[6] = {
        SET_COLUMN_ADDRESS, x, x, SET_ROW_ADDRESS, y, y
    };

    ssd1331_write_commands(self, commands, 6);
    ssd1331_write_data(self, &color, 1);
}

void ssd1331_fill_screen(struct ssd1331 *self, uint16_t color) {
    for (uint8_t x = 0; x < SSD1331_WIDTH; x++) {
        for (uint8_t y = 0; y < SSD1331_HEIGHT; y++) {
            ssd1331_draw_pixel(self, x, y, color);
        }
    }
}

void ssd1331_print_char(struct ssd1331 *self, uint8_t x, uint8_t y, const char c) {
    const uint64_t font_char = font[toupper(c)];

    for (uint8_t offset_y = 0; offset_y < 8; offset_y++) {

        // loop through all bytes of font_char
        const uint8_t byte = (font_char >> (56 - offset_y * 8)) & 0xFF;

        for (uint8_t offset_x = 0; offset_x < 8; offset_x++) {

            // loop through all bit of byte to get font or back color
            const uint16_t pixel_color = ((byte >> (7 - offset_x)) & 0b1) ? 0xFFFF : 0x0000;
            ssd1331_draw_pixel(self, x + offset_x, y + offset_y, pixel_color);
        }
    }
}

void ssd1331_print(struct ssd1331 *self, const char *c) {
    while (*c != '\0') {
        if (self->coordinate.x == SSD1331_WIDTH) {
            self->coordinate.x = 0;
            self->coordinate.y += 10;
        }

        ssd1331_print_char(self, self->coordinate.x, self->coordinate.y, *c);

        self->coordinate.x += 8;
        c++;
    }
}

void ssd1331_println(struct ssd1331 *self, const char *c) {
    ssd1331_print(self, c);
    self->coordinate.x = 0;
    self->coordinate.y += 10;
}
