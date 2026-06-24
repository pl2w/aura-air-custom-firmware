#pragma once

#include <stdint.h>

struct SGP30Reading {
    uint16_t tvoc; // total volatile organic compounds (ppb 0-60k)
    uint16_t eco2; // co2 equivalent (ppm 400-60k)
    bool valid;
};

struct SGP30Baseline {
    uint16_t eco2;
    uint16_t tvoc;
    bool valid;
};

class SGP30 {
public:
    bool init();
    SGP30Reading read();
    void setHumidity(float rh, float tempC);
    SGP30Baseline getBaseline();
    bool setBaseline(uint16_t eco2, uint16_t tvoc);
    bool isWarmedUp() const;

private:
    bool _loadBaseline();
    bool _sendCmd(uint16_t cmd);

    bool _warmedUp = false;
    unsigned long _startTime = 0;
    unsigned long _lastSave = 0;
};
