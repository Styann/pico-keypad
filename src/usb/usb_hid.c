#include "usb_hid.h"
#include "usb.h"

/**
 * @brief send keyboard report to host
 */
void usb_send_keyboard_report(struct usb_keyboard_report *report){
    struct usb_endpoint *endpoint = usb_get_endpoint_configuration(EP_IN_HID);
    usb_start_transfer(endpoint, (uint8_t*)report, sizeof(struct usb_keyboard_report));
}

/**
 * @brief send report with all fields at 0
 */
void release_keyboard(void) {
    struct usb_keyboard_report release = { 0x01, 0, 0, { 0, 0, 0, 0, 0, 0 } };
    usb_send_keyboard_report(&release);
}

/**
 * @brief send consumer control report to host
 */
void usb_send_consumer_control_report(struct usb_consumer_control_report *report) {
    struct usb_endpoint *endpoint = usb_get_endpoint_configuration(EP_IN_HID);
    usb_start_transfer(endpoint, (uint8_t*)report, sizeof(struct usb_consumer_control_report));
}

/**
 * @brief send consumer control, then release
 * @param consumer_control
 */
void usb_send_consumer_control(uint16_t consumer_control) {
    struct usb_consumer_control_report report = { 0x02, consumer_control };

    // send
    usb_send_consumer_control_report(&report);

    // release
    report.consumer_control = 0x0000;
    usb_send_consumer_control_report(&report);
}

void usb_send_gamepad_report(struct usb_gamepad_report *report) {
    struct usb_endpoint *endpoint = usb_get_endpoint_configuration(EP_IN_HID);
    usb_start_transfer(endpoint, (uint8_t*)report, sizeof(struct usb_gamepad_report));
}
