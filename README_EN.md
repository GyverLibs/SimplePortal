This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/SimplePortal.svg?color=brightgreen)](https://github.com/GyverLibs/SimplePortal/releases/latest/download/SimplePortal.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/SimplePortal.svg)](https://registry.platformio.org/libraries/gyverlibs/SimplePortal)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/SimplePortal?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# SimplePortal
Simple WiFi Manager for esp8266 for WiFi login and operating mode

> For more customization options from the web use[GyverPortal](https://github.com/GyverLibs/GyverPortal)Web interface designer library with a lot of possibilities

### Compatibility
esp8266, esp32

## Contents
- [Installation](#install)
- [Initialization](#init)
- [Use of use](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found under the name **SimplePortal** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/SimplePortal/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="init"></a>
## Initialization
No.

<a id="usage"></a>
## Use of use
```cpp
void portalStart();     // launch
void portalStop();      // stop
bool portalTick();      // cycle
void portalRun(uint32_t prd = 60000);   // blocker
byte portalStatus();    // status: 1 connect, 2 ap, 3 local, 4 exit, 5 timeout

// status constant
SP_ERROR            // mistake
SP_SUBMIT           // login password sent
SP_SWITCH_AP        // shift
SP_SWITCH_LOCAL     // switch-off
SP_EXIT             // exit button
SP_TIMEOUT          // timeout
```

### Storage of settings
From the sketch, the variable (structure) **portalCfg** is available:
- `char* SSID[32]`- login
- `char* pass[32]`- password
- `byte mode` - (1 `WIFI_STA`, 2 `WIFI_AP`)
After sending data from the portal, you can collect information from the structure and / or write it to EEPROM.

### How does it work?
![demo](/doc/demo.png)
- The library runs **SoftAP**, runs **DNSServer** and **ESP8266WebServer**.
- Connect to a point called *ESP Config by default *
- The configuration page will open (if not, go to *192.168.1.1*). On the page.
configure the ESP mode, as well as configure the connection to the router (login password).
- The *Submit* button sends the input data to the ESP. After pressing *Submit*, the mode in the config is switched to 1 (WIFI STA).
- The library does not switch the final mode of operation of esp, the buttons of the portal *Switch AP* / *Switch Local* simply give signals "in the sketch".
- The Exit button simply completes the portal.
- After pressing any button or timeout, the portal closes, the server stops, and SoftAP shuts down.

<a id="example"></a>
## Examples
### Blocking
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);
  delay(3000);
  
  portalRun();  // timeout 60c
  //portalRun(30000); // launch with custom timeout
  
  Serial.println(portalStatus());
  // status: 0 error, 1 connect, 2 ap, 3 local, 4 exit, 5 timeout
  
  if (portalStatus() == SP_SUBMIT) {
    Serial.println(portalCfg.SSID);
    Serial.println(portalCfg.pass);
    // take the login password
  }
}

void loop() {
}
```

### asynchronous
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);

  // launch a portal
  portalStart();
}

void loop() {
  // call in the loop
  if (portalTick()) {
    Serial.println(portalStatus());
    if (portalStatus() == SP_SUBMIT) {
      Serial.println(portalCfg.SSID);
      Serial.println(portalCfg.pass);
      // take the login password
    }

    // It will work once in action.
    // the point will be automatically turned off
  }
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 Compatibility with ESP32
- v1.2 - bug corrected

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
