#!/bin/bash

clear

rm -f ./.pio

pio run -t clean

pio run

