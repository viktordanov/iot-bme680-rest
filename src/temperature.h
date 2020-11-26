#ifndef TEMPERATURE_H__
#define TEMPERATURE_H__

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ticker.h>
#include "ArduinoJson.h"



#define TEMP_SENSOR_PIN D3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(TEMP_SENSOR_PIN);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature temp_sensors(&oneWire);
DeviceAddress sensor_addresses[3];

void printAddress(DeviceAddress);
void request_temperature();

Ticker temperature_ticker(request_temperature, 5000, 0, MILLIS);

float LAST_RECORDED_TEMPS[3] = {-127.0, -127.0, -127.0};
float TEMP_OFFSETS[3] = {-0.31, -0.19, 0};
bool TEMP_SENSOR_OFFLINE = true;

void tick_temp_sensor()
{
  temperature_ticker.update();
}

void init_temperature()
{
  temp_sensors.begin();
  Serial.print("Found ");
  Serial.print(temp_sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  for (uint8_t j = 0; j <= 2; ++j)
  {
    if (!temp_sensors.getAddress(sensor_addresses[j], j))
    {
      Serial.println("Unable to find address for Device " + j);
      return;
    }
  }
  for (uint8_t j = 0; j <= 2; ++j)
  {
    Serial.print("Device " + String(j) + " address: ");
    printAddress(sensor_addresses[j]);
    Serial.println();
  }

  TEMP_SENSOR_OFFLINE = false;
  temp_sensors.requestTemperatures();
  temp_sensors.setResolution(12);
  temperature_ticker.start();
}

void request_temperature()
{
  if (TEMP_SENSOR_OFFLINE)
    return;
  temp_sensors.requestTemperatures();
  for (uint8_t j = 0; j <= 2; ++j)
  {
    LAST_RECORDED_TEMPS[j] = temp_sensors.getTempC(sensor_addresses[j]) + TEMP_OFFSETS[j];
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16)
      Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

DynamicJsonDocument get_temperature()
{
  DynamicJsonDocument doc(82);
  for (uint8_t j = 0; j <= 2; ++j)
  {
    doc["tem" + String(j)] = String(LAST_RECORDED_TEMPS[j]);
  }
  return doc;
}

#endif