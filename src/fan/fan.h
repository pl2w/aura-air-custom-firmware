#include "Particle.h"

enum FanMode {
    FAN_OFF    = 0,
    FAN_SLEEP  = 1,
    FAN_LOW    = 2,
    FAN_MEDIUM = 3,
    FAN_HIGH   = 4,
    FAN_TURBO  = 5,
    FAN_CUSTOM = 6,
};

constexpr int fanModeToPWM(FanMode mode) {
    switch (mode) {
        case FAN_OFF:    return 255;
        case FAN_SLEEP:  return 44;
        case FAN_LOW:    return 28;
        case FAN_MEDIUM: return 16;
        case FAN_HIGH:   return 6;
        case FAN_TURBO:  return 0;
        case FAN_CUSTOM: return 255;
    }
    return 255;
}

void fan_init();

void fan_set_mode(FanMode mode);
void fan_set_speed(int speed);

FanMode fan_get_mode();
int fan_get_speed();
float fan_get_rpm();