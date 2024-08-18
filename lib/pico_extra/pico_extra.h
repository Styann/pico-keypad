/**
 * @author Styann
 * @brief contains some generic macro/function...
 */

#ifndef PICO_EXTRA_H
#define PICO_EXTRA_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

#define LOW 0
#define HIGH 1

#define GPIO_IRQ_CHANGE (GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE)

#define BIT_SIZE_OF(type) (sizeof(type) * 8)

enum GPIO {
    GPIO0 = 0,
    GPIO1,
    GPIO2,
    GPIO3,
    GPIO4,
    GPIO5,
    GPIO6,
    GPIO7,
    GPIO8,
    GPIO9,
    GPIO10,
    GPIO11,
    GPIO12,
    GPIO13,
    GPIO14,
    GPIO15,
    GPIO16,
    GPIO17,
    GPIO18,
    GPIO19,
    GPIO20,
    GPIO21,
    GPIO22,
    GPIO25 = 25,
    GPIO26 = 26,
    GPIO27 = 27,
    GPIO28 = 28,
};

static inline uint32_t millis(void) {
    return to_ms_since_boot(get_absolute_time());
}

static inline void built_in_led_init(void) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

static inline void built_in_led_on(void) {
    gpio_put(PICO_DEFAULT_LED_PIN, HIGH);
}

static inline void built_in_led_off(void) {
    gpio_put(PICO_DEFAULT_LED_PIN, LOW);
}

static inline void built_in_led_put(bool value) {
    gpio_put(PICO_DEFAULT_LED_PIN, value);
}

/**
 * @brief select a spi device
 * @param pin_cs
 */
static inline void spi_chip_select(uint8_t pin_cs) {
    asm volatile("nop \n nop \n nop");
    gpio_put(pin_cs, LOW);
    asm volatile("nop \n nop \n nop");
}

/**
 * @brief deselect a spi device
 * @param pin_cs
 */
static inline void spi_chip_deselect(uint8_t pin_cs) {
    asm volatile("nop \n nop \n nop");
    gpio_put(pin_cs, HIGH);
    asm volatile("nop \n nop \n nop");
}

static inline void spi_set_format_from_inst(spi_inst_t *spi, uint data_bits) {
    spi_cpol_t cpol = (spi == spi0) ? SPI_CPOL_0 : SPI_CPOL_1;
    spi_cpha_t cpha = (spi == spi0) ? SPI_CPHA_0 : SPI_CPHA_1;
    spi_set_format(spi, data_bits, cpol,  cpha, SPI_MSB_FIRST);
}

static inline void spi_set_format_8(spi_inst_t *spi) {
    spi_set_format_from_inst(spi, 8);
}

static inline void spi_set_format_16(spi_inst_t *spi) {
    spi_set_format_from_inst(spi, 16);
}

#endif
