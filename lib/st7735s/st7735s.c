/**
 * @link https://www.waveshare.com/w/upload/e/e2/ST7735S_V1.1_20111121.pdf
 */

#include "st7735s.h"
#include "../pico_extra/pico_extra.h"

#define ST7735S_WIDTH 160
#define ST7735S_HEIGHT 80
#define ST7735S_RESOLUTION (160 * 80)

enum st7735s_system_fn_commands {
    NOP       = 0x00,
    SWRESET   = 0x01, // software reset
    RDDID     = 0x04, // read display id
    RDDST     = 0x09, // read display status
    RDDPM     = 0x0A, // read display power mode,
    RDDMADCTL = 0x0B, // read dispalt madctl
    RDDCOLMOD = 0x0C, // read display pixel format
    RDDIM     = 0x0D, // read display image mode
    RDDSM     = 0x0E, // read display signal mode
    RDDSDR    = 0x0F, // read display self-diagnostic result
    SLPIN     = 0x10, // sleep in
    SLPOUT    = 0x11, // sleep out
    PTLON     = 0x12, // partial display mode on
    NORON     = 0x13, // normal display mode on
    INVOFF    = 0x20, // display inversion off
    INVON     = 0x21, // display inversion on
    GAMSET    = 0x26, // gamma set
    DISPOFF   = 0x28, // display off
    DISPON    = 0x29, // display on
    CASET     = 0x2A, // column address set
    RASET     = 0x2B, // row address set
    RAMWR     = 0x2C, // memory write
    RGBSET    = 0x2D, // color setting for 4k, 65k and 262k
    RAMRD     = 0x2E, // memory read
    PTLAR     = 0x30, // partial area
    SCRLAR    = 0x33, // scroll area set
    TEOFF     = 0x34, // tearing effect line off
    TEON      = 0x35, // tearing effect line on
    MADCTL    = 0x36, // memory data access control
    VSCSAD    = 0x37, // scroll start address of ram
    IDMOFF    = 0x38, // idle mode off
    IDMON     = 0x39, // idle mode on
    COLMOD    = 0x3A, // interface pixel format
    RDID1     = 0xDA, // read id1 value
    RDID2     = 0xDB, // read id2 value
    RDID3     = 0xDC // read id3 value
};

enum st7735s_panel_fn_commands {
    FRMCTR1 = 0xB1, // frame rate control (in normal mode/ full colors)
    FRMCTR2 = 0xB2, // frame rate control (in idle mode/ 8-colors)
    FRMCTR3 = 0xB3, // frame rate control (in partial mode/ full colors)
    INVCTR  = 0xB4, // display inversion control
    PWCTR1  = 0xC0, // power control 1
    PWCTR2  = 0xC1, // power control 2
    PWCTR3  = 0xC2, // power control 3 (in normal mode/ full color
    PWCTR4  = 0xC3, // power control 4 (in idle mode/ 8-colors)
    PWCTR5  = 0xC4, // power control 5 (in partial mode/ full-colors)
    VMCTR1  = 0xC5, // vcom control 1
    VMOFCTR = 0xC7, // vcom offset control
    WRID2   = 0xD1, // write id2 value
    WRID3   = 0xD2, // write id3 value
    NVFCTR1 = 0xD9, // nvm control statu
    NVFCTR2 = 0xDE, // nvm read comman
    NVFCTR3 = 0xDF, // nvm write command
    GMCTRP1 = 0xE0, // gamma (‘+’polarity) correction characteristics setting
    GMCTRN1 = 0xE1, // gamma ‘-’polarity correction characteristics setting
    GCV     = 0xFC // gate pump clock frequency variable
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
    st7735s_hard_reset(self);

    gpio_init(self->pin_DC);
    gpio_set_dir(self->pin_DC, GPIO_OUT);
    st7735s_command_mode(self);

    gpio_init(self->pin_CS);
    gpio_set_function(self->pin_CS, GPIO_FUNC_SPI);
    gpio_set_dir(self->pin_CS, GPIO_OUT);
    spi_chip_deselect(self->pin_CS);

    if (self->pin_BLK != 0x00) {
        // gpio_init(self->pin_BLK);
        // gpio_set_dir(self->pin_BLK, GPIO_OUT);
    }

    st7735s_write_command(self, SWRESET);
    sleep_ms(50);

    st7735s_write_command(self, SLPOUT);
    sleep_ms(255);


    const uint8_t framerate[] = {
        FRMCTR1, 0x01, 0x2C, 0x2D,
        FRMCTR2, 0x01, 0x2C, 0x2D,
        FRMCTR3, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,
    };

