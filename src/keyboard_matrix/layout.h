#ifndef LAYOUT_H
#define LAYOUT_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"
#include "../pico_extra.h"

/*#define LAYOUT_COLUMN_LENGTH 15
#define LAYOUT_ROW_LENGTH 5

static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = {
    GPIO6, GPIO7, GPIO8, GPIO9, GPIO10, GPIO11, GPIO12, GPIO13, GPIO14, GPIO15, GPIO16, GPIO17, GPIO18, GPIO19, GPIO20
};

static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = { GPIO0, GPIO1, GPIO2, GPIO3, GPIO4 };

static const uint16_t layout[LAYOUT_ROW_LENGTH][LAYOUT_COLUMN_LENGTH] = {
    // GPIO6         GPIO7        GPIO8        GPIO9     GPIO10        GPIO11   GPIO12         GPIO13         GPIO14          GPIO15     GPIO16        GPIO17                  GPIO18                   GPIO19         GPIO20
    { KC_ESCAPE,     KC_1,        KC_2,        KC_3,     KC_4,         KC_5,    KC_6,          KC_7,          KC_8,           KC_9,      KC_0,         KC_MINUS,               KC_EQUAL,                KC_BACKSPACE,  KC_DELETE    }, // GPIO0
    { KC_TAB,        KC_A,        KC_Z,        KC_E,     KC_R,         KC_T,    KC_Y,          KC_U,          KC_I,           KC_O,      KC_P,         KC_SQUARE_BRACKET_LEFT, KC_SQUARE_BRACKET_RIGHT, KC_BACKSLASH,  KC_GRAVE     }, // GPIO1
    { KC_CAPS_LOCK,  KC_Q,        KC_S,        KC_D,     KC_F,         KC_G,    KC_H,          KC_J,          KC_K,           KC_L,      KC_M,         KC_APOSTROPHE,          KC_ENTER,                KC_NONE,       KC_PAGE_UP   }, // GPIO2
    { KC_SHIFT_LEFT, KC_W,        KC_X,        KC_C,     KC_V,         KC_B,    KC_N,          KC_SEMICOLON,  KC_COMMA,       KC_PERIOD, KC_SLASH,     KC_SHIFT_RIGHT,         KC_ARROW_UP,             KC_NONE,       KC_PAGE_DOWN }, // GPIO3
    { KC_CTRL_LEFT,  KC_GUI_LEFT, KC_ALT_LEFT, KC_SPACE, KC_ALT_RIGHT, KC_NONE, KC_ARROW_LEFT, KC_ARROW_DOWN, KC_ARROW_RIGHT, KC_NONE,   KC_NONE,      KC_NONE,                KC_NONE,                 KC_NONE,       KC_NONE      }  // GPIO4
};*/

#define LAYOUT_COLUMN_LENGTH 3
#define LAYOUT_ROW_LENGTH 3

static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = { GPIO3, GPIO4, GPIO5 };
static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = { GPIO0, GPIO1, GPIO2 };

static const uint8_t layout[LAYOUT_COLUMN_LENGTH][LAYOUT_ROW_LENGTH] = {
    { KC_A, KC_B, KC_C },
    { KC_D, KC_E, KC_F },
    { KC_G, KC_H, KC_I }
};

#endif