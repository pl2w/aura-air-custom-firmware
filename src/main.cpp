#include "Particle.h"
#include "fan/fan.h"
#include "sensors/hdc1080.h"
#include "sensors/sgp30.h"
#include "sensors/cover.h"
#include "sensors/me2co.h"
#include "config.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

Fan fan;
SGP30 sgp30;

Timer fanTimer(1000, []() { fan.tick(); });

void setup() {
    fan.init();
    fanTimer.start();
    
    coverInit();

    if (!hdc1080Init()) {
        Log.warn("HDC1080 not found");
    }

    if (!sgp30.init()) {
        Log.warn("SGP30 not found");
    }

    me2coInit();
}

void loop() {
    auto cover = coverRead();
    fan.setEnabled(cover.closed);

    static unsigned long lastLog = 0;
    if (millis() - lastLog >= 5000) {
        lastLog = millis();
        auto hdc = hdc1080Read();
        if (hdc.valid) {
            sgp30.setHumidity(hdc.humidity, hdc.temperature);
        }
        auto sgp = sgp30.read();
        auto co  = me2coRead();
        if (hdc.valid && sgp.valid) {
            Log.info("Fan: %u RPM | %.1f°C %.0f%% RH | %sTVOC %u eCO2 %u | CO %.2fV | Cover: %s",
                fan.getRPM(), hdc.temperature, hdc.humidity,
                sgp30.isWarmedUp() ? "" : "(warming) ",
                sgp.tvoc, sgp.eco2, co.voltage,
                cover.closed ? "closed" : "OPEN");
        } else if (hdc.valid) {
            Log.info("Fan: %u RPM | %.1f°C %.0f%% RH | Cover: %s",
                fan.getRPM(), hdc.temperature, hdc.humidity,
                cover.closed ? "closed" : "OPEN");
        } else {
            Log.warn("HDC1080 read failed");
        }
    }

    static unsigned long lastBaselineSave = 0;
    static bool baselineInitialized = false;
    if (sgp30.isWarmedUp()) {
        if (!baselineInitialized) {
            baselineInitialized = true;
            lastBaselineSave = millis();
        } else if (millis() - lastBaselineSave >= 3600000) {
            lastBaselineSave = millis();
            auto baseline = sgp30.getBaseline();
            if (baseline.valid) {
                EEPROM.put(EEPROM_ADDR_MAGIC, (uint16_t)0xBEEF);
                EEPROM.put(EEPROM_ADDR_ECO2, baseline.eco2);
                EEPROM.put(EEPROM_ADDR_TVOC, baseline.tvoc);
                Log.info("SGP30 baseline saved: eCO2=%u TVOC=%u", baseline.eco2, baseline.tvoc);
            }
        }
    }

    delay(1000);
}