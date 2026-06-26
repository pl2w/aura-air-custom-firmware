#include "ionizer.h"
#include "../config.h"
#include "Particle.h"

static bool _ionizerOn = false;

bool ionizerInit() {
    pinMode(PIN_IONIZER, OUTPUT);
    digitalWrite(PIN_IONIZER, LOW);
    _ionizerOn = false;
    return true;
}

void ionizerSet(bool on) {
    _ionizerOn = on;
    digitalWrite(PIN_IONIZER, on ? HIGH : LOW);
}

bool ionizerIsOn() {
    return _ionizerOn;
}
