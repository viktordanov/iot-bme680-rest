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

## API (WIP)
### /info 
#### TODO: Should provide basic information about the device

### /get
Provides data from the sensor

e.g
```jsonc
{
  "tem": 26.94,     // Temperature
  "pre": 1004.22,   // Pressure
  "hum": 57.107,    // Relative humidity
  "gas": 160.731,   // Gas resistance
  "alt": 75.45407,  // Approx. altitude
  "aqi": 92.91285,  // Air quality index (/src/sensor.h for more info)
  "hqp": 0.179129,  // Humidity quality part ↓
  "gqp": 0.75       // Gas res. quality part (both used for the calculation of aqi)
}
```

## TODO

- [x] Initial build
- [] Refactor API 
- [] Add more pics
- [] Add schematic of wiring w/ LEDs
