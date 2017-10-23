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
#include "OS/OS.h"
#include "Modes/BasicMode.h"
#include <Adafruit_NeoPixel.h>

class ModeHandler : public Process
{
  public:
    // Constructor
    ModeHandler(OS* os)
    {
      _os = os;
    }

    void setup()
    {
      pinMode(PIN, OUTPUT);
      _pixels = new Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
      _pixels->begin();
      _pixels->setBrightness(8);
      setMode(new BasicMode(_pixels));
    }

    void loop()
    {
      /*
      if (1 == 0)
      {
        setMode(new DiscoMode(_pixels))
      }
      */
    }

  private:
    OS* _os;
    Process* _currentMode;
    const int PIN = 4; // Which pin on the ESP8266 is connected to the NeoPixels?
    const int NUMPIXELS = 5; // How many NeoPixels are attached to the ESP8266?
    Adafruit_NeoPixel* _pixels;

    void setMode(Process* newMode)
    {
      if (_currentMode)
      {
        _os->killProcess(_currentMode);
        delete _currentMode;
      }

      _os->addProcess(newMode);
      _currentMode = newMode;
    }
};
