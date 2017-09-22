#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include "OTA/OTA.h"
#include "Clock/Clock.h"
#include <NeoPixelBrightnessBus.h>
#include "colores.h"

const char* ssid = "****";
const char* password = "****";

#define NUMPIXELS      5 // How many NeoPixels are attached to the ESP8266?
NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> pixels(NUMPIXELS);

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

  pixels.Begin();
  pixels.SetBrightness(8);

  for (int j=0; j<5; j++)
  {
    pixels.SetPixelColor(j, RgbColor(0,0,0));
  }

  pixels.Show();
}

void loop() {
  handleOTA();

  secodsWithMillis extTime = Clock::getTime();

  int colorIndex = extTime.seconds % 8;
  for(int i=0;i<NUMPIXELS;i++)
  {
        pixels.SetPixelColor(i, RgbColor(colores[colorIndex].r,
                                         colores[colorIndex].g,
                                         colores[colorIndex].b));
  }

  pixels.Show();
}
