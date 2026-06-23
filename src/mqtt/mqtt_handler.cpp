#include "mqtt_handler.h"
#include "MQTT.h"

static String cmdTopic;
static String cmdPayload;
static bool cmdPending = false;

static unsigned long lastConnectAttempt = 0;

static void callback(char* topic, byte* payload, unsigned int length) {
    cmdTopic = String(topic);
    cmdPayload = String((char*)payload, length);
    cmdPending = true;
}

MQTT client("test.mosquitto.org", 1883, callback);

static void on_connected() {
    client.subscribe("aura/fan/mode");
    client.subscribe("aura/fan/speed");
    client.subscribe("aura/uvc/set");
    client.publish("aura/status", "online - " + System.deviceID());
    Log.info("MQTT connected");
}

static void try_connect() {
    if (client.isConnected()) return;
    client.connect("aura-" + System.deviceID());
    if (client.isConnected()) on_connected();
}

void mqtt_init() {
    try_connect();
}

void mqtt_loop() {
    if (client.isConnected()) {
        client.loop();
        return;
    }

    if (millis() - lastConnectAttempt > 5000) {
        lastConnectAttempt = millis();
        try_connect();
    }
}

bool mqtt_get_command(String& topic, String& payload) {
    if (!cmdPending) return false;
    topic = cmdTopic;
    payload = cmdPayload;
    cmdPending = false;
    return true;
}

void mqtt_publish_sensor(const char* topic, float value, const char* unit) {
    if (!client.isConnected()) return;
    char buf[32];
    snprintf(buf, sizeof(buf), "%.1f", value);
    client.publish(topic, buf);
}

void mqtt_publish_status(const char* topic, int value) {
    if (!client.isConnected()) return;
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", value);
    client.publish(topic, buf);
}

bool mqtt_is_connected() {
    return client.isConnected();
}