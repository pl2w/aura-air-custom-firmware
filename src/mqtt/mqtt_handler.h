#include "Particle.h"

void mqtt_init();
void mqtt_loop();

bool mqtt_get_command(String& topic, String& payload);

void mqtt_publish_sensor(const char* topic, float value, const char* unit);
void mqtt_publish_status(const char* topic, int value);

bool mqtt_is_connected();