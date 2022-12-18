#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

struct Config {
  String system_wifi_ssid;
  String system_wifi_password;
  String system_wifi_hostname;
  int system_serial_baud;

  uint8_t api_totp_key[16];
  int api_totp_period;
  String api_ssl_keyPath;
  String api_ssl_certPath;
};

Config readConfig() {
  Config config;

  // Öffne die Konfigurationsdatei
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Konfigurationsdatei konnte nicht geöffnet werden");
    return config;
  }

  // Lese die Konfigurationsdaten in das Struct
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("Fehler beim Parsen der Konfigurationsdatei: ");
    Serial.println(error.c_str());
    configFile.close();
    return config;
  }
  config.system_wifi_ssid = doc["system"]["wifi"]["ssid"].as<String>();
  config.system_wifi_password = doc["system"]["wifi"]["password"].as<String>();
  config.system_wifi_hostname = doc["system"]["wifi"]["hostname"].as<String>();
  config.system_serial_baud = doc["system"]["serial"]["baud"].as<long>();
  
  String api_totp_key = doc["api"]["totp"]["key"].as<String>();
  for (int i = 0; i < 16; i++) {
    config.api_totp_key[i] = api_totp_key[i];
  }

  config.api_totp_period = doc["api"]["totp"]["period"].as<int>();
  config.api_ssl_keyPath = doc["api"]["ssl"]["key_path"].as<String>();
  config.api_ssl_certPath = doc["api"]["ssl"]["cert_path"].as<String>();

  configFile.close();
  return config;
}
