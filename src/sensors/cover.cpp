#include "cover.h"
#include "../config.h"
#include "Particle.h"

bool coverInit() {
    pinMode(PIN_COVER_SW, INPUT_PULLUP);
    return true;
}

CoverState coverRead() {
    CoverState s;
    s.closed = (digitalRead(PIN_COVER_SW) == LOW);
    return s;
}
