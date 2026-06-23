#!/bin/bash

particle compile p1 --saveTo builds/firmware.bin
particle flash aura_air_purifier builds/firmware.bin
