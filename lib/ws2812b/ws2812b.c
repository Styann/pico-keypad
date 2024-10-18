/**
 * @author Styann
 * @link https://cdn-shop.adafruit.com/datasheets/WS2812.pdf
 */

#include "ws2812b.h"
#include "hardware/gpio.h"
#include "../pico_extra/pico_extra.h"
#include <stdlib.h>
#include <string.h>

void ws2812b_init(struct ws2812b *self) {
    ws2812b_set_off(self);
    gpio_init(self->pin_Din);
    spi_init(self->spi_inst, WS2812B_BAUD_RATE);
    gpio_set_function(self->pin_Din, GPIO_FUNC_SPI);
}

/**
 * @brief set one color to all leds
 * @param color
 */
void ws2812b_set_all(struct ws2812b *self, struct grb color) {
    for (uint16_t i = 0; i < self->num_leds; i++) {
        self->leds_buffer[i] = color;
    }
}

/**
 * @brief set the color of a single led
 * @param led - led index
 * @param color
 */
void ws2812b_set_one(struct ws2812b *self, uint16_t led, struct grb color) {
    self->leds_buffer[led] = color;
}

/**
 * @brief turn off all leds
 */
void ws2812b_set_off(struct ws2812b *self) {
    const struct grb off = { 0, 0, 0 };
    ws2812b_set_all(self, off);
}

void ws2812b_set_brightness(struct ws2812b *self, float factor) {
    for (uint16_t i = 0; i < self->num_leds; i++) {
        self->leds_buffer[i].g *= factor;
        self->leds_buffer[i].r *= factor;
        self->leds_buffer[i].b *= factor;
    }
}

void ws2812b_breathing(struct ws2812b *self, struct grb color) {
    struct grb color2 = color;

    while (true) {
        ws2812b_set_all(self, color2);
        ws2812b_write(self);
        sleep_ms(10);
        if (color2.g > 0) color2.g--;
        if (color2.r > 0) color2.r--;
        if (color2.b > 0) color2.b--;

        if (color2.g == 0 && color2.r == 0 && color2.b == 0) break;
    }

    while (true) {
        if (color2.g < color.g) color2.g++;
        if (color2.r < color.r) color2.r++;
        if (color2.b < color.b) color2.b++;

        ws2812b_set_all(self, color2);
        ws2812b_write(self);
        sleep_ms(10);

        if (color2.g == color.g && color2.r == color.r && color2.b == color.b) break;
    }
}

/**
 * @brief calculate spi buffer from leds buffer
 * @private
 */
static void ws2812b_update_spi_buffer(struct ws2812b *self) {
    for (uint16_t i = 0; i < self->num_leds; i++) {
        for (uint8_t bi = 0, rbi = 7; bi < 8; bi++, rbi--) {
            // bi (bit index) go from 0 to 7, while rbi (reverse bit index) go from 7 to 0
            const uint32_t spi_buffer_i = i * bitsizeof(struct grb) + bi;
            self->spi_buffer[spi_buffer_i] = ((self->leds_buffer[i].g >> rbi) & 0b1) ? T1 : T0;
            self->spi_buffer[spi_buffer_i + 8] = ((self->leds_buffer[i].r >> rbi) & 0b1) ? T1 : T0;
            self->spi_buffer[spi_buffer_i + 16] = ((self->leds_buffer[i].b >> rbi) & 0b1) ? T1 : T0;
        }
    }
}

bool ws2812b_write(struct ws2812b *self) {
    ws2812b_update_spi_buffer(self);

    spi_set_format_8(self->spi_inst);

    if (!spi_is_busy(self->spi_inst) && spi_is_writable(self->spi_inst)) {
        spi_write_blocking(self->spi_inst, self->spi_buffer, (self->num_leds * bitsizeof(struct grb)));
        sleep_us(RES);
        return true;
    }

    return false;
}
