/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/SSD1331_1.2.pdf
 */

#include "ssd1331.h"

void ssd1331_init(struct ssd1331 *this) {
    spi_init(this->spi_inst, SSD1331_BAUD_RATE);

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

    ssd1331_write_commands(this, (uint8_t*)commands, sizeof(commands));
}

void ssd1331_reset(struct ssd1331 *this) {
    gpio_put(this->pin_RES, LOW);
    sleep_ms(50);
    gpio_put(this->pin_RES, HIGH);
    sleep_ms(50);
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
void ssd1331_write_command(struct ssd1331 *this, uint8_t command) {
    ssd1331_command_mode(this);
    spi_set_format_8(this->spi_inst);

    if (!spi_is_busy(this->spi_inst) && spi_is_writable(this->spi_inst)) {
        spi_chip_select(this->pin_CS);
        spi_write_blocking(this->spi_inst, &command, sizeof(uint8_t));
        spi_chip_deselect(this->pin_CS);
    }
}

/**
 * @param commands
 * @param size
 */
void ssd1331_write_commands(struct ssd1331 *this, uint8_t *commands, size_t size) {
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
