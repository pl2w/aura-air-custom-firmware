#include "fan.h"

static int currentSpeed = 255;
static FanMode currentMode = FAN_OFF;

static volatile unsigned long tachPulses = 0;

static void tachISR() {
    tachPulses++;
}

void fan_init() {
    pinMode(24, OUTPUT);
    pinMode(27, INPUT_PULLUP);

    analogWrite(24, 255); // off
    attachInterrupt(27, tachISR, FALLING);

    Log.info("Fan initialized");
}

void fan_set_speed(int speed) {
    if (speed < 0) speed = 0;
    if (speed > 255) speed = 255;
    currentMode = FAN_CUSTOM;
    currentSpeed = speed;
    analogWrite(24, speed);
    Log.info("Fan custom: PWM=%d", speed);
}

void fan_set_mode(FanMode mode) {
    if (mode > FAN_CUSTOM) return;
    currentMode = mode;
    currentSpeed = fanModeToPWM(mode);
    analogWrite(24, currentSpeed);
    Log.info("Fan mode: %d (PWM=%d)", mode, currentSpeed);
}

int fan_get_speed() {
    return currentSpeed;
}

FanMode fan_get_mode() {
    return currentMode;
}

float fan_get_rpm() {
    static unsigned long lastTime = 0;
    unsigned long now = millis();
    unsigned long elapsed = now - lastTime;
    if (elapsed < 1000) return -1;

    noInterrupts();
    unsigned long count = tachPulses;
    tachPulses = 0;
    interrupts();

    float rpm = (count * 60000.0) / (elapsed * 2);
    lastTime = now;
    return rpm;
}