#pragma once

#include "Particle.h"
#include "secrets.h"

const uint8_t PIN_I2C_SDA    = D0;
const uint8_t PIN_I2C_SCL    = D1;
const uint8_t PIN_ZPH02_RX   = D2;
const uint8_t PIN_ZPH02_TX   = D3;
const uint8_t PIN_CO_SENSOR  = A2;
const uint8_t PIN_IONIZER    = D2;  
const uint8_t PIN_UVC_LED    = 25;
const uint8_t PIN_COVER_SW   = 28;  
const uint8_t PIN_FAN_PWM    = 24;  
const uint8_t PIN_FAN_FG     = 27;  

const uint8_t HDC1080_ADDR   = 0x40;
const uint8_t SGP30_ADDR     = 0x58;

const uint16_t EEPROM_ADDR_MAGIC = 0;
const uint16_t EEPROM_ADDR_ECO2  = 2;
const uint16_t EEPROM_ADDR_TVOC  = 4;

#define MQTT_BROKER   SECRET_MQTT_BROKER
#define MQTT_PORT     SECRET_MQTT_PORT
#define MQTT_USERNAME SECRET_MQTT_USERNAME
#define MQTT_PASSWORD SECRET_MQTT_PASSWORD