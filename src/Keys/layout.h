/*#define LAYOUT_LENGTH 17
#define LAYOUT_COLUMN_LENGTH 4
#define LAYOUT_ROW_LENGTH 5


static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = {2, 3, 4, 5};
static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = {11, 12, 13, 14, 15};

static const hid_keyboard_report_t layout[LAYOUT_ROW_LENGTH][LAYOUT_COLUMN_LENGTH] = {
    {
        {KEYBOARD_MODIFIER_LEFTCTRL, 0, {HID_KEY_CONTROL_LEFT, 0, 0, 0, 0, 0}},
        //{0, 0, {HID_KEY_NUM_LOCK       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_DIVIDE  , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_MULTIPLY, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_SUBTRACT, 0, 0, 0, 0, 0}}
    },
    {
        {KEYBOARD_MODIFIER_LEFTSHIFT, 0, {HID_KEY_SHIFT_LEFT, 0, 0, 0, 0, 0}},
        //{0, 0, {HID_KEY_KEYPAD_7       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_8       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_9       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_ADD     , 0, 0, 0, 0, 0}}
    },
    {
        {0, 0, {HID_KEY_ESCAPE, 0, 0, 0, 0, 0}},
        //{0, 0, {HID_KEY_KEYPAD_4       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_5       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_6       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_ENTER   , 0, 0, 0, 0, 0}}
    },
    {
        {0, 0, {HID_KEY_KEYPAD_1       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_2       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_3       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_0       , 0, 0, 0, 0, 0}}
    },
    {
        {0, 0, {HID_KEY_KEYPAD_DECIMAL , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_NONE           , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_NONE           , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_NONE           , 0, 0, 0, 0, 0}}
    }
};*/