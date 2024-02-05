#ifndef LAYOUT_H
#define LAYOUT_H

#include "pico/stdlib"
#include "../usb/usb_hid.h"

#define LAYOUT_LENGTH 17
#define LAYOUT_COLUMN_LENGTH 4
#define LAYOUT_ROW_LENGTH 5


typedef struct {
    uint8_t keycode;
    uint8_t modifier;
    uint8_t consumer_control;
} keyboard_key_t;

static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = {2, 3, 4, 5};
static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = {11, 12, 13, 14, 15};

static const uint16_t layout[LAYOUT_ROW_LENGTH][LAYOUT_COLUMN_LENGTH] = {
    {KC_ESCAPE    , KC_F1,       KC_F2}
    {KC_GRAVE     , KC_1       , KC_2       , KC_3    , KC_4        , KC_5        , KC_6   , KC_7, KC_8, KC_9, KC_0, KC_MINUS, KC_EQUAL, KC_BACKSPACE}
    {KC_TAB       , KC_A       , KC_Z       , KC_E    , KC_R        , KC_T        , KC_Y   , KC_U, KC_I, KC_O, KC_P,         KC_BRACKET_LEFT, KC_BRACKET_RIGHT, KC_BACKSLASH},
    {KC_CAPS_LOCK , KC_Q       , KC_S       , KC_D    , KC_F        , KC_G        , KC_H   , KC_J, KC_K, KC_L, KC_SEMICOLON, KC_APOSTROPHE,   KC_ENTER},
    {KC_SHIFT_LEFT, KC_W       , KC_X       , KC_C    , KC_V        , KC_B        , KC_N   , KC_COMMA, KC_PERIOD, KC_SLASH, KC_SHIFT_RIGHT},
    {KC_CTRL_LEFT , KC_GUI_LEFT, KC_ALT_LEFT, KC_SPACE, KC_ALT_RIGHT, KC_GUI_RIGHT, KC_MENU, KC_CTRL_RIGHT }
    
    
};

#endif