# iot-bme680-rest

<img alt="An example version of the device" src="https://github.com/viktordanov/iot-bme680-rest/blob/master/nodemcuv1.jpg?raw=true" width="50%" center/>

## Note: This project is intended to run on a NodeMCUv1 (or a similar ESP8266 board) together with a Bosch BME680 sensor.

To compile, load the project in VS Code and install PlatformIO to install all dependencies, and compile and monitor.

```c++
// /src/webserver.h

...

// Line 15
AsyncWebServer server(80);

const char *ssid = "SSID"; // Change to match your network's settings
const char *password = "PASSWORD";

...

```
