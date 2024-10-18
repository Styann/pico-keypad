/**
 * @author Styann
 * @brief small ssd1331 oled display driver, it can just write raw image and text
 * @link https://cdn-shop.adafruit.com/datasheets/SSD1331_1.2.pdf
 */

#ifndef SSD1331_H
#define SSD1331_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../pico_extra/pico_extra.h"

// DISPLAY CHARACTERISTICS
#define SSD1331_WIDTH 96
#define SSD1331_HEIGHT 64
#define SSD1331_RESOLUTION 6144
#define SSD1331_BAUD_RATE 8000000

struct ssd1331 {
	uint8_t pin_SCL;
	uint8_t pin_SDA;
	uint8_t pin_RES;
	uint8_t pin_DC;
	uint8_t pin_CS;
	spi_inst_t *spi_inst;
	struct {
		uint8_t x;
		uint8_t y;
	} coordinate;
	volatile bool is_on;
};

void ssd1331_init(struct ssd1331 *self);

void ssd1331_reset(struct ssd1331 *self);

void ssd1331_turn(struct ssd1331 *self, const bool onoff);

void ssd1331_turn_on(struct ssd1331 *self);

void ssd1331_turn_off(struct ssd1331 *self);

static void ssd1331_command_mode(struct ssd1331 *self);

static void ssd1331_data_mode(struct ssd1331 *self);

void ssd1331_write_command(struct ssd1331 *self, const uint8_t command);

void ssd1331_write_commands(struct ssd1331 *self, const uint8_t *commands, const uint len);

void ssd1331_write_data(struct ssd1331 *self, uint16_t *data, const uint len);

void ssd1331_draw_pixel(struct ssd1331 *self, uint8_t x, uint8_t y, uint16_t color);

void ssd1331_fill_screen(struct ssd1331 *self, uint16_t color);

void ssd1331_print_char(struct ssd1331 *self, uint8_t x, uint8_t y, const char c);

void ssd1331_print(struct ssd1331 *self, const char *c);

void ssd1331_println(struct ssd1331 *self, const char *c);

enum ssd1331_commands {
	SET_COLUMN_ADDRESS     			   = 0x15,
	SET_ROW_ADDRESS        			   = 0x75,
	SET_CONTRAST_COLOR_A   			   = 0x81,
	SET_CONTRAST_COLOR_B   			   = 0x82,
	SET_CONTRAST_COLOR_C   			   = 0x83,
	MASTER_CURRENT_CONTROL 			   = 0x87,
	SET_SECOND_PRECHARGE_SPEED_COLOR_A = 0x8A,
	SET_SECOND_PRECHARGE_SPEED_COLOR_B = 0x8B,
	SET_SECOND_PRECHARGE_SPEED_COLOR_C = 0x8C,
	SET_REMAP_AND_DATA_FORMAT 		   = 0xA0,
	SET_DISPLAY_START_LINE 			   = 0xA1,
	SET_DISPLAY_OFFSET 				   = 0xA2,
	SET_DISPLAY_MODE_NORMAL 		   = 0xA4,
	SET_DISPLAY_MODE_ENTIRE_ON 		   = 0xA5,
	SET_DISPLAY_MODE_ENTIRE_OFF 	   = 0xA6,
	SET_DISPLAY_MODE_INVERSE 		   = 0xA7,
	SET_MULTIPLEX_RATIO 			   = 0xA8,
	DIM_MODE_SETTING 				   = 0xAB,
	SET_MASTER_CONFIGURATION 		   = 0xAD,
	SET_DISPLAY_DIM_MODE_DISPLAY_ON    = 0xAC,
	SET_DISPLAY_OFF					   = 0xAE,
	SET_DISPLAY_ON  				   = 0xAF,
	POWER_SAVE_MODE 				   = 0xB0,
	PHASE_1_AND_2_PERIOD_ADJUSTMENT    = 0xB1,
	SET_DISPLAY_CLOCK_DIVIDE_RATIO 	   = 0xB3,
	SET_GRAY_SCALE_TABLE			   = 0xB8,
	ENABLE_LINEAR_GRAY_SCALE_TABLE	   = 0xB9,
	SET_PRECHARGE_VOLTAGE			   = 0xBB,
	SET_Vcomh_VOLTAGE				   = 0xBE,
	NOP								   = 0xBC, // or 0xBD or 0xE3
	SET_COMMAND_LOCK 				   = 0xFD,
	DRAW_LINE 						   = 0x21,
	DRAW_RECTANGLE 					   = 0x22,
	COPY 							   = 0x23,
	DIW_WINDOW 						   = 0x24,
	CLEAR_WINDOW 					   = 0x25,
	FILL_TOGGLE 					   = 0x26,
	CONTINUOUS_X_AND_Y_SCROLLING_SETUP = 0x27,
	DEACTIVATE_SCROLLING 			   = 0x2E,
	ACTIVATE_SCROLLING 				   = 0x2F,
	COLOR_ORDER_RGB					   = 0x72,
	COLOR_ORDER_BGR					   = 0x76
};

#endif
