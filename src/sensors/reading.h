#pragma once
#include <stddef.h>
#include "hdc1080.h"
#include "sgp30.h"
#include "zph02.h"

struct SensorReading {
    HDC1080Reading hdc;
    SGP30Reading sgp;
    uint16_t fanRpm;
    bool coverOpen;
    uint8_t fanSpeedPct;
    bool uvcOn;
    bool ionizerOn;
    ZPH02Reading zph;
};

struct SensorMeta {
    const char* component;
    const char* slug;     
    const char* name;     
    const char* config;   
};

static const SensorMeta SENSOR_DISCOVERY[] = {
    {"sensor",       "temp",     "Temperature", "\"device_class\":\"temperature\",\"unit_of_measurement\":\"°C\",\"icon\":\"mdi:thermometer\",\"value_template\":\"{{value_json.temperature}}\""},
    {"sensor",       "hum",      "Humidity",    "\"device_class\":\"humidity\",\"unit_of_measurement\":\"%\",\"icon\":\"mdi:water-percent\",\"value_template\":\"{{value_json.humidity}}\""},
    {"sensor",       "eco2",     "eCO2",        "\"device_class\":\"carbon_dioxide\",\"unit_of_measurement\":\"ppm\",\"icon\":\"mdi:molecule-co2\",\"value_template\":\"{{value_json.eco2}}\""},
    {"sensor",       "tvoc",     "TVOC",        "\"unit_of_measurement\":\"ppb\",\"icon\":\"mdi:air-filter\",\"value_template\":\"{{value_json.tvoc}}\""},
    {"sensor",       "fan_rpm",  "Fan RPM",     "\"unit_of_measurement\":\"rpm\",\"icon\":\"mdi:fan\",\"value_template\":\"{{value_json.fan_rpm}}\""},
    {"binary_sensor","cover",    "Cover",       "\"device_class\":\"opening\",\"icon\":\"mdi:door-open\",\"value_template\":\"{% if value_json.cover_open %}ON{% else %}OFF{% endif %}\""},
    {"fan",          "fan",      "Air Purifier","\"command_topic\":\"aura/command/fan_power\",\"percentage_command_topic\":\"aura/command/fan_speed\",\"percentage_state_topic\":\"aura/sensors\",\"percentage_value_template\":\"{{value_json.fan_speed}}\",\"state_value_template\":\"{{value_json.fan_state}}\",\"payload_on\":\"ON\",\"payload_off\":\"OFF\",\"icon\":\"mdi:fan\""},
    {"switch",       "uvc",      "UVC LED",    "\"command_topic\":\"aura/command/uvc\",\"state_topic\":\"aura/sensors\",\"value_template\":\"{{value_json.uvc}}\",\"payload_on\":\"ON\",\"payload_off\":\"OFF\",\"icon\":\"mdi:led-outline\""},
    {"switch",       "ionizer",  "Ionizer",    "\"command_topic\":\"aura/command/ionizer\",\"state_topic\":\"aura/sensors\",\"value_template\":\"{{value_json.ionizer}}\",\"payload_on\":\"ON\",\"payload_off\":\"OFF\",\"icon\":\"mdi:plus-circle-outline\""},
    {"sensor",       "pm25",     "PM2.5",      "\"device_class\":\"pm25\",\"unit_of_measurement\":\"µg/m³\",\"icon\":\"mdi:blur\",\"value_template\":\"{{value_json.pm25}}\""},
};

static const size_t SENSOR_DISCOVERY_COUNT = sizeof(SENSOR_DISCOVERY) / sizeof(SENSOR_DISCOVERY[0]);