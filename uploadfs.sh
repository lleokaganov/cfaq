#!/bin/bash

clear
ls -l /dev/ttyU*
ls -l /dev/ttyA*

echo "-----------------------------"

~/.platformio/penv/bin/pio run --target uploadfs
