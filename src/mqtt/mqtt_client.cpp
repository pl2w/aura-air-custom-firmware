#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../config.h"
#include "MQTT.h"
#include "mqtt_client.h"
#include "../sensors/reading.h"
#include "Particle.h"

static void _onMessage(char* topic, uint8_t* payload, unsigned int length);
static MQTT _client(MQTT_BROKER, MQTT_PORT, 768, _onMessage);

void MQTTClient::init() {
    _connect();
}

void MQTTClient::loop() {
    if (_client.isConnected()) {
        _client.loop();
    } else {
        _connect();
    }
}

void MQTTClient::publish(const char* topic, const char* payload) {
    if (_client.isConnected()) {
        _client.publish(topic, payload);
    }
}

void MQTTClient::publishSensors(const SensorReading& r) {
    if (!_client.isConnected()) return;
    
    char payload[256];
    snprintf(payload, sizeof(payload),
        "{\"temperature\":%.2f,\"humidity\":%.2f,\"eco2\":%d,\"tvoc\":%d,\"fan_rpm\":%u,\"fan_speed\":%u,\"fan_state\":\"%s\",\"cover_open\":%s,\"uvc\":\"%s\",\"ionizer\":\"%s\"}",
        r.hdc.valid ? r.hdc.temperature : -1,
        r.hdc.valid ? r.hdc.humidity : -1,
        r.sgp.valid ? r.sgp.eco2 : -1,
        r.sgp.valid ? r.sgp.tvoc : -1,
        r.fanRpm,
        r.fanSpeedPct,
        r.fanSpeedPct > 0 ? "ON" : "OFF",
        r.coverOpen ? "true" : "false",
        r.uvcOn ? "ON" : "OFF",
        r.ionizerOn ? "ON" : "OFF");
    
    _client.publish("aura/sensors", payload);
}

void MQTTClient::_connect() {
    if (!WiFi.ready()) {
        Log.info("MQTT connect skipped: WiFi not ready");
        return;
    }

    String id = System.deviceID();
    Log.info("MQTT connecting to %s:%d as '%s' with id '%s'",
        MQTT_BROKER, MQTT_PORT, MQTT_USERNAME, id.c_str());

    if (_client.connect(id, MQTT_USERNAME, MQTT_PASSWORD)) {
        Log.info("MQTT connected");
        _client.subscribe("aura/command/fan_speed");
        _client.subscribe("aura/command/fan_power");
        _client.subscribe("aura/command/uvc");
        _client.subscribe("aura/command/ionizer");
        _publishDiscovery();
    } else {
        Log.info("MQTT connection failed");
        Log.info("  broker reachable? ping %s", MQTT_BROKER);
        Log.info("  isConnected: %d", _client.isConnected());
    }
}

void MQTTClient::_publishSensor(const char* component, const char* slug, const char* name, const char* config) {
    String id = System.deviceID();
    char topic[128];
    char payload[768];
    char dev[160];
    snprintf(dev, sizeof(dev),
        "{\"identifiers\":[\"%s\"],\"name\":\"Air Purifier\",\"model\":\"Air\",\"sw_version\":\"1.0\",\"manufacturer\":\"Aura\"}",
        id.c_str());
    snprintf(topic, sizeof(topic), "homeassistant/%s/aura_%s/%s/config", component, id.c_str(), slug);
    snprintf(payload, sizeof(payload),
        "{\"name\":\"%s\",\"state_topic\":\"aura/sensors\",%s,\"unique_id\":\"aura_%s_%s\",\"device\":%s}",
        name, config, slug, id.c_str(), dev);
    _client.publish(topic, payload, true);
}

void MQTTClient::_publishDiscovery() {
    for (auto& s : SENSOR_DISCOVERY) {
        _publishSensor(s.component, s.slug, s.name, s.config);
    }
}

static void (*_fanSpeedHandler)(uint8_t) = nullptr;
static void (*_fanPowerHandler)(bool) = nullptr;
static void (*_uvcHandler)(bool) = nullptr;
static void (*_ionizerHandler)(bool) = nullptr;

void MQTTClient::onFanSpeed(void (*cb)(uint8_t)) {
    _fanSpeedHandler = cb;
}

void MQTTClient::onFanPower(void (*cb)(bool)) {
    _fanPowerHandler = cb;
}

void MQTTClient::onUvcCommand(void (*cb)(bool)) {
    _uvcHandler = cb;
}

void MQTTClient::onIonizerCommand(void (*cb)(bool)) {
    _ionizerHandler = cb;
}

void _onMessage(char* topic, uint8_t* payload, unsigned int length) {
    if (strcmp(topic, "aura/command/fan_speed") == 0) {
        char msg[length + 1];
        memcpy(msg, payload, length);
        msg[length] = '\0';
        int speed = atoi(msg);
        if (speed >= 0 && speed <= 100 && _fanSpeedHandler) {
            _fanSpeedHandler((uint8_t)speed);
        }
        return;
    }
    if (strcmp(topic, "aura/command/fan_power") == 0) {
        if (_fanPowerHandler) {
            _fanPowerHandler(length == 2 && payload[0] == 'O' && payload[1] == 'N');
        }
        return;
    }
    if (strcmp(topic, "aura/command/uvc") == 0) {
        if (_uvcHandler) {
            _uvcHandler(length == 2 && payload[0] == 'O' && payload[1] == 'N');
        }
        return;
    }
    if (strcmp(topic, "aura/command/ionizer") == 0) {
        if (_ionizerHandler) {
            _ionizerHandler(length == 2 && payload[0] == 'O' && payload[1] == 'N');
        }
        return;
    }
}