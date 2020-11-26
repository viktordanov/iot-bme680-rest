#include <Arduino.h>

#include "webserver.h"
#include "sensor.h"
#include "temperature.h"
#include "leds.h"

void remeasure()
{
  get_measurement();
}

Ticker periodical_check(remeasure, 1000 * 60 * 10, 0, MILLIS);

void setup()
{
  Serial.begin(115200);
  init_temperature();
  init_leds();
  init_sensor();
  init_webserver();
 
  periodical_check.start();
}

void loop()
{
  tick_leds();
  tick_temp_sensor();
  tick_sensor_debouncer();
  periodical_check.update();
}