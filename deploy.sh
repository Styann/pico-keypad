#!/bin/bash

BUILD_PATH="/Pico/pico-keypad/build"
PICO_DISK_PATH="/media/boudiou/RPI-RP2/"

if test -d $PICO_DISK_PATH; then
    cd $BUILD_PATH &&
    make &&
    cp $BUILD_PATH/pico-keypad.uf2 $PICO_DISK_PATH
else
    echo -e "\e[31mERROR: pico unmounted or unmounted as a mass storage device.\e[0m"
fi
