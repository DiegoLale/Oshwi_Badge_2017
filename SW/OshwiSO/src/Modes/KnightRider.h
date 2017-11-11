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

// delay 100
class KnightRider : public Process
{
  public:
    KnightRider(Adafruit_NeoPixel* pixels)
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

      _pixels->setPixelColor(1, _pixels->Color(255,0,0));
      _pixels->show();
      delay(500);

    }

    void loop()
    {
      switch(step++)
      {
        case 0:
          _pixels->setPixelColor(0, red);
          _pixels->setPixelColor(1, black);
          _pixels->show();
          break;
        case 1:
          _pixels->setPixelColor(1, red);
          _pixels->setPixelColor(0, black);
          _pixels->show();
          break;
        case 2:
          _pixels->setPixelColor(2, red);
          _pixels->setPixelColor(1, black);
          _pixels->show();
          break;
        case 3:
          _pixels->setPixelColor(3, red);
          _pixels->setPixelColor(2, black);
          _pixels->show();
          break;
        case 4:
          _pixels->setPixelColor(4, red);
          _pixels->setPixelColor(3, black);
          _pixels->show();
          break;
        case 5:
          _pixels->setPixelColor(3, red);
          _pixels->setPixelColor(4, black);
          _pixels->show();
          break;
        case 6:
          _pixels->setPixelColor(2, red);
          _pixels->setPixelColor(3, black);
          _pixels->show();
          break;
        case 7:
          _pixels->setPixelColor(1, red);
          _pixels->setPixelColor(2, black);
          _pixels->show();
          break;
      }

      if (step > 7) step = 0;
    }

  private:
    Adafruit_NeoPixel* _pixels;
    uint8_t step = 0;

    uint32_t colorVar= _pixels->Color(  0,   0,   0);
    const uint32_t black  = _pixels->Color(  0,   0,   0);
    const uint32_t green  = _pixels->Color(  0, 255,   0);
    const uint32_t yellow = _pixels->Color(255, 255,   0);
    const uint32_t red    = _pixels->Color(255,   0,   0);
    const uint32_t white  = _pixels->Color(255, 255, 255);
};
