/*
 Blink.h - This class is an example of a simple process. For simplicity it is
 implemented in the header file but if the class is larger the usual separation
 between .h and .cpp files is recomended.

 Copyright (c) 2017 Oscar Blanco.  All right reserved.
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "OS/Process.h"
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "Colores.h"

// delay 1000
class Rssi : public Process
{
  public:
    Rssi(Adafruit_NeoPixel* pixels)
    {
      _pixels = pixels;
    }

    void setup()
    {
      Process::setup();
      _pixels->setBrightness(8);
      for (int j=0; j<5; j++)
      {
        _pixels->setPixelColor(j, _pixels->Color(0,0,0));
      }

      _pixels->setPixelColor(3, _pixels->Color(255,0,0));
      _pixels->show();
      delay(500);
    }

    void loop()
    {
      float rssi = WiFi.RSSI();
      int bars = 0;

      if (rssi > -55)
        { bars = 5; colorVar = green;}
      else if (rssi > -65 & rssi < -55)
        { bars = 4; colorVar = green;}
      else if (rssi > -70 & rssi < -65)
        { bars = 3; colorVar = yellow;}
      else if (rssi > -80 & rssi < -70)
        { bars = 2; colorVar = yellow;}
      else
        { bars = 1; colorVar = red;}

      for(int i=0;i<bars;i++)
        _pixels->setPixelColor(i, colorVar);
      Serial.println(rssi);
      _pixels->show();
    }

  private:
    Adafruit_NeoPixel* _pixels;

    uint32_t colorVar= _pixels->Color(  0,   0,   0);
    const uint32_t black  = _pixels->Color(  0,   0,   0);
    const uint32_t green  = _pixels->Color(  0, 255,   0);
    const uint32_t yellow = _pixels->Color(255, 255,   0);
    const uint32_t red    = _pixels->Color(255,   0,   0);
    const uint32_t white  = _pixels->Color(255, 255, 255);
};
