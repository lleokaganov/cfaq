#!/bin/bash

clear

#pio system info | grep esptool
#ls -l /dev/ttyU*
#ls -l /dev/ttyA*

cd ~/.platformio/packages/tool-esptoolpy/
~/.platformio/penv/bin/python3.10 \
~/.platformio/packages/tool-esptoolpy/esptool.py \
--port /dev/ttyACM0 flash_id

cd ~/.platformio/packages/tool-esptoolpy/
~/.platformio/penv/bin/python3.10 \
~/.platformio/packages/tool-esptoolpy/esptool.py \
--port /dev/ttyUSB0 flash_id
