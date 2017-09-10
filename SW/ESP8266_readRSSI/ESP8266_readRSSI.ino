//code by TCRobotics
//Reads wifi signal power
//LED 0 is heartbeat to detect crashes
//4 Green leds = >-55dBm Great signal
//3 Green leds = >-65dBm Good signal
//2 Yellow leds = -70dBm Enough signal
//1 Yellow leds = -80dBm Bad signal
//only heartbeat = <80dBm Really bad signal
//no hearbeat = no power/crash
#include "ESP8266WiFi.h"
#include <Adafruit_NeoPixel.h> //you need to install "Adafruit NeoPixel" library

#define PIN            4 //Which pin is connected to the NeoPixels
#define NUMPIXELS      5 //How many NeoPixels we have
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t color  = pixels.Color(  0,   0,   0);
uint32_t black  = pixels.Color(  0,   0,   0);
uint32_t green  = pixels.Color(  0, 255,   0);
uint32_t yellow = pixels.Color(255, 255,   0);
uint32_t red    = pixels.Color(255,   0,   0);

//const char *ssid         = "";
//const char *password     = "";
ADC_MODE(ADC_VCC);

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(8);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");
  pixels.setPixelColor(0, green); //indicates boot
  pixels.show();

  WiFi.mode(WIFI_STA);

  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    yield();
    Serial.print(".");
    pixels.setPixelColor(1, yellow); //indicates connecting
    pixels.show();
    delay(200);
    pixels.setPixelColor(1, black);
    pixels.show();
    delay(200);
  }
  Serial.println();
  Serial.println("WiFi connected");
          
  pixels.setPixelColor(1, green); //indicates wifi connected
  pixels.show();
  delay(500);
  pixels.setPixelColor(0, black);
  pixels.setPixelColor(1, black);
  pixels.show();
}

void loop() {
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

  for(int i=0;i<NUMPIXELS;i++) //clear all
    pixels.setPixelColor(i, black);
  
  for(int i=0;i<bars;i++)
    pixels.setPixelColor(i, color);
  pixels.show();

  Serial.println(rssi);
  heartbeat();
}

void heartbeat(){
  pixels.setPixelColor(0, red);
  pixels.show();
  delay(500);
  pixels.setPixelColor(0, black);
  pixels.show();
  delay(500);
  yield();
}

