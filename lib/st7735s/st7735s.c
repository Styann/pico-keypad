/**
 * @link https://www.waveshare.com/w/upload/e/e2/ST7735S_V1.1_20111121.pdf
 */

#include "st7735s.h"
#include "../pico_extra/pico_extra.h"
#include <string.h>

enum st7735s_system_fn_commands {
    ST7735_NOP       = 0x00,
    ST7735_SWRESET   = 0x01, // software reset
    ST7735_RDDID     = 0x04, // read display id
    ST7735_RDDST     = 0x09, // read display status
    ST7735_RDDPM     = 0x0A, // read display power mode,
    ST7735_RDDMADCTL = 0x0B, // read display madctl
    ST7735_RDDCOLMOD = 0x0C, // read display pixel format
    ST7735_RDDIM     = 0x0D, // read display image mode
    ST7735_RDDSM     = 0x0E, // read display signal mode
    ST7735_RDDSDR    = 0x0F, // read display self-diagnostic result
    ST7735_SLPIN     = 0x10, // sleep in
    ST7735_SLPOUT    = 0x11, // sleep out
    ST7735_PTLON     = 0x12, // partial display mode on
    ST7735_NORON     = 0x13, // normal display mode on
    ST7735_INVOFF    = 0x20, // display inversion off
    ST7735_INVON     = 0x21, // display inversion on
    ST7735_GAMSET    = 0x26, // gamma set
    ST7735_DISPOFF   = 0x28, // display off
    ST7735_DISPON    = 0x29, // display on
    ST7735_CASET     = 0x2A, // column address set
    ST7735_RASET     = 0x2B, // row address set
    ST7735_RAMWR     = 0x2C, // memory write
    ST7735_RGBSET    = 0x2D, // color setting for 4k, 65k and 262k
    ST7735_RAMRD     = 0x2E, // memory read
    ST7735_PTLAR     = 0x30, // partial area
    ST7735_SCRLAR    = 0x33, // scroll area set
    ST7735_TEOFF     = 0x34, // tearing effect line off
    ST7735_TEON      = 0x35, // tearing effect line on
    ST7735_MADCTL    = 0x36, // memory data access control
    ST7735_VSCSAD    = 0x37, // scroll start address of ram
    ST7735_IDMOFF    = 0x38, // idle mode off
    ST7735_IDMON     = 0x39, // idle mode on
    ST7735_COLMOD    = 0x3A, // interface pixel format
    ST7735_RDID1     = 0xDA, // read id1 value
    ST7735_RDID2     = 0xDB, // read id2 value
    ST7735_RDID3     = 0xDC // read id3 value
};

enum st7735s_panel_fn_commands {
    ST7735_FRMCTR1 = 0xB1, // frame rate control (in normal mode/ full colors)
    ST7735_FRMCTR2 = 0xB2, // frame rate control (in idle mode/ 8-colors)
    ST7735_FRMCTR3 = 0xB3, // frame rate control (in partial mode/ full colors)
    ST7735_INVCTR  = 0xB4, // display inversion control
    ST7735_PWCTR1  = 0xC0, // power control 1
    ST7735_PWCTR2  = 0xC1, // power control 2
    ST7735_PWCTR3  = 0xC2, // power control 3 (in normal mode/ full color
    ST7735_PWCTR4  = 0xC3, // power control 4 (in idle mode/ 8-colors)
    ST7735_PWCTR5  = 0xC4, // power control 5 (in partial mode/ full-colors)
    ST7735_VMCTR1  = 0xC5, // vcom control 1
    ST7735_VMOFCTR = 0xC7, // vcom offset control
    ST7735_WRID2   = 0xD1, // write id2 value
    ST7735_WRID3   = 0xD2, // write id3 value
    ST7735_NVFCTR1 = 0xD9, // nvm control statu
    ST7735_NVFCTR2 = 0xDE, // nvm read comman
    ST7735_NVFCTR3 = 0xDF, // nvm write command
    ST7735_GMCTRP1 = 0xE0, // gamma (‘+’polarity) correction characteristics setting
    ST7735_GMCTRN1 = 0xE1, // gamma ‘-’polarity correction characteristics setting
    ST7735_GCV     = 0xFC // gate pump clock frequency variable
};

