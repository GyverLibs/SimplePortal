This is an automatic translation, may be incorrect in some places. See sources and examples!

# simpleportal
Simple WiFi manager for esp8266 to set WiFi login-password and work mode

> For more customization options from the web, use [GyverPortal](https://github.com/GyverLibs/GyverPortal) - a web interface builder library with tons of features

### Compatibility
esp8266, esp32

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **SimplePortal** and installed via the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download Library](https://github.com/GyverLibs/SimplePortal/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initializeation
Not

<a id="usage"></a>
## Usage
```cpp
void portalStart(); // start portal
void portalStop(); // stop the portal
bool portalTick(); // call in a loop
void portalRun(uint32_t prd = 60000); // blocking call
byte portalStatus(); // status: 1 connect, 2 ap, 3 local, 4 exit, 5 timeout

// status constants
SP_ERROR // error
SP_SUBMIT // login-password sent
SP_SWITCH_AP // AP switch signal
SP_SWITCH_LOCAL // signal to change in Local
SP_EXIT // exit button pressed
SP_TIMEOUT // timeout expired
```

### Storing settings
A variable (structure) **portalCfg** is available from the sketch:
- `char* SSID[32]` - login
- `char* pass[32]` - password
- `byte mode` - (1 `WIFI_STA`, 2 `WIFI_AP`)
After sending data from the portal, you can take information from the structure and / or write it to EEPROM.

### How does it work
![demo](/doc/demo.png)
- The library starts **SoftAP** mode, starts **DNSServer** and **ESP8266WebServer**.
- Connect to a hotspot, default name is *"ESP Config"*
- The configuration page will open (if not opened, go to *192.168.1.1*). On the page you can
configure the ESP operation mode, as well as configure the connection to the router (login-password).
- The *Submit* button sends the entered data to the ESP. After pressing *Submit*, the mode in the config switches to 1 (WIFI_STA).
- The library does not switch the final mode of operation of esp, the portal buttons *Switch AP* / *Switch Local* just give signals "to the sketch".
- The Exit button just exits the portal.
- After pressing any button or exiting the timeout, the portal is closed, the server is stopped, SoftAP is disconnected.

<a id="example"></a>
## Examples
### Blocking
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);
  delay(3000);
  
  portalRun(); // run with 60s timeout
  //portalRun(30000); // run with custom timeout
  
  Serial.println(portalStatus());
  // status: 0 error, 1connect, 2 ap, 3 local, 4 exit, 5 timeout
  
  if (portalStatus() == SP_SUBMIT) {
    Serial.println(portalCfg.SSID);
    Serial.println(portalCfg.pass);
    // get login-password
  }
}

void loop() {
}
```

### Asynchronous
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);

  // start the portal
  portalStart();
}

void loop() {
  // call in loop
  if (portalTick()) {
    Serial.println(portalStatus());
    if (portalStatus() == SP_SUBMIT) {
      Serial.println(portalCfg.SSID);
      Serial.println(portalCfg.pass);
      // get login-password
    }

    // will work once on action
    // dot will be automatically turned off
  }
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - compatible with ESP32

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!