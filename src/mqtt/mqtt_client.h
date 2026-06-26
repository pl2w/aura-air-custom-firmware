#pragma once

#include <stdint.h>
#include "sensors/reading.h"

class MQTTClient {
public:
    void init();
    void loop();
    void publish(const char* topic, const char* payload);
    void publishSensors(const SensorReading& r);
    void onFanSpeed(void (*callback)(uint8_t speed));
    void onFanPower(void (*callback)(bool on));
    void onUvcCommand(void (*callback)(bool on));

private:
    static void _callback(char* topic, uint8_t* payload, unsigned int len);
    void _connect();
    void _publishDiscovery();
    void _publishSensor(const char* component, const char* slug, const char* name, const char* config);
};
