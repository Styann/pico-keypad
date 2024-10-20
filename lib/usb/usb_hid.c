#include "usb_hid.h"
#include "usb.h"

extern struct usb_device pico;

/**
 * @brief send keyboard report to host
 */
void usb_send_keyboard_report(const struct usb_keyboard_report *report) {
    struct usb_endpoint *endpoint = &(pico.hid_interfaces[0].endpoint);
    usb_xfer(endpoint, (uint8_t *)report, sizeof(const struct usb_keyboard_report));
}

void usb_send_mouse_report(const struct usb_mouse_report *report) {
    struct usb_endpoint *endpoint = &(pico.hid_interfaces[1].endpoint);
    usb_xfer(endpoint, (uint8_t *)report, sizeof(const struct usb_mouse_report));
}

void usb_send_single_keycode(uint8_t keycode) {
    const struct usb_keyboard_report report = { 1, 0, 0, { keycode, 0, 0, 0, 0, 0 } };
    usb_send_keyboard_report(&report);
    release_keyboard();
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
    struct usb_endpoint *endpoint = &(pico.hid_interfaces[0].endpoint);
    usb_xfer(endpoint, (uint8_t*)report, sizeof(struct usb_consumer_control_report));
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
    struct usb_endpoint *endpoint = &(pico.hid_interfaces[0].endpoint);
    usb_xfer(endpoint, (uint8_t*)report, sizeof(struct usb_gamepad_report));
}
