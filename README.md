# pico-keypad

This project is a keyboard (and arcade stick) firmware for Raspberry Pi Pico using the pico SDK (C/C++).


This project contain :
* a simple ssd1331 oled display spi driver, just to display raw image
* a ws2812b (rgb led strip) spi driver
* a hw40 (rotary encoder) driver

## 📦 build

```sh
cd /Pico/pico-keypad/build
```

```sh
cmake -D PICO_KEYPAD_PATH=/Pico/pico-keypad ../../
```

```sh
make pico-keypad
```

```sh
cp /Pico/pico-keypad/build/pico-keypad.uf2 /media/user/RPI-RP2/
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