This is an automatic translation, may be incorrect in some places. See sources and examples!

# SimplePortal
Simple WiFi Manager for ESP8266 for setting WiFi Login-Parol and operating mode

> For the wider possibilities of setting up with a web use [gyverportal] (https://github.com/gyverlibs/gyverPortal) - a library of a web designer with a lot of capabilities

## compatibility
ESP8266, ESP32

## Content
- [installation] (# Install)
- [initialization] (#init)
- [use] (#usage)
- [Example] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** SimplePortal ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/simpleportal/archive/refs/heads/main.zip). Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="init"> </a>
## initialization
No

<a id="usage"> </a>
## Usage
`` `CPP
VOID Portalstart ();// Launch the portal
VOID Portalstop ();// Stop the portal
Bool Portaltick ();// Call in the cycle
VOID Portalrun (Uint32_T PRD = 60000);// blocking call
Byte portalstatus ();// Status: 1 Connect, 2 AP, 3 Local, 4 Exit, 5 Timeout

// Constants of Status
SP_ERROR // Error
Sp_submit // sent login-paralle
Sp_switch_ap // Signal to change in AP
Sp_switch_local // shift signal to Local
Sp_exit // click button output
Sp_timeout // Taimout came out
`` `

### storage of settings
A variable (structure) ** PortalCFG ** is available from the sketch:
- `char* ssid [32]` - login
- `char* pass [32]` - password
- `byte mode` - (1` wifi_sta`, 2 `wifi_ap`)
After sending data from the portal, you can pick up information from the structure and/or write it in EEPROM.

### How does it work
! [Demo] (/doc/demo.png)
- The library triggers the ** Softap ** mode, launches ** dnsServer ** and ** ESP8266WebServer **.
- Connect to the point, by default it is called *"ESP Config" *
- The configuration page will open (if you have not opened, go through *192.168.1.1 *).On the page you can
Configure the ESP operating mode, as well as configure the connection to the router (login-paralle).
- The * Submit * button sends the entered data on ESP.OutCranberries E press * submit * The config mode switches to 1 (wifi_sta).
- The library does not switch the final operating mode of ESP, the portal buttons * Switch ap * / * Switch local * simply give signals "in the sketch".
- The Exit button simply completes the portal.
- After pressing any button or output of the timeout, the portal closes, the server stops, Softap turns off.

<a id="EXAMPLE"> </a>
## Examples
### blocking
`` `CPP
#include <ESP8266WIFI.H>
#include <simpleportal.h>

VOID setup () {
  Serial.Begin (9600);
  Delay (3000);
  
  Portalrun ();// Launch with a timeout 60C
  // Portalrun (30000);// Launch with a custom timeout
  
  Serial.println (Portalstatus ());
  // Status: 0 Error, 1 Connect, 2 AP, 3 Local, 4 Exit, 5 Timeout
  
  if (portalstatus () == sp_submit) {
    Serial.println (Portalcfg.ssid);
    Serial.println (Portalcfg.pass);
    // Take the login-Parol
  }
}

VOID loop () {
}
`` `

### asynchronous
`` `CPP
#include <ESP8266WIFI.H>
#include <simpleportal.h>

VOID setup () {
  Serial.Begin (9600);

  // Launch the portal
  Portalstart ();
}

VOID loop () {
  // CALL in LOOP
  if (portalTick ()) {
    Serial.println (Portalstatus ());
    if (portalstatus () == sp_submit) {
      Serial.println (Portalcfg.ssid);
      Serial.println (Portalcfg.pass);
      // Take the login-Parol
    }

    // will work once under action
    // point will be automatically turned off
  }
}
`` `

<a id="versions"> </a>
## versions
- V1.0
- V1.1 - Compatibility with ESP32
- V1.2 - Bag is fixed

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!


When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code