#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#endif

#include <FS.h>
#include <SecuritySettingsService.h>
#include <WiFiSettingsService.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <OTASettingsService.h>
#include <AuthenticationService.h>
#include <WiFiScanner.h>
#include <WiFiStatus.h>
#include <NTPStatus.h>
#include <APStatus.h>
#include <SystemStatus.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <MashSettingsService.h>
#include <BoilSettingsService.h>
#include <BrewSettingsService.h>
#include <BrewService.h>
#include <MashService.h>
#include <BoilService.h>
#include <TemperatureService.h>
#include <HeaterService.h>
#include <ActiveStatus.h>
#include <Buzzer.h>
#include <Pump.h>

#define SERIAL_BAUD_RATE 115200

OneWire oneWire(TEMPERATURE_BUS);
DallasTemperature DS18B20(&oneWire);
int deviceCount = 0;

AsyncWebServer server(80);

WiFiSettingsService wifiSettingsService = WiFiSettingsService(&server, &SPIFFS);
APSettingsService apSettingsService = APSettingsService(&server, &SPIFFS);
OTASettingsService otaSettingsService = OTASettingsService(&server, &SPIFFS);

WiFiScanner wifiScanner = WiFiScanner(&server);
WiFiStatus wifiStatus = WiFiStatus(&server);
NTPStatus ntpStatus = NTPStatus(&server);
APStatus apStatus = APStatus(&server);
SystemStatus systemStatus = SystemStatus(&server);

//brewUNO
ActiveStatus activeStatus = ActiveStatus(&SPIFFS);
NTPSettingsService ntpSettingsService = NTPSettingsService(&server, &SPIFFS, &activeStatus);

BrewSettingsService brewSettingsService = BrewSettingsService(&server, &SPIFFS, &activeStatus);
TemperatureService temperatureService = TemperatureService(&server, &SPIFFS, DS18B20, &brewSettingsService);
MashSettingsService mashSettings = MashSettingsService(&server, &SPIFFS);
BoilSettingsService boilSettingsService = BoilSettingsService(&server, &SPIFFS, &brewSettingsService);

Pump pump = Pump(&server, &activeStatus, &brewSettingsService);
HeaterService heaterService = HeaterService(&temperatureService, &activeStatus, &brewSettingsService);
MashService mashService = MashService(&SPIFFS, &temperatureService, &pump);
BoilService boilService = BoilService(&SPIFFS, &temperatureService, &brewSettingsService);
BrewService brewService = BrewService(&server, &SPIFFS, &mashService, &boilService, &brewSettingsService, &heaterService, &activeStatus, &temperatureService, &pump);

void setup()
{
  // Disable wifi config persistance and auto reconnect
  WiFi.persistent(false);
  WiFi.setAutoReconnect(false);

#if defined(ESP_PLATFORM)
  // Init the wifi driver on ESP32
  WiFi.mode(WIFI_MODE_MAX);
  WiFi.mode(WIFI_MODE_NULL);
#endif

  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();
  //SPIFFS.format();

  // Start security settings service first
  //securitySettingsService.begin();

  // Start services
  ntpSettingsService.begin();
  otaSettingsService.begin();
  apSettingsService.begin();
  wifiSettingsService.begin();

  // Serving static resources from /www/
  server.serveStatic("/js/", SPIFFS, "/www/js/", "max-age=86400");
  server.serveStatic("/css/", SPIFFS, "/www/css/", "max-age=86400");
  server.serveStatic("/fonts/", SPIFFS, "/www/fonts/", "max-age=86400");
  server.serveStatic("/app/", SPIFFS, "/www/app/", "max-age=86400");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico", "max-age=86400");
  server.serveStatic("/app/logo.png", SPIFFS, "/www/app/logo.png", "max-age=86400");

  // Serving all other get requests with "/www/index.htm"
  // OPTIONS get a straight up 200 response
  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_GET)
      request->send(SPIFFS, "/www/index.html");
    else if (request->method() == HTTP_OPTIONS)
      request->send(200);
    else
      request->send(404);
  });

// Disable CORS if required
#if defined(ENABLE_CORS)
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", CORS_ORIGIN);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
#endif

  server.begin();

  //BrewUNO
  pinMode(PUMP_BUS, OUTPUT);

#ifdef BUZZER_BUS
  pinMode(BUZZER_BUS, OUTPUT);
  digitalWrite(BUZZER_BUS, LOW);
#endif  // BUZZER_BUS

  pinMode(HEATER_BUS, OUTPUT);
  digitalWrite(HEATER_BUS, LOW);

  pump.TurnPumpOff();
  DS18B20.begin();
  // locate devices on the bus
  Serial.println("");
  Serial.println("Hello! I'm BrewUNO =)");
  Serial.println("Locating DS18B20 devices...");
  Serial.print("Found ");
  deviceCount = DS18B20.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  temperatureService.DeviceCount = deviceCount;
  brewSettingsService.begin();
  brewService.begin();
}

void loop()
{
  wifiSettingsService.loop();
  apSettingsService.loop();
  ntpSettingsService.loop();
  otaSettingsService.loop();
  brewService.loop();
}