#include "Particle.h"
#include "util/poll_timer.h"
#include "fan/fan.h"
#include "sensors/hdc1080.h"
#include "sensors/sgp30.h"
#include "sensors/cover.h"
#include "sensors/reading.h"
#include "mqtt/mqtt_client.h"
#include "led/status_led.h"
#include "led/uvc_led.h"
#include "config.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

PollTimer sensorTimer(1000);
PollTimer baselineTimer(3600000);

Fan fan;
SGP30 sgp30;
MQTTClient mqttClient;

void onFanSpeedCommand(uint8_t speed) {
    fan.setSpeed(speed);
}

void onFanPowerCommand(bool on) {
    fan.setEnabled(on);
}

void onUvcCommand(bool on) {
    uvcLedSet(on);
}

void setup() {
    hdc1080Init();
    sgp30.init();
    coverInit();
    uvcLedInit();

    fan.init();
    fan.setSpeed(40);

    mqttClient.onFanSpeed(onFanSpeedCommand);
    mqttClient.onFanPower(onFanPowerCommand);
    mqttClient.onUvcCommand(onUvcCommand);
    mqttClient.init();
}

void loop() {
    if (sensorTimer.ready()) {
        fan.tick();

        SensorReading r;

        r.hdc = hdc1080Read();
        if (r.hdc.valid) {
            sgp30.setHumidity(r.hdc.humidity, r.hdc.temperature);
        }

        r.sgp = sgp30.read();
        r.coverOpen = coverRead();
        r.uvcOn = uvcLedIsOn();
        r.fanRpm = fan.getRPM();
        r.fanSpeedPct = fan.getSpeed();

        if (r.hdc.valid) {
            Log.info("Temp: %.2f C, Hum: %.2f %%", r.hdc.temperature, r.hdc.humidity);
        }

        if (r.sgp.valid && sgp30.isWarmedUp()) {
            Log.info("eCO2: %u ppm, TVOC: %u ppb", r.sgp.eco2, r.sgp.tvoc);
        }

        Log.info("Fan RPM: %u, Cover: %s", r.fanRpm, r.coverOpen ? "open" : "closed");

        mqttClient.publishSensors(r);
    }

    if (baselineTimer.ready() && sgp30.isWarmedUp()) {
        auto baseline = sgp30.getBaseline();
        if (baseline.valid) {
            EEPROM.put(EEPROM_ADDR_MAGIC, (uint16_t)0xBEEF);
            EEPROM.put(EEPROM_ADDR_ECO2, baseline.eco2);
            EEPROM.put(EEPROM_ADDR_TVOC, baseline.tvoc);
            Log.info("SGP30 baseline saved: eCO2=%u TVOC=%u", baseline.eco2, baseline.tvoc);
        }
    }

    mqttClient.loop();
}