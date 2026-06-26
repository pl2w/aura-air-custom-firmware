#include "fan.h"
#include "../config.h"
#include "Particle.h"

volatile uint16_t Fan::_pulseCount = 0;

void Fan::init() {
    pinMode(PIN_FAN_PWM, OUTPUT);
    pinMode(PIN_FAN_FG, INPUT_PULLUP);
    attachInterrupt(PIN_FAN_FG, _isr, FALLING);
    _enabled = true;
}

void Fan::setSpeed(uint8_t pct) {
    if (pct > 100) pct = 100;
    _speedPct = pct;

    if (!_enabled) return;  // just save, don't spin

    // PWM is inverted: 0 = full speed, 80 = stopped
    // Quadratic curve compensates for logarithmic RPM response
    // 100% speed -> PWM 0, 50% speed -> PWM 20, 0% speed -> PWM 80
    uint8_t pwm = (uint32_t)(100 - pct) * (100 - pct) * 80 / 10000;
    analogWrite(PIN_FAN_PWM, pwm);
}

uint8_t Fan::getSpeed() const {
    return _speedPct;
}

uint16_t Fan::getRPM() const {
    return _rpm;
}

void Fan::tick() {
    unsigned long now = millis();
    unsigned long dt = now - _lastTick;
    _lastTick = now;

    uint16_t pulses = __atomic_exchange_n(&_pulseCount, 0, __ATOMIC_RELAXED);

    if (dt > 0) {
        _rpm = ((uint32_t)pulses * 30000UL) / dt;
    }
}

void Fan::setEnabled(bool enabled) {
    if (enabled) {
        _enabled = true;
        uint8_t target = _speedPct > 0 ? _speedPct : _lastSpeed;
        _lastSpeed = target;
        setSpeed(target);  // writes PWM since _enabled is true
    } else {
        _lastSpeed = _speedPct > 0 ? _speedPct : 40;
        _enabled = false;
        _speedPct = 0;
        analogWrite(PIN_FAN_PWM, 80);  // stop
    }
}

void Fan::_isr() {
    __atomic_fetch_add(&_pulseCount, 1, __ATOMIC_RELAXED);
}