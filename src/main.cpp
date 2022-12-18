/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-post-image-photo-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <NTPClient.h>
#include <TOTP.cpp>

#include "readConfig.cpp"

// #include "camera.cpp"


// Initialisieren Sie den Async Webserver auf Port 80
AsyncWebServer server(80);

// Setup the ntpUDP client
WiFiUDP ntpUDP;
// Start the NTP client
NTPClient timeClient = NTPClient(ntpUDP, "time-a-g.nist.gov", 3600, 60000);


void setup() {
  // initialisation of the main variables
  Serial.begin(9600);

  while (!Serial) { }

  Serial.println("Starting…");

  // Initialisieren Sie den SPIFFS
  Serial.println("Initialisieren des SPIFFS…");
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS konnte nicht initialisiert werden");
    return;
  }

  // Read the config file
  Serial.println("Reading config…");// Create the config object
  Config config = readConfig();

  // Connect to the WiFi network
  Serial.println("Verbindung zum WiFi-Netzwerk aufbauen…");
  // Setzen Sie den WiFi-Modus auf Station
  WiFi.mode(WIFI_STA);

  // Verbinden Sie sich mit dem WiFi-Netzwerk
  WiFi.begin(config.system_wifi_ssid.c_str(), config.system_wifi_password.c_str());

  // Setzen des Hostnamen
  WiFi.setHostname("ESP32-CAM");

  // Warten Sie, bis Sie mit dem WiFi-Netzwerk verbunden sind
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print the IP address and hostname of the ESP32-CAM
  Serial.println("Verbunden mit dem WiFi-Netzwerk");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());

  // Sync the RTC with the NTP server
  timeClient.begin();
  
  // Set the TOTP secret key
  // carst String secret into uint8_t array
  static TOTP totp = TOTP(config.api_totp_key, 16, 30);

  // Start the camera
  // cameraSetup();

  // Redirect to the totp page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "Server is up and running");
  });

  // Definieren Sie eine Route, die den aktuellen TOTP-Code zurückgibt
  server.on("/totp", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("TOTP requested");

    // Generieren Sie den aktuellen TOTP-Code
    timeClient.update();
    String newCode = String(totp.getCode(timeClient.getEpochTime()));

    // Senden Sie den TOTP-Code als Textantwort zurück
    request->send(200, "text/plain", newCode);
  });

  // Resives a totp code via GET and checks if it is valid
  server.on("/verify", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("TOTP requested");

    // if the parameter is found
    if (request->hasParam("totp")) {
      // Get the totp code from the request
      String requestCode = request->getParam("totp")->value();

      // Generieren Sie den aktuellen TOTP-Code
      timeClient.update();
      String currentCode = String(totp.getCode(timeClient.getEpochTime()));

      // Check if the totp code is valid
      if (requestCode == currentCode) {
        request->send(200, "text/plain", "Valid TOTP code");
      } else {
        request->send(200, "text/plain", "Invalid TOTP code");
      }
    } else {
      request->send(200, "text/plain", "No TOTP code provided");
    }
  });

  // This endpoint returns the current DateTime
  server.on("/time", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Time requested");
    
    // Get the current DateTime
    timeClient.update();

    time_t rawtime = timeClient.getEpochTime();
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);

    // Send the DateTime as a text response
    request->send(200, "text/plain", asctime(timeinfo));
  });

  server.onNotFound([](AsyncWebServerRequest *request){request->send(404);});

  Serial.println("Starting server…");
  server.begin(); // Starten Sie den Async Webserver
}

void loop() { }


