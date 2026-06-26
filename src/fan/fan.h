#pragma once

#include <stdint.h>

class Fan {
public:
    void init();
    void setSpeed(uint8_t pct);
    void setEnabled(bool enabled);
    uint8_t getSpeed() const;
    uint16_t getRPM() const;
    void tick();

private:
    static void _isr();
    static volatile uint16_t _pulseCount;

    bool _enabled = false;
    uint8_t _speedPct = 0;
    uint8_t _lastSpeed = 40;
    uint16_t _rpm = 0;
    unsigned long _lastTick = 0;
};