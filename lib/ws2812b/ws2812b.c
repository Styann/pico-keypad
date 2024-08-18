/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/WS2812.pdf
 */

#include "ws2812b.h"
#include "hardware/gpio.h"
#include "../pico_extra/pico_extra.h"
#include <stdlib.h>
#include <string.h>

void ws2812b_init(struct ws2812b *this) {
    ws2812b_set_off(this);

    gpio_init(this->pin_mosi);
    spi_init(this->spi_inst, WS2812B_BAUD_RATE);
    gpio_set_function(this->pin_mosi, GPIO_FUNC_SPI);
}

/**
 * @brief set one color to all leds
 * @param color
 */
void ws2812b_set_all(struct ws2812b *this, grb32_t color) {
    for (uint16_t i = 0; i < this->num_leds; i++) {
        this->leds_buffer[i] = color;
    }
}

/**
 * @brief set the color of a single led
 * @param led - led index
 * @param color
 */
void ws2812b_set_one(struct ws2812b *this, uint16_t led, grb32_t color) {
    this->leds_buffer[led] = color;
}

/**
 * @brief turn off all leds
 */
void ws2812b_set_off(struct ws2812b *this) {
    memset(this->leds_buffer, GRB_OFF, this->num_leds * sizeof(uint32_t));
}

void ws2812b_set_brightness(struct ws2812b *this, float factor) {
    for (uint16_t i = 0; i < this->num_leds; i++) {
        grb32_t *led_color = &(this->leds_buffer[i]);

        uint8_t g = ((*led_color & GRB_G_MASK) >> 16) * factor;
        uint8_t r = ((*led_color & GRB_R_MASK) >> 8) * factor;
        uint8_t b = (*led_color & GRB_B_MASK) * factor;

        *led_color = (g << 16) | (r << 8) | b;
    }
}

/**
 * @brief calculate spi buffer from leds buffer
 * @private
 */
static void ws2812b_update_spi_buffer(struct ws2812b *this) {
    for (uint16_t i = 0; i < this->num_leds; i++) {
        for (uint8_t b = 0; b < GRB_BIT_SIZE; b++) {
            // rb mean reverse bit index (23..0)
            uint8_t rb = GRB_BIT_SIZE - b - 1;

            // convert grb bit to spi logical 0 or 1
            this->spi_buffer[i * GRB_BIT_SIZE + b] = ((this->leds_buffer[i] >> rb) & 0b1) ? T1 : T0;
        }
    }
}

bool ws2812b_write(struct ws2812b *this) {
    ws2812b_update_spi_buffer(this);

    spi_inst_t *spi_inst = this->spi_inst;
    spi_set_format_8(spi_inst);

    if (!spi_is_busy(spi_inst) && spi_is_writable(spi_inst)) {
        spi_write_blocking(spi_inst, this->spi_buffer, (this->num_leds * GRB_BIT_SIZE));
        sleep_us(RES);
        return true;
    }

    return false;
}

grb32_t rgb_to_grb32(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16 | g << 8 | b);
}
