#ifndef WEBSERVER_H__
#define WEBSERVER_H__

#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include "ArduinoJson.h"

#include "leds.h"
#include "sensor.h"

AsyncWebServer server(80);

const char *ssid = "SSID";
const char *password = "PASSWORD";

const char *PARAM_MESSAGE = "";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void init_webserver()
{
  WARNING_LED = true;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    ERROR_LED = true;
    Serial.printf("WiFi Failed!\n");
    return;
  }
  WARNING_LED = false;
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world");
  });

  // Send a GET request to <IP>/get?message=<message>
  server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
    String message = "info";
    request->send(200, "text/plain", "Hello, GET: " + message);
  });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String message;
    serializeJson(get_measurement(), message);
    request->send(200, "application/json", message);
  });

  server.onNotFound(notFound);
  server.begin();
}

#endif