void st7735s_init(st7735s_t *const self, uint baudrate) {
    spi_init(self->spi_inst, baudrate);
    spi_set_format_8(self->spi_inst);

    gpio_init(self->pin_SCL);
    gpio_set_function(self->pin_SCL, GPIO_FUNC_SPI);

    gpio_init(self->pin_SDA);
    gpio_set_function(self->pin_SDA, GPIO_FUNC_SPI);

    gpio_init(self->pin_RES);
    gpio_set_dir(self->pin_RES, GPIO_OUT);

    gpio_init(self->pin_DC);
    gpio_set_dir(self->pin_DC, GPIO_OUT);

    gpio_init(self->pin_CS);
    gpio_set_function(self->pin_CS, GPIO_FUNC_SPI);
    gpio_set_dir(self->pin_CS, GPIO_OUT);
    spi_chip_deselect(self->pin_CS);

    // commands list
    const uint8_t a[] = { ST7735_FRMCTR1, 0x01, 0x2C, 0x2D };
    const uint8_t b[] = { ST7735_FRMCTR2, 0x01, 0x2C, 0x2D };
    const uint8_t c[] = { ST7735_FRMCTR3, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D };
    const uint8_t d[] = { ST7735_INVCTR, 0x07 };
    const uint8_t e[] = { ST7735_PWCTR1, 0xA2, 0x02, 0x84 };
    const uint8_t f[] = { ST7735_PWCTR2, 0xC5 };
    const uint8_t g[] = { ST7735_PWCTR3, 0x0A, 0x00 };
    const uint8_t h[] = { ST7735_PWCTR4, 0x8A, 0x2A };
    const uint8_t i[] = { ST7735_PWCTR5, 0x8A, 0xEE };
    const uint8_t j[] = { ST7735_VMCTR1, 0x0E };
    const uint8_t k[] = { ST7735_MADCTL, 0b01101000 };
    const uint8_t l[] = { ST7735_COLMOD, 0x05 };
    const uint8_t o[] = { ST7735_GMCTRP1, 0x04, 0x22, 0x07, 0x0A, 0x2E, 0x30, 0x25, 0x2A, 0x28, 0x26, 0x2E, 0x3A, 0x00, 0x01, 0x03, 0x13 };
    const uint8_t p[] = { ST7735_GMCTRN1, 0x04, 0x16, 0x06, 0x0D, 0x2D, 0x26, 0x23, 0x27, 0x27, 0x25, 0x2D, 0x3B, 0x00, 0x01, 0x04, 0x13 };

    st7735s_hardware_reset(self);

    st7735s_write_command(self, ST7735_SWRESET);
    sleep_ms(100);

    st7735s_write_command(self, ST7735_SLPOUT);
    sleep_ms(255);

    // st7735s_write_command(self, ST7735_DISPOFF);
    // st7735s_write_command_with_data(self, a, sizeof(a));
    // st7735s_write_command_with_data(self, b, sizeof(b));
    // st7735s_write_command_with_data(self, c, sizeof(c));
    // st7735s_write_command_with_data(self, d, sizeof(d));
    // st7735s_write_command_with_data(self, e, sizeof(e));
    // st7735s_write_command_with_data(self, f, sizeof(f));
    // st7735s_write_command_with_data(self, g, sizeof(g));
    // st7735s_write_command_with_data(self, h, sizeof(h));
    // st7735s_write_command_with_data(self, i, sizeof(i));
    st7735s_write_command_with_data(self, j, sizeof(j));
    st7735s_write_command_with_data(self, k, sizeof(k));

    // st7735s_write_command(self, ST7735_INVON);
    // st7735s_write_command_with_data(self, o, sizeof(o));
    // st7735s_write_command_with_data(self, p, sizeof(p));
    st7735s_write_command_with_data(self, l, sizeof(l));
    st7735s_write_command(self, ST7735_INVON);

    // st7735s_write_command(self, ST7735_NORON);
    // sleep_ms(10);
    st7735s_write_command(self, ST7735_DISPON);
    sleep_ms(100);

    self->is_on = true;
}

/**
 * @brief set DC low
 */
void st7735s_command_mode(const st7735s_t *const self) {
    gpio_put(self->pin_DC, LOW);
}

/**
 * @brief set DC high
 */
void st7735s_data_mode(const st7735s_t *const self) {
    gpio_put(self->pin_DC, HIGH);
}

/**
 * @brief write one command followed by data
 * @param buffer command + data array
 * @param len len of buffer, command included
 */
void st7735s_write_command_with_data(const st7735s_t *const self, const uint8_t *const buffer, const uint len) {
    st7735s_write_command(self, buffer[0]);
    st7735s_write_data(self, &buffer[1], len - 1);
}

/**
 * @brief write one command
 */
void st7735s_write_command(const st7735s_t *const self, const uint8_t command) {
    st7735s_command_mode(self);
    st7735s_write(self, &command, 1);
}

/**
 * @brief write data
 */
void st7735s_write_data(const st7735s_t *const self, const uint8_t *const data, const uint len) {
    st7735s_data_mode(self);
    st7735s_write(self, data, len);
}

/**
 * @brief write BGR pixels
 */
void st7735s_write_data16(const st7735s_t *const self, const uint16_t *const data, const uint len) {
    st7735s_data_mode(self);
    spi_set_format_16(self->spi_inst);

    if (!spi_is_busy(self->spi_inst) && spi_is_writable(self->spi_inst)) {
        spi_chip_select(self->pin_CS);
        spi_write16_blocking(self->spi_inst, data, len);
        spi_chip_deselect(self->pin_CS);
    }
}

/**
 * @brief write command or data
 */
void st7735s_write(const st7735s_t *const self, const uint8_t *const bytes, const uint len) {
    spi_set_format_8(self->spi_inst);

    if (!spi_is_busy(self->spi_inst) && spi_is_writable(self->spi_inst)) {
        spi_chip_select(self->pin_CS);
        spi_write_blocking(self->spi_inst, bytes, len);
        spi_chip_deselect(self->pin_CS);
    }
}

void st7735s_hardware_reset(const st7735s_t *const self) {
    gpio_put(self->pin_RES, LOW);
    sleep_ms(200);
    gpio_put(self->pin_RES, HIGH);
    sleep_ms(20);
}

void st7735s_write_image(const st7735s_t *const self, const uint16_t *const image) {
    // TODO: fix dead column
    const uint8_t window[] = { ST7735_CASET, 0, 0, 0, 159 };
    const uint8_t window2[] = { ST7735_RASET, 0, 26, 0, 79 };
    st7735s_write_command_with_data(self, window, 5);
    st7735s_write_command_with_data(self, window2, 4);

    st7735s_write_command(self, ST7735_RAMWR);
    st7735s_write_data16(self, image, 80 * 160);
}

void st7735s_write_fill_screen(const st7735s_t *const self, const uint16_t color) {

}
