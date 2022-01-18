/*
    Простой менеджер WiFi для esp8266 для задания логина-пароля WiFi и режима работы
    GitHub: https://github.com/GyverLibs/SimplePortal
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0
    v1.1 - совместимость с ESP32
*/

#define SP_AP_NAME "ESP Config"     // название точки
#define SP_AP_IP 192,168,1,1        // IP точки

#ifndef _SimplePortal_h
#define _SimplePortal_h
#include <DNSServer.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#else
#include <WiFi.h>
#include <WebServer.h>
#endif

static DNSServer _SP_dnsServer;
#ifdef ESP8266
static ESP8266WebServer _SP_server(80);
#else
static WebServer _SP_server(80);
#endif

#define SP_ERROR 0
#define SP_SUBMIT 1
#define SP_SWITCH_AP 2
#define SP_SWITCH_LOCAL 3
#define SP_EXIT 4
#define SP_TIMEOUT 5
#define SP_CUSTOM_COUNT 5
struct PortalCfg {
  char SSID[32] = "";
  char pass[32] = "";
  uint8_t mode = WIFI_AP;    // (1 WIFI_STA, 2 WIFI_AP)
};
PortalCfg portalCfg;

static bool _SP_started = false;
static byte _SP_status = 0;

#ifndef SP_CUSTOM_PAGE
const char SP_connect_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
</head><body>
<style type="text/css">
    input[type="text"] {margin-bottom:8px;font-size:20px;}
    input[type="submit"] {width:180px; height:60px;margin-bottom:8px;font-size:20px;}
</style>
<center>
<h3>WiFi settings</h3>
<form action="/connect" method="POST">
    <input type="text" name="ssid" placeholder="SSID">
    <input type="text" name="pass" placeholder="Pass">
    <input type="submit" value="Submit">
</form>
<h3>Switch WiFi mode</h3>
<form action="/ap" method="POST">
    <input type="submit" value="Access Point">
</form>
<form action="/local" method="POST">
    <input type="submit" value="Local Mode">
</form>
<form action="/exit" method="POST">
    <input type="submit" value="Exit Portal">
</form>
</center>
</body></html>)rawliteral";
#else
struct CustomField { char** storage; char* name; };
struct CustomFields {
  CustomField fields[SP_CUSTOM_COUNT];
  uint8_t count = 0;
};
const char* SP_connect_page;
CustomFields SP_customFields;
#endif

void SP_handleConnect() {
  strcpy(portalCfg.SSID, _SP_server.arg("ssid").c_str());
  strcpy(portalCfg.pass, _SP_server.arg("pass").c_str());
  #ifdef SP_CUSTOM_PAGE
  if (SP_customFields.count > 0) {
    for (uint8_t i = 0; i < SP_customFields.count; i++) {
      strcpy(*SP_customFields.fields[i].storage, _SP_server.arg(SP_customFields.fields[i].name).c_str());
    }
  }
  #endif
  portalCfg.mode = WIFI_STA;
  _SP_status = 1;
}

void SP_handleAP() {
  portalCfg.mode = WIFI_AP;
  _SP_status = 2;
}

void SP_handleLocal() {
  portalCfg.mode = WIFI_STA;
  _SP_status = 3;
}

void SP_handleExit() {
  _SP_status = 4;
}

 // запустить портал
void portalStart() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  IPAddress apIP(SP_AP_IP);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, subnet);
  WiFi.softAP(SP_AP_NAME);
  _SP_dnsServer.start(53, "*", apIP);

  _SP_server.onNotFound([]() {
    _SP_server.send(200, "text/html", SP_connect_page);
  });
  _SP_server.on("/connect", HTTP_POST, SP_handleConnect);
  _SP_server.on("/ap", HTTP_POST, SP_handleAP);
  _SP_server.on("/local", HTTP_POST, SP_handleLocal);
  _SP_server.on("/exit", HTTP_POST, SP_handleExit);
  _SP_server.begin();
  _SP_started = true;
  _SP_status = 0;
}

// остановить портал
void portalStop() {
  WiFi.softAPdisconnect();
  _SP_server.stop();
  _SP_dnsServer.stop();
  _SP_started = false;
}

// вызывать в цикле
bool portalTick() {
  if (_SP_started) {
    _SP_dnsServer.processNextRequest();
    _SP_server.handleClient();
    yield();
    if (_SP_status) {
      portalStop();
      return 1;
    }
  }
  return 0;
}

// блокирующий вызов
void portalRun(uint32_t prd) {
  uint32_t tmr = millis();
  portalStart();
  while (!portalTick()) {
    if (millis() - tmr > prd) {
      _SP_status = 5;
      portalStop();
      break;
    }
  }
}

// статус: 1 connect, 2 ap, 3 local, 4 exit, 5 timeout
byte portalStatus() {
  return _SP_status;
}    

#ifdef SP_CUSTOM_PAGE
void portalAddCustomField(char** storage, char* name) {
  if (SP_customFields.count == SP_CUSTOM_COUNT) {
    return;
  }

  CustomField field = { .storage = storage, .name = name };

  SP_customFields.fields[SP_customFields.count] = field;
  SP_customFields.count++;
}

void portalSetCustomPage(const char* page) {
  SP_connect_page = page;
}
#endif

#endif