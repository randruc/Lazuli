#!/bin/bash

# Upload the binary image to the target.

avrdude \
    -p m328p \
    -c atmelice_isp \
    -U lfuse:w:0x62:m \
    -U hfuse:w:0xd8:m \
    -U efuse:w:0xff:m \
    -U flash:w:Lazuli.hex
