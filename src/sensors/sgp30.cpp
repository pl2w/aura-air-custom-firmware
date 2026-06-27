#include "sgp30.h"
#include "../config.h"
#include "Particle.h"
#include <math.h>

// CRC-8 for Sensirion sensors (polynomial 0x31)
static uint8_t _crc8(const uint8_t* data, size_t len) {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else            crc <<= 1;
        }
    }
    return crc;
}

bool SGP30::_sendCmd(uint16_t cmd) {
    Wire.beginTransmission(SGP30_ADDR);
    Wire.write(cmd >> 8);
    Wire.write(cmd & 0xFF);
    return Wire.endTransmission() == 0;
}

bool SGP30::init() {
    if (!_sendCmd(0x2003)) return false;
    delay(10);
    // Read init response (2 bytes + CRC) to clear the sensor
    Wire.requestFrom(SGP30_ADDR, 3);
    while (Wire.available()) Wire.read();

    _startTime = millis();

    //uint16_t magic;
    //EEPROM.get(EEPROM_ADDR_MAGIC, magic);
    //if (magic == 0xBEEF) {
    //    uint16_t eco2base, tvocbase;
    //    EEPROM.get(EEPROM_ADDR_ECO2, eco2base);
    //    EEPROM.get(EEPROM_ADDR_TVOC, tvocbase);
    //    setBaseline(eco2base, tvocbase);
    //    Log.info("SGP30 baseline restored: eCO2=%u TVOC=%u", eco2base, tvocbase);
    //}

    return true;
}

void SGP30::setHumidity(float rh, float tempC) {
    float ah = (6.112f * expf((17.62f * tempC) / (243.12f + tempC)) * rh * 2.1674f)
               / (273.15f + tempC);

    uint8_t ah_int  = (uint8_t)ah;
    uint8_t ah_frac = (uint8_t)((ah - ah_int) * 256.0f);
    uint8_t buf[2]  = { ah_int, ah_frac };

    Wire.beginTransmission(SGP30_ADDR);
    Wire.write(0x20); Wire.write(0x61);
    Wire.write(ah_int);
    Wire.write(ah_frac);
    Wire.write(_crc8(buf, 2));
    Wire.endTransmission();
    delay(1);
}

SGP30Baseline SGP30::getBaseline() {
    SGP30Baseline b = {};
    if (!_sendCmd(0x2015)) return b;
    delay(10);
    Wire.requestFrom(SGP30_ADDR, 6);
    if (Wire.available() < 6) return b;
    uint8_t buf[6];
    for (int i = 0; i < 6; i++) buf[i] = Wire.read();
    if (_crc8(buf, 2) != buf[2]) return b;
    if (_crc8(buf + 3, 2) != buf[5]) return b;
    b.eco2 = (buf[0] << 8) | buf[1];
    b.tvoc = (buf[3] << 8) | buf[4];
    b.valid = true;
    return b;
}

bool SGP30::setBaseline(uint16_t eco2, uint16_t tvoc) {
    uint8_t eco2buf[2] = { (uint8_t)(eco2 >> 8), (uint8_t)(eco2 & 0xFF) };
    uint8_t tvocbuf[2] = { (uint8_t)(tvoc >> 8), (uint8_t)(tvoc & 0xFF) };
    Wire.beginTransmission(SGP30_ADDR);
    Wire.write(0x20); Wire.write(0x1e);
    Wire.write(tvocbuf[0]); Wire.write(tvocbuf[1]); Wire.write(_crc8(tvocbuf, 2));
    Wire.write(eco2buf[0]); Wire.write(eco2buf[1]); Wire.write(_crc8(eco2buf, 2));
    return Wire.endTransmission() == 0;
}

SGP30Reading SGP30::read() {
    SGP30Reading r = {};

    if (!_sendCmd(0x2008)) return r;
    delay(12);

    Wire.requestFrom(SGP30_ADDR, 6);
    if (Wire.available() < 6) return r;

    uint8_t buf[6];
    for (int i = 0; i < 6; i++) buf[i] = Wire.read();

    // Verify CRCs
    if (_crc8(buf, 2) != buf[2]) return r;
    if (_crc8(buf + 3, 2) != buf[5]) return r;

    r.eco2 = (buf[0] << 8) | buf[1];
    r.tvoc = (buf[3] << 8) | buf[4];
    r.valid = true;

    if (!_warmedUp && millis() - _startTime >= 15000) {
        _warmedUp = true;
    }

    return r;
}

bool SGP30::isWarmedUp() const {
    return _warmedUp;
}
