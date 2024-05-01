#include "ssd1331.h"

void ssd1331_init(struct ssd1331 *this) {
    gpio_init(this->pin_SCL);
    gpio_init(this->pin_SDA);
    gpio_init(this->pin_RES);
    gpio_init(this->pin_DC);
    gpio_init(this->pin_CS);

    spi_init(this->spi_inst, SSD1331_BAUD_RATE);
    // gpio_set_function(this->, GPIO_FUNC_SPI);
}