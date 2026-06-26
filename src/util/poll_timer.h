#pragma once

#include "Particle.h"

class PollTimer {
public:
    PollTimer(unsigned long interval) : _interval(interval), _last(0) {}
    bool ready() {
        unsigned long now = millis();
        if (now - _last >= _interval) {
            _last = now;
            return true;
        }
        return false;
    }
private:
    unsigned long _interval;
    unsigned long _last;
};