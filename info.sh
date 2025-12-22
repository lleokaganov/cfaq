#!/bin/bash

clear

find ~/.platformio -name xtensa-esp32-elf-size

~/.platformio/packages/toolchain-xtensa-esp32/bin/xtensa-esp32-elf-size -A .pio/build/esp32/src/sys/ARG.cpp.o

exit



#pio libdeps resolve

# grep -E '^\.(text|data|rodata)' .pio/build/esp32/firmware.map | sort -k3 -n | tail -20

find .pio/build/esp32 -name "*.o" -exec ls -lh {} \; | sort -k5 -h | tail -20

#pio run --target size-components

#grep -R "LittleFS" -n src lib include

exit

pio run -v > log.txt

