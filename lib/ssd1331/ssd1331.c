/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/SSD1331_1.2.pdf
 */

#include "ssd1331.h"
#include "font.h"

#include <ctype.h> // for toupper()

void ssd1331_init(struct ssd1331 *this) {
    spi_init(this->spi_inst, SSD1331_BAUD_RATE);

    gpio_init(this->pin_SCL);
    gpio_init(this->pin_SDA);
    gpio_set_function(this->pin_SCL, GPIO_FUNC_SPI);
    gpio_set_function(this->pin_SDA, GPIO_FUNC_SPI);

    // init and set chip select to high
    gpio_init(this->pin_CS);
    gpio_set_function(this->pin_CS, GPIO_FUNC_SPI);
    gpio_set_dir(this->pin_CS, GPIO_OUT);
    spi_chip_deselect(this->pin_CS);

    gpio_init(this->pin_DC);
    gpio_set_dir(this->pin_DC, GPIO_OUT);
    ssd1331_data_mode(this);

    gpio_init(this->pin_RES);
    gpio_set_dir(this->pin_RES, GPIO_OUT);

    ssd1331_reset(this);

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

    ssd1331_write_commands(this, commands, sizeof(commands));
}

void ssd1331_reset(struct ssd1331 *this) {
    gpio_put(this->pin_RES, LOW);
    sleep_ms(50);
    gpio_put(this->pin_RES, HIGH);
    sleep_ms(50);
}

void ssd1331_turn_off(struct ssd1331 *this) {
    const uint8_t set_display_off = SET_DISPLAY_OFF;
    ssd1331_write_command(this, set_display_off);
}

/**
 * @brief enter to command mode by set DC to low state
 */
static void ssd1331_command_mode(struct ssd1331 *this) {
    gpio_put(this->pin_DC, LOW);
}

/**
 * @brief enter to data mode by set DC to high state
 */
static void ssd1331_data_mode(struct ssd1331 *this) {
    gpio_put(this->pin_DC, HIGH);

}

/**
 * @param command
 */
void ssd1331_write_command(struct ssd1331 *this, const uint8_t command) {
    ssd1331_write_commands(this, &command, 1);
}

/**
 * @param commands
 * @param size
 */
void ssd1331_write_commands(struct ssd1331 *this, const uint8_t *commands, size_t size) {
    ssd1331_command_mode(this);
    spi_set_format_8(this->spi_inst);

    if (!spi_is_busy(this->spi_inst) && spi_is_writable(this->spi_inst)) {
        spi_chip_select(this->pin_CS);
        spi_write_blocking(this->spi_inst, commands, size);
        spi_chip_deselect(this->pin_CS);
    }
}

/**
 * @param data
 * @param size
 */
void ssd1331_write_data(struct ssd1331 *this, uint16_t *data, size_t size) {
    ssd1331_data_mode(this);
    spi_set_format_16(this->spi_inst);

    if (!spi_is_busy(this->spi_inst) && spi_is_writable(this->spi_inst)) {
        spi_chip_select(this->pin_CS);
        spi_write16_blocking(this->spi_inst, data, size);
        spi_chip_deselect(this->pin_CS);
    }
}

void ssd1331_draw_pixel(struct ssd1331 *this, uint8_t x, uint8_t y, uint16_t color) {
    const uint8_t commands[6] = {
        SET_COLUMN_ADDRESS, x, x, SET_ROW_ADDRESS, y, y
    };

    ssd1331_write_commands(this, commands, 6);
    ssd1331_write_data(this, &color, 1);
}

void ssd1331_fill_screen(struct ssd1331 *this, uint16_t color) {
    for (uint8_t x = 0; x < SSD1331_WIDTH; x++) {
        for (uint8_t y = 0; y < SSD1331_HEIGHT; y++) {
            ssd1331_draw_pixel(this, x, y, color);
        }
    }
}

void ssd1331_print_char(struct ssd1331 *this, uint8_t x, uint8_t y, const char c) {
    const uint64_t font_char = font[toupper(c) - 48];

    for (uint8_t offset_y = 0; offset_y < 8; offset_y++) {

        // loop through all bytes of font_char
        const uint8_t byte = (font_char >> (56 - offset_y * 8)) & 0xFF;

        for (uint8_t offset_x = 0; offset_x < 8; offset_x++) {

            // loop through all bit of byte to get font or back color
            const uint16_t pixel_color = ((byte >> (7 - offset_x)) & 0b1) ? 0xFFFF : 0x0000;
            ssd1331_draw_pixel(this, x + offset_x, y + offset_y, pixel_color);
        }
    }
}

void ssd1331_print(struct ssd1331 *this, const char *c) {
    while (*c != '\0') {
        if (this->coordinate.x == SSD1331_WIDTH) {
            this->coordinate.x = 0;
            this->coordinate.y += 10;
        }

        ssd1331_print_char(this, this->coordinate.x, this->coordinate.y, *c);

        this->coordinate.x += 8;
        c++;
    }
}

void ssd1331_println(struct ssd1331 *this, const char *c) {
    ssd1331_print(this, c);
    this->coordinate.x = 0;
    this->coordinate.y += 10;
}
