#include "usb_hid.h"
#include "usb.h"

/**
 * @brief send keyboard report to host
 */
void usb_send_keyboard_report(struct usb_endpoint *const endpoint, const struct usb_keyboard_report *const report) {
    usb_xfer(endpoint, (uint8_t *)report, sizeof(const struct usb_keyboard_report));
}

void usb_send_mouse_report(struct usb_endpoint *const endpoint, const struct usb_mouse_report *const report) {
    usb_xfer(endpoint, (uint8_t *)report, sizeof(const struct usb_mouse_report));
}

void usb_send_single_keycode(struct usb_endpoint *const endpoint, uint8_t keycode) {
    const struct usb_keyboard_report report = { 1, 0, 0, { keycode, 0, 0, 0, 0, 0 } };
    usb_send_keyboard_report(endpoint, &report);
    release_keyboard(endpoint);
}

/**
 * @brief send report with all fields at 0
 */
void release_keyboard(struct usb_endpoint *const endpoint) {
    struct usb_keyboard_report release = { 0x01, 0, 0, { 0, 0, 0, 0, 0, 0 } };
    usb_send_keyboard_report(endpoint, &release);
}

/**
 * @brief send consumer control report to host
 */
void usb_send_consumer_control_report(struct usb_endpoint *const endpoint, const struct usb_consumer_control_report *const report) {
    usb_xfer(endpoint, (uint8_t*)report, sizeof(struct usb_consumer_control_report));
}

/**
 * @brief send consumer control, then release
 * @param consumer_control
 */
void usb_send_consumer_control(struct usb_endpoint *const endpoint, const uint16_t consumer_control) {
    struct usb_consumer_control_report report = { 0x02, consumer_control };

    // send
    usb_send_consumer_control_report(endpoint, &report);

    // release
    report.consumer_control = 0x0000;
    usb_send_consumer_control_report(endpoint, &report);
}

void usb_send_gamepad_report(struct usb_endpoint *const endpoint, const struct usb_gamepad_report *const report) {
    usb_xfer(endpoint, (uint8_t*)report, sizeof(struct usb_gamepad_report));
}

uint hid_interface_cpy(uint8_t *buffer, const struct usb_hid_interface *const hid_interface) {
    memcpy((void *)buffer, hid_interface->interface_descriptor, hid_interface->interface_descriptor->bLength);
    uint offset = hid_interface->interface_descriptor->bLength;

    memcpy((void *)buffer + offset, hid_interface->hid_descriptor, hid_interface->hid_descriptor->bLength);
    offset += hid_interface->hid_descriptor->bLength;

    memcpy((void *)buffer + offset, hid_interface->endpoint->descriptor, hid_interface->endpoint->descriptor->bLength);
    offset += hid_interface->endpoint->descriptor->bLength;

    return offset;
}
