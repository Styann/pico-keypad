#ifndef SSD1331_H
#define SSD1331_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "../pico_extra.h"

// DISPLAY CHARACTERISTICS
#define SSD1331_WIDTH 96
#define SSD1331_HEIGHT 64
#define SSD1331_RESOLUTION 6144
#define SSD1331_BAUD_RATE 8000000

struct ssd1331 {
	uint8_t pin_SCL; // CLK
	uint8_t pin_SDA; // Serial Data, MOSI
	uint8_t pin_RES; // low -> chip init excuted
	uint8_t pin_DC; // DataCommand / high -> data = data/ low -> data = command
	uint8_t pin_CS; // Chip Select
	spi_inst_t *spi_inst;
};

void ssd1331_init(struct ssd1331 *this);

void ssd1331_reset(struct ssd1331 *this);

static void ssd1331_command_mode(struct ssd1331 *this);

static void ssd1331_data_mode(struct ssd1331 *this);

void ssd1331_write_command(struct ssd1331 *this, uint8_t command);

void ssd1331_write_commands(struct ssd1331 *this, uint8_t *commands, size_t size);

void ssd1331_write_data(struct ssd1331 *this, uint16_t *data, size_t size);

#define RGB16_RED 0xF800

// COMMANDS
#define SET_COLUMN_ADDRESS     			  	0x15
#define SET_ROW_ADDRESS        			  	0x75
#define SET_CONTRAST_COLOR_A   			  	0x81
#define SET_CONTRAST_COLOR_B   			  	0x82
#define SET_CONTRAST_COLOR_C   			  	0x83
#define MASTER_CURRENT_CONTROL 			  	0x87
#define SET_SECOND_PRECHARGE_SPEED_COLOR_A	0x8A
#define SET_SECOND_PRECHARGE_SPEED_COLOR_B	0x8B
#define SET_SECOND_PRECHARGE_SPEED_COLOR_C	0x8C
#define SET_REMAP_AND_DATA_FORMAT 		  	0xA0
#define SET_DISPLAY_START_LINE 			  	0xA1
#define SET_DISPLAY_OFFSET 				  	0xA2
#define SET_DISPLAY_MODE_NORMAL 		  	0xA4
#define SET_DISPLAY_MODE_ENTIRE_ON 		  	0xA5
#define SET_DISPLAY_MODE_ENTIRE_OFF 	  	0xA6
#define SET_DISPLAY_MODE_INVERSE 		  	0xA7
#define SET_MULTIPLEX_RATIO 			  	0xA8
#define DIM_MODE_SETTING 				  	0xAB
#define SET_MASTER_CONFIGURATION 		  	0xAD
#define SET_DISPLAY_DIM_MODE_DISPLAY_ON   	0xAC
#define SET_DISPLAY_OFF					  	0xAE
#define SET_DISPLAY_ON  				  	0xAF
#define POWER_SAVE_MODE 				  	0xB0
#define PHASE_1_AND_2_PERIOD_ADJUSTMENT   	0xB1
#define SET_DISPLAY_CLOCK_DIVIDE_RATIO 		0xB3
#define SET_GRAY_SCALE_TABLE				0xB8
#define ENABLE_LINEAR_GRAY_SCALE_TABLE		0xB9
#define SET_PRECHARGE_VOLTAGE				0xBB
#define SET_Vcomh_VOLTAGE					0xBE
#define NOP									0xBC // or 0xBD or 0xE3
#define SET_COMMAND_LOCK 					0xFD
#define DRAW_LINE 							0x21
#define DRAW_RECTANGLE 						0x22
#define COPY 								0x23
#define DIW_WINDOW 							0x24
#define CLEAR_WINDOW 						0x25
#define FILL_TOGGLE 						0x26
#define CONTINUOUS_X_AND_Y_SCROLLING_SETUP	0x27
#define DEACTIVATE_SCROLLING 				0x2E
#define ACTIVATE_SCROLLING 					0x2F

#define COLOR_ORDER_RGB						0x72
#define COLOR_ORDER_BGR						0x76

#endif
