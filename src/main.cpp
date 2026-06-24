#include "Particle.h"
#include "fan/fan.h"
#include "sensors/hdc1080.h"
#include "sensors/cover.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

Fan fan;

void setup() {
    fan.init();
    coverInit();

    if (hdc1080Init()) {
        Log.info("HDC1080 connected");
    } else {
        Log.warn("HDC1080 not found");
    }
}

void loop() {
    auto cover = coverRead();
    fan.setEnabled(cover.closed);
    fan.tick();

    static unsigned long lastLog = 0;
    if (millis() - lastLog >= 5000) {
        lastLog = millis();

        auto hdc = hdc1080Read();

        if (hdc.valid) {
            Log.info("Fan: %u RPM | %.1f°C %.0f%% RH | Cover: %s",
                fan.getRPM(), hdc.temperature, hdc.humidity,
                cover.closed ? "closed" : "OPEN");
        } else {
            Log.warn("HDC1080 read failed");
        }
    }

    delay(1000);
}