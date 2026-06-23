#include "Particle.h"
#include "config.h"
#include "mqtt/mqtt_handler.h"
#include "fan/fan.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

void setup() {
    fan_init();

    pinMode(PIN_UVC_LED, OUTPUT);
    pinMode(PIN_COVER_SWITCH, INPUT_PULLUP);

    digitalWrite(PIN_UVC_LED, LOW);

    Wire.begin();               // i2c hdc1080 sgp30
    Serial1.begin(9600);        // zph02 pm2.5

    waitUntil(WiFi.ready);

    mqtt_init();
}

void loop() {
    mqtt_loop();

    String topic, payload;
    if (mqtt_get_command(topic, payload)) {
        if (topic == "aura/fan/mode") {
            fan_set_mode((FanMode)payload.toInt());
        } else if (topic == "aura/fan/speed") {
            fan_set_speed(payload.toInt());
        }
    }

    static unsigned long lastRpm = 0;
    if (millis() - lastRpm > 5000) {
        lastRpm = millis();
        mqtt_publish_status("aura/status/fan_rpm", (int)fan_get_rpm());
    }
}