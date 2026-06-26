#include "uvc_led.h"
#include "../config.h"
#include "Particle.h"

static bool _uvcOn = false;

bool uvcLedInit() {
    pinMode(PIN_UVC_LED, OUTPUT);
    digitalWrite(PIN_UVC_LED, LOW);
    _uvcOn = false;
    return true;
}

void uvcLedSet(bool on) {
    _uvcOn = on;
    digitalWrite(PIN_UVC_LED, on ? HIGH : LOW);
}

bool uvcLedIsOn() {
    return _uvcOn;
}
