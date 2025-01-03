#include "usb_config.h"
#include <stdio.h>



// #define USB_REPORT_DESCRIPTOR_LENGTH 40
// const uint8_t fightstick_report_descriptor[40] = {
//     USAGE_PAGE, GENERIC_DESKTOP_PAGE,

//     USAGE, USAGE_JOYSTICK,
//     COLLECTION, COLLECTION_APPLICATION,
//             COLLECTION, COLLECTION_LOGICAL,
//                     // axis x and y, 1 byte for each
//                     REPORT_SIZE, 0x08,
//                     REPORT_COUNT, 0x02,
//                     LOGICAL_MINIMUM, 0b10000001,
//                     LOGICAL_MAXIMUM, 0b01111111,
//                     // PHYSICAL_MINIMUM, 0x00,
//                     // PHYSICAL_MAXIMUM, 0xFF,
//                     USAGE, USAGE_X,
//                     USAGE, USAGE_Y,
//                     INPUT, DATA_VAR_ABS,

//                     REPORT_SIZE, 0x01,
//                     REPORT_COUNT, 0x08,
//                     LOGICAL_MAXIMUM, 0x01,
//                     PHYSICAL_MAXIMUM, 0x01,
//                     USAGE_PAGE, BUTTON_PAGE,
//                     USAGE_MINIMUM, 0x01,
//                     USAGE_MAXIMUM, 0x08,
//                     INPUT, DATA_VAR_ABS,
//             END_COLLECTION,
//     END_COLLECTION
// };
