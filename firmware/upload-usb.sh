#!/bin/sh

echo "(Re)connect Pico with the BOOTSEL button pressed, and then press ENTER."
read

picotool load -f $1
picotool reboot
