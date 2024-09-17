# pico-keypad

This project is a keyboard (and arcade stick) firmware for Raspberry Pi Pico using the pico SDK (C/C++).


This project contain :
* a simple ssd1331 oled display spi driver, just to display raw image
* a ws2812b (rgb led strip) spi driver
* a hw40 (rotary encoder) driver

## 📦 build

```sh
cd ~/Workspace/pico-keypad && ./deploy.sh
```

## 🗂️ ressources
* A Raspberry Pi Pico
* pico C/C++ SDK
* pi pico usb examples C/C++
* WireShark usbmon (very very useful)
* TinyUSB (as an example)

## todo
* add led (Caps-lock...) read USB RX
* rotary encoder using ints ?
* control xfer
* pio