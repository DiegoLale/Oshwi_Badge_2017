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

class BasicMode : public Process
{
  public:
    BasicMode(Adafruit_NeoPixel* pixels)
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

      _pixels->setPixelColor(0, _pixels->Color(255,0,0));
      _pixels->show();
      delay(500);
    }

    void loop()
    {
      secodsWithMillis extTime = Clock::getTime();

      int colorIndex = extTime.seconds % 8;
      for(int i=0;i<_pixels->numPixels();i++)
      {
            _pixels->setPixelColor(i, _pixels->Color(colores[colorIndex].r,
                                                 colores[colorIndex].g,
                                                 colores[colorIndex].b));
      }

      _pixels->show();
    }

  private:
    Adafruit_NeoPixel* _pixels;
};
