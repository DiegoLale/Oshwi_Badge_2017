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
#include "Modes/KnightRider.h"
#include "Modes/Rainbow.h"
#include "Modes/VoltageTest.h"
#include "Modes/RSSI.h"
#include "Modes/Flashlight.h"
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

      basicMode = new BasicMode(_pixels);
      knightRider = new KnightRider(_pixels);
      rainbow = new Rainbow(_pixels);
      rssi=new Rssi(_pixels);
      flashlight = new Flashlight(_pixels);
      voltageTest = new VoltageTest(_pixels);
      setMode(basicMode);
    }

    void loop()
    {

    }

    void buttonInterrupt(){
      if (millis() - lastModeChange < 200)
        return;

      lastModeChange = millis();
      Process* newMode;

      switch(mode++){
        case 0:
          Serial.println("setMode: BasicMode");
          setMode(basicMode);
          break;
        case 1:
          Serial.println("setMode: KnightRider");
          setMode(knightRider, 100);
          break;
        case 2:
          Serial.println("setMode: Rainbow");
          setMode(rainbow, 2);
          break;
        case 3:
          Serial.println("setMode: Rssi");
          setMode(rssi, 1000);
          break;
        case 4:
          Serial.println("setMode: VoltageTest");
          setMode(voltageTest, 1000);
          break;
        case 5:
          Serial.println("setMode: Flashlight");
          setMode(flashlight, 20000);
          break;
      }

      if (mode > 5) mode = 0;
    }

  private:
    Process *basicMode, *knightRider, *rainbow, *rssi, *flashlight, *voltageTest;

    uint8_t mode = 1; // start with 1 because we are already in mode 0
    OS* _os;
    Process* _currentMode;
    const uint8_t PIN = 2; // Which pin on the ESP8266 is connected to the NeoPixels?
    const uint8_t NUMPIXELS = 5; // How many NeoPixels are attached to the ESP8266?
    Adafruit_NeoPixel* _pixels;
    unsigned long lastModeChange = 0;

    void setMode(Process* newMode, unsigned int interval=0)
    {
      if (_currentMode)
      {
        _os->killProcess(_currentMode);
        //delete _currentMode;
      }

      _os->addProcess(newMode, interval);
      _currentMode = newMode;
    }
};
