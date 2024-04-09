#include "ws2812b.h"

void ws2812b_init(struct ws2812b *ws2812b) {
    // buffer size calculation
    ws2812b->buffer_size = (ws2812b->num_leds * GRB_BIT_LEN) + RES;

    memset(ws2812b->buffer, 0, ws2812b->buffer_size);

    spi_init(ws2812b->spi_inst, WS2812B_BAUD_RATE);
    gpio_set_function(ws2812b->spi_mosi_pin, GPIO_FUNC_SPI);
}

void ws2812b_set_all(struct ws2812b *ws2812b, grb_t *color) {
    uint32_t hcolor = grb_to_uint32(color);

    uint8_t color_bits_buf[24];

    for (uint16_t i = 0; i < GRB_BIT_LEN; i++) {
        color_bits_buf[i] = ((hcolor >> i) & 0x1) ? 0b11111110 : 0b10000000;
    }

    for (uint16_t bit_i = 0; bit_i < GRB_BIT_LEN; bit_i++) {
        for (uint16_t led_i = 0; led_i < ws2812b->num_leds; led_i++) {
            *(ws2812b->buffer + (led_i * 24)) = color_bits_buf[bit_i];
        }
    }
}

bool ws2812b_write(struct ws2812b *ws2812b) {
    spi_inst_t *spi_inst = ws2812b->spi_inst;

    if (!spi_is_busy(spi_inst) && spi_is_writable(spi_inst)) {
        spi_write_blocking(spi_inst, ws2812b->buffer, ws2812b->buffer_size);
        return true;
    }

    return false;
}

uint32_t grb_to_uint32(grb_t *color) {
    return ((color->g << 16) | (color->r << 8) | color->b);
}







void ws2812b_test_init(void) {
    spi_init(spi0, WS2812B_BAUD_RATE);
    gpio_set_function(19, GPIO_FUNC_SPI);

}

void ws2812b_test(void) {

    const uint8_t one = 0b11111110;
    const uint8_t zero = 0b10000000;
    uint8_t buffer[24 + RES] = {
        one, one, one, one, one, one, one, one,
        zero, zero, zero, zero, zero, zero, zero, zero,
        zero, zero, zero, zero, zero, zero, zero, zero,
    };

    spi_write_blocking(spi0, buffer, 25);

    //sleep_us(60);
}
