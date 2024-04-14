#include "ws2812b.h"

void ws2812b_init(struct ws2812b *this) {
    // buffer size calculation
    this->buffer_size = sizeof(uint8_t[this->num_leds][GRB_BIT_SIZE]);

    // buffer allocation
    this->buffer = calloc(this->buffer_size, sizeof(uint8_t));
    this->buffer_alloc_status = (this->buffer == NULL) ? false : true;

    spi_init(this->spi_inst, WS2812B_BAUD_RATE);
    gpio_set_function(this->spi_mosi_pin, GPIO_FUNC_SPI);

    ws2812b_set_off(this);
}

void ws2812b_set_all(struct ws2812b *this, grb_t color) {
    uint8_t color_bits_buf[GRB_BIT_SIZE];

    for (uint8_t i = 0, ri = GRB_BIT_SIZE - 1; i < GRB_BIT_SIZE; i++, ri--) {
        color_bits_buf[i] = ((color >> ri) & 0x1) ? T1 : T0;
    }

    for (uint16_t i = 0; i < this->num_leds; i++) {
        for (uint8_t j = 0; j < GRB_BIT_SIZE; j++) {
            this->buffer[i * GRB_BIT_SIZE + j] = color_bits_buf[j];
        }
    }
}

void ws2812b_set_one(struct ws2812b *this, uint16_t led, grb_t color) {
    uint8_t ri = GRB_BIT_SIZE;

    uint16_t led_index = (led * GRB_BIT_SIZE);

    for (uint8_t i = 0; i < GRB_BIT_SIZE; i++) {
        this->buffer[led_index + i] = ((color >> --ri) & 0x1) ? T1 : T0;
    }
}

void ws2812b_set_off(struct ws2812b *this) {
    memset(this->buffer, T0, this->buffer_size);
}

void ws2812b_set_brightness(struct ws2812b *this) {
    // todo
    // retrieve value from buffer, then grb*brightness/100
}

bool ws2812b_write(struct ws2812b *this) {
    bool result = false;
    spi_inst_t *spi_inst = this->spi_inst;

    if (!spi_is_busy(spi_inst) && spi_is_writable(spi_inst)) {
        spi_write_blocking(spi_inst, this->buffer, this->buffer_size);
        sleep_us(RES);
        result = true;
    }

    return result;
}

grb_t rgb_to_grb(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16 | g << 8 | b);
}
