//Code by TCRobotics
#include <Adafruit_NeoPixel.h>

#define NEOPIXELPIN 4
#define NUMPIXELS   5

#define BUTTONPIN 0
#define MODE0 0
#define MODE1 1
#define MODE2 2
#define MODE3 3
#define MODE4 4

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

uint32_t red    = pixels.Color(255,   0,   0);
uint32_t black  = pixels.Color(  0,   0,   0);

int functionMode = 0;
long functionStartTime = 0;
int timeout = 5000;


void buttonInterrupt(){
  long intTime = millis();
  if((intTime - functionStartTime)>200) //avoid button bounce
  {
    functionMode++;
    if (functionMode>4) functionMode = 0;
    functionStartTime = intTime;
  }
}

void setup() {
  pixels.begin();
  pixels.setBrightness(8);
  pinMode(BUTTONPIN,INPUT);
  attachInterrupt(BUTTONPIN, buttonInterrupt, FALLING);
}

void loop() {
  if((millis() - functionStartTime) > timeout) functionMode = 0;

  for(int i=0;i<NUMPIXELS;i++)
    pixels.setPixelColor(i, black);

  switch(functionMode)
  {
    case MODE0:
      pixels.setPixelColor(0, red);
      break;
    case MODE1:
      pixels.setPixelColor(1, red);
      break;
    case MODE2:
      pixels.setPixelColor(2, red);
      break;
    case MODE3:
      pixels.setPixelColor(3, red);
      break;
    case MODE4:
      pixels.setPixelColor(4, red);
      break;
  }
  pixels.show();
}
