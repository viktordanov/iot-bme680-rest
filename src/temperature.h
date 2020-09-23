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
DallasTemperature temp_sensor(&oneWire);
DeviceAddress thermometerAddr;

void printAddress(DeviceAddress);
void request_temperature();

Ticker temperature_ticker(request_temperature, 5000, 0, MILLIS);

float LAST_RECORDED_TEMP = -127.0;
bool TEMP_SENSOR_OFFLINE = true;

void tick_temp_sensor()
{
  temperature_ticker.update();
}

void init_temperature()
{
  temp_sensor.begin();
  Serial.print("Found ");
  Serial.print(temp_sensor.getDeviceCount(), DEC);
  Serial.println(" devices.");
  if (!temp_sensor.getAddress(thermometerAddr, 0))
  {
    Serial.println("Unable to find address for Device 0");
    return;
  }
  Serial.print("Device 0 Address: ");
  printAddress(thermometerAddr);
  Serial.println();
  TEMP_SENSOR_OFFLINE = false;
  temp_sensor.requestTemperatures();
  temperature_ticker.start();
}

void request_temperature()
{
  if (TEMP_SENSOR_OFFLINE)
    return;
  Serial.print("Requesting temperatures...");
  temp_sensor.requestTemperatures();
  Serial.println("DONE");
  LAST_RECORDED_TEMP = temp_sensor.getTempC(thermometerAddr);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16)
      Serial.print("0x");
    Serial.print(deviceAddress[i], HEX);
  }
}

DynamicJsonDocument get_temperature()
{
  DynamicJsonDocument doc(48);
  doc["tem"] = String(LAST_RECORDED_TEMP);
  return doc;
}

#endif