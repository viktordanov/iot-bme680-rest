#ifndef SENSOR_H__
#define SENSOR_H__

#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

#include "Adafruit_BME680.h"
#include "ArduinoJson.h"
#include "temperature.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

// Allow execution when 0, set to 5 after running and decrease till 0
uint8 DEBOUNCER = 0;
void update_sensor_debouncer()
{
  if (DEBOUNCER != 0)
    DEBOUNCER--;
}
Ticker sensor_debouncer(update_sensor_debouncer, 500, 0, MILLIS);

Adafruit_BME680 bme; // I2C

float hum_weighting =
    0.25; // so hum effect is 25% of the total air quality score
float gas_weighting =
    0.75; // so gas effect is 75% of the total air quality score

float hum_score, gas_score;
float gas_reference = 250000;
float hum_reference = 40;
int getgasreference_count = 0;

void get_gas_reference();
String calculate_iaq(float score);

void init_sensor()
{
  if (!bme.begin())
  {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1)
      delay(10);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_16X);
  bme.setHumidityOversampling(BME680_OS_4X);
  bme.setPressureOversampling(BME680_OS_NONE);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_15);
  bme.setGasHeater(320, 150); // 320*C for 150 ms (was 150)
  get_gas_reference();
  DEBOUNCER = 15;
  sensor_debouncer.start();
}
void tick_sensor_debouncer()
{
  sensor_debouncer.update();
}

int i{};
int counter{0};

DynamicJsonDocument get_measurement()
{
  DynamicJsonDocument doc(512);
  if (DEBOUNCER != 0)
  {
    String a = "Sensor not yet ready. Wait ";
    a += (DEBOUNCER * 0.5);
    a += "s";
    doc["message"] = a;
    return doc;
  }
  DEBOUNCER = 10;
  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0)
  {
    doc["error"] = "Failed to begin reading";
    ERROR_LED = true;
    return doc;
  }

  delay(150); // This represents parallel work.
  if (!bme.endReading())
  {
    doc["error"] = "Failed to complete reading";
    ERROR_LED = true;
    return doc;
  }

  doc["tem"] = bme.temperature;

  float temp_sensors_avg = (LAST_RECORDED_TEMPS[0] + LAST_RECORDED_TEMPS[1] + LAST_RECORDED_TEMPS[2]) / 3;
  doc["tem_alt"] = temp_sensors_avg;
  doc["pre"] = bme.pressure / 100.0;
  doc["hum"] = bme.humidity;
  doc["gas"] = bme.gas_resistance / 1000.0;
  doc["alt"] = bme.readAltitude(SEALEVELPRESSURE_HPA);

  float current_humidity = bme.readHumidity();
  if (current_humidity >= 38 && current_humidity <= 42)
    hum_score = 0.25 * 100; // Humidity +/-5% around optimum
  else
  { // sub-optimal
    if (current_humidity < 38)
      hum_score = 0.25 / hum_reference * current_humidity * 100;
    else
    {
      hum_score =
          ((-0.25 / (100 - hum_reference) * current_humidity) + 0.416666) * 100;
    }
  }

  // Calculate gas contribution to IAQ index
  float gas_lower_limit = 5000;  // Bad air quality limit
  float gas_upper_limit = 50000; // Good air quality limit
  if (gas_reference > gas_upper_limit)
    gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit)
    gas_reference = gas_lower_limit;
  gas_score =
      (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference -
       (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) *
      100;

  // Combine results for the final IAQ index value (0-100% where 100% is good
  // quality air)
  float air_quality_score = hum_score + gas_score;

  doc["aqi"] = air_quality_score;
  doc["hqp"] = hum_score / 100;
  doc["gqp"] = gas_score / 100;

  // if (bme.readGas() < 120000)
  //   Serial.println("***** Poor air quality *****");
  if ((getgasreference_count++) % 500 == 0)
    get_gas_reference();
  // Serial.println(CalculateIAQ(air_quality_score));
  // Serial.println("------------------------------------------------");
  if (air_quality_score < 65)
  {
    SUCCESS_LED = false;
    WARNING_LED = false;
    ERROR_LED = true;
  }
  else if (air_quality_score < 85)
  {
    SUCCESS_LED = false;
    WARNING_LED = true;
    ERROR_LED = false;
  }
  else
  {
    SUCCESS_LED = true;
    WARNING_LED = false;
    ERROR_LED = false;
  }
  return doc;
}

void get_gas_reference()
{
  // Now run the sensor for a burn-in period, then use combination of relative
  // humidity and gas resistance to estimate indoor air quality as a percentage.
  // Serial.println("Getting a new gas reference value");
  int readings = 10;
  for (int i = 1; i <= readings; i++)
  { // read gas for 10 x 0.100ms = 1sec
    gas_reference += bme.readGas();
  }
  gas_reference = gas_reference / readings;
}

String calculate_iaq(float score)
{
  String IAQ_text = "Air quality is ";
  score = (100 - score) * 5;
  if (score >= 301)
    IAQ_text += "Hazardous";
  else if (score >= 201 && score <= 300)
    IAQ_text += "Very Unhealthy";
  else if (score >= 176 && score <= 200)
    IAQ_text += "Unhealthy";
  else if (score >= 151 && score <= 175)
    IAQ_text += "Unhealthy for Sensitive Groups";
  else if (score >= 51 && score <= 150)
    IAQ_text += "Moderate";
  else if (score >= 00 && score <= 50)
    IAQ_text += "Good";
  IAQ_text += " (score: " + static_cast<String>(score) + ")";
  return IAQ_text;
}

#endif