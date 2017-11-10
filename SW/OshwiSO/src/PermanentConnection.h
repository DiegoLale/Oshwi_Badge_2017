/*
 Blink.h - This class connects to a wifi networks and mantains the Connection
 forever.

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
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

class PermanentConnection : public Process
{
  public:
    void setup()
    {
      wifiManager.resetSettings();
      pinMode(PIN, INPUT_PULLUP);
      String apName = String("Oshwi_") + String(ESP.getChipId());
      if (digitalRead(PIN))
        wifiManager.autoConnect(apName.c_str(), _password);
      else
        if (!wifiManager.startConfigPortal(apName.c_str(), _password)) {
          Serial.println("failed to connect and hit timeout");
          delay(3000);
          //reset and try again, or maybe put it to deep sleep
          ESP.reset();
          delay(5000);
        }
    }

    void loop(){}

  private:
    const int PIN = 5;
    WiFiManager wifiManager;
    const char* _password = "12345678";
};
