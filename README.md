# iot-bme680-rest

![An example version of the device](https://octodex.github.com/images/yaktocat.png)

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
