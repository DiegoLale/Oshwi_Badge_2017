#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include "OTA/OTA.h"
#include "Clock/Clock.h"
#include <Adafruit_NeoPixel.h>
#include "colores.h"

const char* ssid = "****";
const char* password = "****";

#define PIN            4 // Which pin on the ESP8266 is connected to the NeoPixels?
#define NUMPIXELS      5 // How many NeoPixels are attached to the ESP8266?
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  initOTA();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  while (!Clock::updateTime())
  {
    Serial.println("Unable to update time.");
  }

  for (int j=0; j<5; j++)
  {
    pixels.setPixelColor(j, pixels.Color(0,0,0));
  }

  pixels.show();
}

void loop() {
  handleOTA();

  secodsWithMillis extTime = Clock::getTime();

  int colorIndex = extTime.seconds % 8;
  for(int i=0;i<NUMPIXELS;i++)
  {
        pixels.setPixelColor(i, pixels.Color(colores[colorIndex].r,
                                             colores[colorIndex].g,
                                             colores[colorIndex].b));
  }

  pixels.show();
}
