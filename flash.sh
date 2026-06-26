#!/bin/bash

particle compile p1 --saveTo builds/firmware.bin
particle flash --usb builds/firmware.bin -v
