#include "hdc1080.h"
#include "../config.h"
#include "Particle.h"

bool hdc1080Init() {
    Wire.begin();
    Wire.beginTransmission(HDC1080_ADDR);
    return Wire.endTransmission() == 0;
}

HDC1080Reading hdc1080Read() {
    HDC1080Reading r = {};

    Wire.beginTransmission(HDC1080_ADDR);
    Wire.write(0x00);
    if (Wire.endTransmission() != 0) return r;
    delay(15);

    Wire.requestFrom(HDC1080_ADDR, 4);
    if (Wire.available() < 4) return r;

    uint16_t rawTemp = Wire.read() << 8 | Wire.read();
    uint16_t rawHum  = Wire.read() << 8 | Wire.read();

    r.temperature = rawTemp * 165.0f / 65536.0f - 40.0f;
    r.humidity    = rawHum  * 100.0f / 65536.0f;
    r.valid = true;
    return r;
}
