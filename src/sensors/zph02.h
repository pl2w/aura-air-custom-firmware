#pragma once

#include <stdint.h>

struct ZPH02Reading {
    bool valid;
    uint16_t pm25;  // ug/m3
};

bool zph02Init();
ZPH02Reading zph02Read();