    st7735s_write_command_with_data(self, framerate, 4);
    st7735s_write_command_with_data(self, framerate + 4, 4);
    st7735s_write_command_with_data(self, framerate + 8, 7);

    const uint8_t inversion[] = { INVCTR, 0x07 };
    st7735s_write_command_with_data(self, inversion, sizeof(inversion));

    const uint8_t power1[] = { PWCTR1, 0xA2, 0x02, 0x84 };
    st7735s_write_command_with_data(self, power1, sizeof(power1));

    const uint8_t power2[] = { PWCTR2, 0xC5 };
    st7735s_write_command_with_data(self, power2, sizeof power2);

    const uint8_t power3[] = { PWCTR3, 0x0A, 0x00 };
    st7735s_write_command_with_data(self, power3, sizeof(power3));

    const uint8_t power4[] = { PWCTR4, 0x8A, 0x2A };
    st7735s_write_command_with_data(self, power4, sizeof(power4));

    const uint8_t power5[] = { PWCTR5, 0x8A, 0xEE };
    st7735s_write_command_with_data(self, power5, sizeof(power5));

    const uint8_t vcom1[] = { VMCTR1, 0x0E };
    st7735s_write_command_with_data(self, vcom1, sizeof(vcom1));

    st7735s_write_command(self, INVOFF);

    const uint8_t memaccess[] = { MADCTL, 0x40 | 0x20 };
    st7735s_write_command_with_data(self, memaccess, sizeof(memaccess));

    const uint8_t colmod[] = { COLMOD, 0x05 };
    st7735s_write_command_with_data(self, colmod, sizeof(colmod));

    const uint8_t b1[] = { CASET, 0x00, 0x00, 0x00, 0x4F};
    const uint8_t b2[] = { RASET, 0x00, 0x00, 0x00, 0x9F};
    st7735s_write_command_with_data(self, b1, sizeof(b1));
    st7735s_write_command_with_data(self, b2, sizeof(b2));
    st7735s_write_command(self, INVON);


    const uint8_t bg[] = { RAMWR, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    st7735s_write_command_with_data(self, bg, sizeof(bg));

    const uint8_t gmct[] = {
            GMCTRP1,
            0x02, 0x1c, 0x07, 0x12,
            0x37, 0x32, 0x29, 0x2d,
            0x29, 0x25, 0x2B, 0x39,
            0x00, 0x01, 0x03, 0x10,
    };

    const uint8_t gmc[] = {
        GMCTRN1,
        0x03, 0x1d, 0x07, 0x06,
        0x2E, 0x2C, 0x29, 0x2D,
        0x2E, 0x2E, 0x37, 0x3F,
        0x00, 0x00, 0x02, 0x10
    };

    st7735s_write_command_with_data(self, gmct, sizeof(gmct));
    st7735s_write_command_with_data(self, gmc, sizeof(gmc));

    st7735s_write_command(self, NORON);
    sleep_ms(10);
    st7735s_write_command(self, DISPON);
    sleep_ms(100);

    self->is_on = true;
}

void st7735s_command_mode(const st7735s_t *const self) {
    gpio_put(self->pin_DC, HIGH);
}

void st7735s_data_mode(const st7735s_t *const self) {
    gpio_put(self->pin_DC, LOW);
}

/**
 * @param buffer command + data array
 * @param size size of buffer, command included
 */
void st7735s_write_command_with_data(const st7735s_t *const self, const uint8_t *const buffer, const size_t size) {
    st7735s_write_command(self, buffer[0]);
    st7735s_write_data(self, &buffer[1], size - 1);
}

void st7735s_write_command(const st7735s_t *const self, const uint8_t command) {
    st7735s_write(self, &command, 1, true);
}

void st7735s_write_data(const st7735s_t *const self, const uint8_t *const data, const size_t size) {
    st7735s_write(self, data, size, false);
}

void st7735s_write(const st7735s_t *const self, const uint8_t *const bytes, const size_t size, const bool is_command) {
    (is_command) ? st7735s_command_mode(self) : st7735s_data_mode(self);

    if (!spi_is_busy(self->spi_inst) && spi_is_writable(self->spi_inst)) {
        spi_chip_select(self->pin_CS);
        spi_write_blocking(self->spi_inst, bytes, size);
        spi_chip_deselect(self->pin_CS);
    }
}

void st7735s_hard_reset(const st7735s_t *const self) {
    gpio_put(self->pin_RES, HIGH);
    sleep_ms(100);
    gpio_put(self->pin_RES, LOW);
    sleep_ms(100);
    gpio_put(self->pin_RES, HIGH);
    sleep_ms(100);
}
