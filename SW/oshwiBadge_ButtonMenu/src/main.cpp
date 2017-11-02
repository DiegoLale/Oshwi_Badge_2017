//Fast & badly coded by TCRobotics
//USR button changes between five modes
//Modes 1 to 4 have a timeout that return to mode 0
//if you press five seconds USR button flashlight mode is activated!

#include <Arduino.h> //for platformio
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <Adafruit_NeoPixel.h>    //needed for adresable leds
//needed for wifiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Ticker.h>//for timing
//needed for OTA
#include "ArduinoOTA.h"
#include <ESP8266mDNS.h>

const char* ssid = "OshwiBadge_XX"; //XX put here your badge number
const char* password = "pulpinho";

#define NEOPIXELPIN 2
#define NUMPIXELS   5

#define BUTTONPIN 0
#define MODE0 0 // Default: LED sync
#define MODE1 1 // KnightRider
#define MODE2 2 // Rainbow
#define MODE3 3 // RSSI
#define MODE4 4 // Voltage test
#define MODE5 5 // FLASHLIGHT

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);
Ticker ticker; //for timing

uint32_t color  = pixels.Color(  0,   0,   0);
uint32_t black  = pixels.Color(  0,   0,   0);
uint32_t green  = pixels.Color(  0, 255,   0);
uint32_t yellow = pixels.Color(255, 255,   0);
uint32_t red    = pixels.Color(255,   0,   0);
uint32_t white  = pixels.Color(255, 255, 255);

int functionMode = 0;
long functionStartTime = 0;
int timeout = 10000;
int firstTime = 1; //to check mode change

ADC_MODE(ADC_VCC); //to read internal voltage

//from Adafruit_NeoPixel library example buttoncycler
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//for blinking ESP12 led
void tick()
{
  int state = digitalRead(BUILTIN_LED);  // get the current state
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void buttonInterrupt(){
  long intTime = millis();
  if((intTime - functionStartTime)>200) //avoid button bounce
  {
    functionMode++;
    if (functionMode>4) functionMode = MODE0;
    functionStartTime = intTime;
    firstTime = 1;
  }
}

void setup() {
  Serial.begin(115200);
  pixels.begin(); //start LEDs
  pixels.show();

  //OTA///////////////////////////////////////////////////////////////////////////////////////
  ArduinoOTA.setPassword(password);
  ArduinoOTA.onStart([]()
  {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
     // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]()
  {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  //WifiManager//////////////////////////////////////////////////////////////////////////////
  pinMode(BUILTIN_LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //set callback that gets called when connecting to previous WiFi fails,
  //and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect(ssid, password)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  //keep LED off
  digitalWrite(BUILTIN_LED, HIGH);
  //pinMode(BUILTIN_LED, INPUT_PULLUP);
  //MENU/////////////////////////////////////////////////////////////////////////////////////
  pinMode(BUTTONPIN,INPUT);
  attachInterrupt(BUTTONPIN, buttonInterrupt, FALLING);
}

void loop() {

  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, black);
  pixels.setBrightness(8);

  if((millis() - functionStartTime) > timeout)
  {
    functionMode = MODE0;
    if(!digitalRead(BUTTONPIN)) //FLASHLIGHT MODE!!
      functionMode = MODE5;
  }

  switch(functionMode)
  {
    case MODE0:
      if(firstTime)
      {
        pixels.setPixelColor(0, red);
        pixels.show();
        delay(500);
        firstTime = 0;
      }
      else
      {

      }
      break;
    case MODE1:
      if(firstTime)
      {
        pixels.setPixelColor(1, red);
        pixels.show();
        delay(500);
        firstTime = 0;
      }
      else
      {
        pixels.setPixelColor(0, red);
        pixels.setPixelColor(1, black);
        pixels.show();
        delay(100);
        pixels.setPixelColor(1, red);
        pixels.setPixelColor(0, black);
        pixels.show();
        delay(100);
        pixels.setPixelColor(2, red);
        pixels.setPixelColor(1, black);
        pixels.show();
        delay(100);
        pixels.setPixelColor(3, red);
        pixels.setPixelColor(2, black);
        pixels.show();
        delay(100);
        pixels.setPixelColor(4, red);
        pixels.setPixelColor(3, black);
        pixels.show();
        delay(100);
        pixels.setPixelColor(3, red);
        pixels.setPixelColor(4, black);
        pixels.show();
        delay(100);
        pixels.setPixelColor(2, red);
        pixels.setPixelColor(3, black);
        pixels.show();
        delay(100);
        pixels.setPixelColor(1, red);
        pixels.setPixelColor(2, black);
        pixels.show();
        delay(100);
      }
      break;
    case MODE2:
      if(firstTime)
      {
        pixels.setPixelColor(2, red);
        pixels.show();
        delay(500);
        firstTime = 0;
      }
      else
      {
        //from Adafruit_NeoPixel library example buttoncycler
        uint16_t i, j;

        for(j=0; j<256; j++) {
          for(i=0; i< pixels.numPixels(); i++) {
            pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
          }
          pixels.show();
          delay(2);
        }
      }
      break;
    case MODE3:
      if(firstTime)
      {
        pixels.setPixelColor(3, red);
        pixels.show();
        delay(500);
        firstTime = 0;
      }
      else
      {
        float rssi = WiFi.RSSI();
        int bars = 0;

        if (rssi > -55)
          { bars = 5; color = green;}
        else if (rssi > -65 & rssi < -55)
          { bars = 4; color = green;}
        else if (rssi > -70 & rssi < -65)
          { bars = 3; color = yellow;}
        else if (rssi > -80 & rssi < -70)
          { bars = 2; color = yellow;}
        else
          { bars = 1; color = red;}

        for(int i=0;i<bars;i++)
          pixels.setPixelColor(i, color);
        Serial.println(rssi);
        pixels.show();
        delay(1000);
      }
      break;
    case MODE4:
      if(firstTime)
      {
        pixels.setPixelColor(4, red);
        pixels.show();
        delay(500);
        firstTime = 0;
      }
      else
      {
        float voltage = ESP.getVcc()/1000.0;
        int bars = 0;

        if (voltage > 3.2)
          { bars = 5; color = green;}
        else if (voltage > 3.1 & voltage < 3.2)
          { bars = 4; color = green;}
        else if (voltage > 3.0 & voltage < 3.1)
          { bars = 3; color = yellow;}
        else if (voltage > 2.9 & voltage < 3.0)
          { bars = 2; color = yellow;}
        else
          { bars = 1; color = red;}

        for(int i=0;i<bars;i++)
          pixels.setPixelColor(i, color);
        Serial.println(voltage);
        pixels.show();
      }
      break;
    case MODE5: //FLASHLIGT MODE
      pixels.setBrightness(255);
      pixels.setPixelColor(0, white);
      pixels.setPixelColor(1, white);
      pixels.setPixelColor(2, white);
      pixels.setPixelColor(3, white);
      pixels.setPixelColor(4, white);
      break;
  }

  pixels.show();
  ArduinoOTA.handle();
}
