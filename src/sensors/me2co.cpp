#include "me2co.h"
#include "../config.h"
#include "Particle.h"

bool me2coInit() {
    pinMode(PIN_CO_SENSOR, INPUT);
    return true;
}

ME2COReading me2coRead() {
    ME2COReading r = {};

    r.raw = analogRead(PIN_CO_SENSOR);
    r.voltage = r.raw * 3.3f / 4095.0f;
    r.valid = true;

    return r;
}
