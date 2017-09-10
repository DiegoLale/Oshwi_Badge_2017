//code by TCRobotics
//Reads internal Vcc voltage, needs radio on and ADC pin floating!
//LED 0 is heartbeat to detect crashes
//4 Green leds = VCC>3.2V
//3 Green leds = VCC>3.1V
//2 Yellow leds = VCC>3.0V
//1 Yellow leds = VCC>2.9V
//only heartbeat = VCC<2.9V Replace lipo!
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
  pixels.show();

  Serial.println(voltage);
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

