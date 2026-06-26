#include "zph02.h"
#include "Particle.h"

static const int ZPH02_PACKET_LEN = 9;
static const int ZPH02_TIMEOUT = 2000;

static bool _zph02Valid = false;
static uint16_t _zph02Pm25 = 0;
static unsigned long _lastPacket = 0;

static uint8_t _buf[9];
static int _bufPos = 0;

bool zph02Init() {
    Serial1.begin(9600);
    return true;
}

ZPH02Reading zph02Read() {
    while (Serial1.available()) {
        uint8_t b = Serial1.read();

        if (_bufPos == 0 && b != 0xFF) continue;

        _buf[_bufPos++] = b;

        if (_bufPos == ZPH02_PACKET_LEN) {
            uint8_t sum = 0;
            for (int i = 0; i < ZPH02_PACKET_LEN - 1; i++) {
                sum += _buf[i];
            }
            uint8_t chk = 0xFF - (sum & 0xFF);

            if (_buf[0] == 0xFF && _buf[1] == 0x18 && chk == _buf[8]) {
                _zph02Pm25 = ((uint16_t)_buf[2] << 8) | _buf[3];
                _zph02Valid = true;
                _lastPacket = millis();
            }

            _bufPos = 0;
        }
    }

    if (_zph02Valid && millis() - _lastPacket > ZPH02_TIMEOUT) {
        _zph02Valid = false;
    }

    return { _zph02Valid, _zph02Pm25 };
}
