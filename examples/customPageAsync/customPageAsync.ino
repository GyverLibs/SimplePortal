#include <ESP8266WiFi.h>
// должно быть определено, чтобы установить кастомный режим
#define SP_CUSTOM_PAGE //чтобы эта фишка сработала: нужно весь код перенести в .h
#include <SimplePortal.h>



const char setup_page[] PROGMEM = R"rawliteral(
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
    <input type="text" name="devid" placeholder="device id">
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

char* device_id;

void setup() {
  Serial.begin(9600);
  device_id = (char*)malloc(32);
  // устанавливаем собственную страницу
  portalSetCustomPage(setup_page);
  // добавляем собственное поле, используемое в странице
  portalAddCustomField(&device_id, "devid");
  // запускаем портал
  portalStart();
}

void loop() {
  if (portalTick()) {
    Serial.println(portalStatus());
    if (portalStatus() == SP_SUBMIT) {
      Serial.println(portalCfg.SSID);
      Serial.println(portalCfg.pass);
      // посмотрим содержимое нашего собственного поля
      Serial.println(device_id);
    }
  }
}
