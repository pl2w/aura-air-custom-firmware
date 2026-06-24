#pragma once

#include <stdint.h>

struct ME2COReading {
    uint16_t raw;       // 0–4095 ADC value
    float voltage;      // voltage on the pin
    bool valid;
};

bool me2coInit();
ME2COReading me2coRead();
