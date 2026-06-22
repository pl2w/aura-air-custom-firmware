#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

void setup() {
    pinMode(24, OUTPUT);        // fan pwm
    pinMode(25, OUTPUT);        // uvc led
    pinMode(28, INPUT_PULLUP);  // cover switch

    analogWrite(24, 255);       // set fan pwm to off
    digitalWrite(25, LOW);      // set uvc led to off

    Wire.begin();               // i2c hdc1080 sgp30
    Serial1.begin(9600);        // zph02 pm2.5
}

void loop() {
}  