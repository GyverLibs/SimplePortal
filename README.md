[![latest](https://img.shields.io/github/v/release/GyverLibs/SimplePortal.svg?color=brightgreen)](https://github.com/GyverLibs/SimplePortal/releases/latest/download/SimplePortal.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/SimplePortal.svg)](https://registry.platformio.org/libraries/gyverlibs/SimplePortal)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/SimplePortal?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# SimplePortal
Простой менеджер WiFi для esp8266 для задания логина-пароля WiFi и режима работы  

> Для более широких возможностей настройки с веба используй [GyverPortal](https://github.com/GyverLibs/GyverPortal) - библиотеку конструктора веб интерфейса с кучей возможностей

### Совместимость
esp8266, esp32

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **SimplePortal** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/SimplePortal/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!


<a id="init"></a>
## Инициализация
Нет

<a id="usage"></a>
## Использование
```cpp
void portalStart();     // запустить портал
void portalStop();      // остановить портал
bool portalTick();      // вызывать в цикле
void portalRun(uint32_t prd = 60000);   // блокирующий вызов
byte portalStatus();    // статус: 1 connect, 2 ap, 3 local, 4 exit, 5 timeout

// константы статуса
SP_ERROR            // ошибка
SP_SUBMIT           // отправлены логин-пароль
SP_SWITCH_AP        // сигнал на смену в AP
SP_SWITCH_LOCAL     // сигнал на смену в Local
SP_EXIT             // нажата кнопка выход
SP_TIMEOUT          // вышел таймаут
```

### Хранение настроек
Из скетча доступна переменная (структура) **portalCfg**:
- `char* SSID[32]` - логин
- `char* pass[32]` - пароль
- `byte mode` - (1 `WIFI_STA`, 2 `WIFI_AP`)
После отправки данных с портала можно забрать информацию из структуры и/или записать её в EEPROM.

### Как работает
![demo](/doc/demo.png)
- Библиотека запускает режим **SoftAP**, запускает **DNSServer** и **ESP8266WebServer**.
- Коннектимся к точке, по умолчанию называется *"ESP Config"*
- Откроется страница конфигурации (если не открылась - перейди по *192.168.1.1*). На странице можно 
настроить режим работы ESP, а также сконфигурировать подключение к роутеру (логин-пароль).
- Кнопка *Submit* отправляет введённые данные на ESP. После нажатия *Submit* режим в конфиге переключается в 1 (WIFI_STA).
- Библиотека не переключает итоговый режим работы esp, кнопки портала *Switch AP* / *Switch Local* просто дают сигналы "в скетч".
- Кнопка Exit просто завершает работу портала.
- После нажатия любой кнопки или выхода таймаута портал закрывается, сервер останавливается, SoftAP отключается.

<a id="example"></a>
## Примеры
### Блокирующий
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);
  delay(3000);
  
  portalRun();  // запустить с таймаутом 60с
  //portalRun(30000); // запустить с кастомным таймаутом
  
  Serial.println(portalStatus());
  // статус: 0 error, 1 connect, 2 ap, 3 local, 4 exit, 5 timeout
  
  if (portalStatus() == SP_SUBMIT) {
    Serial.println(portalCfg.SSID);
    Serial.println(portalCfg.pass);
    // забираем логин-пароль
  }
}

void loop() {
}
```

### Асинхронный
```cpp
#include <ESP8266WiFi.h>
#include <SimplePortal.h>

void setup() {
  Serial.begin(9600);

  // запускаем портал
  portalStart();
}

void loop() {
  // вызываем в loop
  if (portalTick()) {
    Serial.println(portalStatus());
    if (portalStatus() == SP_SUBMIT) {
      Serial.println(portalCfg.SSID);
      Serial.println(portalCfg.pass);
      // забираем логин-пароль
    }

    // сработает однократно при действии
    // точка будет автоматически выключена
  }
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - совместимость с ESP32
- v1.2 - исправлен баг

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!


При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код
