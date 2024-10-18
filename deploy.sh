#!/bin/bash

# required compilers
# sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
# sudo apt-get install build-essential
# sudo apt install g++

export PICO_KEYPAD_PATH=/home/boudiou/Workspace/pico-keypad

BUILD_PATH="$PICO_KEYPAD_PATH/build"
MAKEFILE_PATH="$BUILD_PATH/Makefile"
PICO_USB_DISK_PATH="/media/boudiou/RPI-RP2/"

if ! test -f $MAKEFILE_PATH; then
    echo "executing 'cmake'"
    cd $BUILD_PATH &&
    cmake ..
fi

if test -d $PICO_USB_DISK_PATH; then
    cd $BUILD_PATH &&
    make &&

    echo "COPYING UF2 to pi pico" &&
    cp $BUILD_PATH/pico-keypad.uf2 $PICO_USB_DISK_PATH;
else
    echo -e "\e[31mERROR: pico unmounted or unmounted as a mass storage device.\e[0m"
fi
