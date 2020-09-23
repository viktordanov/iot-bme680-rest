#ifndef LEDS_H__
#define LEDS_H__

#include "Ticker.h"

#define SUCCESS_LED_PIN D6
#define WARNING_LED_PIN D7
#define ERROR_LED_PIN D8

#define LED_UPDATE_RATE_MS 50

bool SUCCESS_LED, WARNING_LED, ERROR_LED;
bool success_set, warning_set, error_set;
uint16 success_timer = 0, warning_timer = 0, error_timer = 0;

void led_updater();

Ticker led_ticker(led_updater, LED_UPDATE_RATE_MS, 0, MILLIS);

// Called once every LED_UPDATE_RATE_MS (50ms)
void led_updater()
{
  if (SUCCESS_LED)
  {
    if (!success_set)
    {
      digitalWrite(SUCCESS_LED_PIN, HIGH);
      success_set = true;
    }
    // if (success_set && success_timer < 1000 / LED_UPDATE_RATE_MS)
    // {
    //   success_timer++;
    // }
    // else if (success_set && success_timer >= 1000 / LED_UPDATE_RATE_MS)
    // {
    //   SUCCESS_LED = false;
    //   success_set = false;
    //   success_timer = 0;
    //   digitalWrite(SUCCESS_LED_PIN, LOW);
    // }
  }
  if (!SUCCESS_LED && success_set)
  {
    SUCCESS_LED = false;
    success_set = false;
    success_timer = 0;
    digitalWrite(SUCCESS_LED_PIN, LOW);
  }

  if (WARNING_LED)
  {
    if (!warning_set)
    {
      digitalWrite(WARNING_LED_PIN, HIGH);
      warning_set = true;
    }
    // if (warning_set && warning_timer < 1000 / LED_UPDATE_RATE_MS)
    // {
    //   warning_timer++;
    // }
    // else if (warning_set && warning_timer >= 1000 / LED_UPDATE_RATE_MS)
    // {
    //   WARNING_LED = false;
    //   warning_set = false;
    //   warning_timer = 0;
    //   digitalWrite(WARNING_LED_PIN, LOW);
    // }
  }
  if (!WARNING_LED && warning_set)
  {
    WARNING_LED = false;
    warning_set = false;
    warning_timer = 0;
    digitalWrite(WARNING_LED_PIN, LOW);
  }

  if (ERROR_LED)
  {
    if (!error_set)
    {
      digitalWrite(ERROR_LED_PIN, HIGH);
      error_set = true;
    }
    // if (error_set && error_timer < 1000 / LED_UPDATE_RATE_MS)
    // {
    //   error_timer++;
    // }
    // else if (error_set && error_timer >= 1000 / LED_UPDATE_RATE_MS)
    // {
    //   ERROR_LED = false;
    //   error_set = false;
    //   error_timer = 0;
    //   digitalWrite(ERROR_LED_PIN, LOW);
    // }
  }
  if (!ERROR_LED && error_set)
  {
    ERROR_LED = false;
    error_set = false;
    error_timer = 0;
    digitalWrite(ERROR_LED_PIN, LOW);
  }
}

void init_leds()
{
  pinMode(SUCCESS_LED_PIN, OUTPUT);
  pinMode(WARNING_LED_PIN, OUTPUT);
  pinMode(ERROR_LED_PIN, OUTPUT);
  led_ticker.start();
}

void tick_leds()
{
  led_ticker.update();
}

#endif