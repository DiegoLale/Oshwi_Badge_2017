//Copied, mixed and remixed by TCRobotics
//based on SNTPtime for ESP8266 by Andreas Spiess
#include <Adafruit_NeoPixel.h>
#include <SNTPtime.h> //https://github.com/SensorsIot/SNTPtime

// Which pin on the ESP8266 is connected to the NeoPixels?
#define PIN            4
// How many NeoPixels are attached to the ESP8266?
#define NUMPIXELS      5

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

SNTPtime NTPch("es.pool.ntp.org");

strDateTime dateTime;

//const char *ssid     = "";
//const char *password = "";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(8);

  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, pixels.Color(255,0,0));
  pixels.show(); // This sends the updated pixel color to the hardware.

  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("WiFi connected");

  while (!NTPch.setSNTPtime()) Serial.print("."); // set internal clock
  Serial.println();
  Serial.println("Time set");

  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, pixels.Color(0,255,0));
  pixels.show();
  delay(1000);
}

void loop() {
  // first parameter: Time zone; second parameter: 1 for European summer time; 2 for US daylight saving time (not implemented yet)
  dateTime = NTPch.getTime(1.0, 1); // get time from internal clock
  NTPch.printDateTime(dateTime);
  
  byte actualsecond = dateTime.second;
  if(actualsecond>9) actualsecond = actualsecond - (actualsecond/10)*10;
  
  switch (actualsecond) {
    case 0:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      break;
    case 1:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,0,255));
      break;
    case 2:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,255,0));
      break;
    case 3:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,255,255));
      break; 
    case 4:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,0,0));
      break;
    case 5:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,0,255));
      break;
    case 6:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,255,0));
      break;
    case 7:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,255,255));
      break;
    case 8:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(128,0,128));
      break;
    case 9:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,165,0));
      break;              
  }
  pixels.show();
  delay(50);
}
