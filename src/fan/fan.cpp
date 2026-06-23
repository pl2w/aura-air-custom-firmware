#include "fan.h"

static int currentSpeed = 255;

static volatile unsigned long tachPulses = 0;

static void tachISR() {
    tachPulses++;
}

void fan_init() {
    pinMode(24, OUTPUT);        // fan pwm
    pinMode(27, INPUT_PULLUP);  // fg wire

    fan_set_speed(255);  // set fan pwm to off

    attachInterrupt(27, tachISR, FALLING);
}

void fan_set_speed(int speed) {
    if (speed >= 0 && speed <= 255) {
        analogWrite(24, speed);
        currentSpeed = speed;
    }
}

int fan_get_speed() {
    return currentSpeed;
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