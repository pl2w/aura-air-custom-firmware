#include "cover.h"
#include "../config.h"
#include "Particle.h"

bool coverInit() {
    pinMode(PIN_COVER_SW, INPUT_PULLUP);
    return true;
}

bool coverRead() {
    return digitalRead(PIN_COVER_SW) == HIGH;
}
