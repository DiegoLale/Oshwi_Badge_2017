//Copied, mixed and remixed by TCRobotics
//based on SNTPtime for ESP8266 by Andreas Spiess
#include <Adafruit_NeoPixel.h>
#include <SNTPtime.h> //https://github.com/SensorsIot/SNTPtime

// Which pin on the ESP8266 is connected to the NeoPixels?
#define PIN       4
// How many NeoPixels are attached to the ESP8266?
#define NUMPIXELS 5

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t color[10] = {  pixels.Color(0,   0,   0), 
                        pixels.Color(0,   0, 255),
                        pixels.Color(0, 255,   0),
                        pixels.Color(0, 255, 255),
                        pixels.Color(255, 0,   0),
                        pixels.Color(255, 0, 255),
                        pixels.Color(255,255,  0),
                        pixels.Color(255,255,255),
                        pixels.Color(128, 0, 128),
                        pixels.Color(255,165,  0)};

SNTPtime NTPch("es.pool.ntp.org");
strDateTime dateTime;
byte actualSecond = 0;

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

  // first parameter: Time zone; 
  // second parameter: 1 for European summer time; 2 for US daylight saving time (not implemented yet)
  dateTime = NTPch.getTime(1.0, 1); // get time from internal clock
  actualSecond = dateTime.second;
  
  do 
  {
    dateTime = NTPch.getTime(1.0, 1); 
    yield(); //to not crash while waiting
  }
  while (actualSecond==dateTime.second);
}

void loop() {
  dateTime = NTPch.getTime(1.0, 1);
  actualSecond = dateTime.second;
  if(actualSecond>9) actualSecond = actualSecond - (actualSecond/10)*10;

  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, color[actualSecond]);
  pixels.show();
  yield(); //to not crash
}
