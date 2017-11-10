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

// delay 2
class Rainbow : public Process
{
  public:
    Rainbow(Adafruit_NeoPixel* pixels)
    {
      _pixels = pixels;
    }

    void setup()
    {
      for (int j=0; j<5; j++)
      {
        _pixels->setPixelColor(j, _pixels->Color(0,0,0));
      }

      _pixels->setPixelColor(2, _pixels->Color(255,0,0));
      _pixels->show();
      delay(500);
    }

    void loop()
    {
      uint16_t i, j;

      for(j=0; j<256; j++) {
        for(i=0; i< _pixels->numPixels(); i++) {
          _pixels->setPixelColor(i, Wheel(((i * 256 / _pixels->numPixels()) + j) & 255));
        }
        _pixels->show();
      }
    }

  private:
    Adafruit_NeoPixel* _pixels;

    uint32_t Wheel(byte WheelPos) {
      WheelPos = 255 - WheelPos;
      if(WheelPos < 85) {
        return _pixels->Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      if(WheelPos < 170) {
        WheelPos -= 85;
        return _pixels->Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      WheelPos -= 170;
      return _pixels->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
};
