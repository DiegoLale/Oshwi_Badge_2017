/*
 main.cpp - This is the entry point of the application.

 Copyright (c) 2017 Bricolabs.  All right reserved.
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

#include <Arduino.h>
#include "OS/OS.h"
#include "Blink.h"
#include "OTA.h"
#include "PermanentConnection.h"
#include "ClockUpdater.h"
#include "ModeHandler.h"
#include <Adafruit_NeoPixel.h>

const uint8_t BUTTONPIN = 0;
const uint8_t LEDPIN = 2; // Which pin on the ESP8266 is connected to the NeoPixels?
const uint8_t NUMPIXELS = 5; // How many NeoPixels are attached to the ESP8266?
Adafruit_NeoPixel* pixels = new Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

OS* os = new OS();
//Blink* blink = new Blink();
PermanentConnection* permanentConnection;
OTA* ota = new OTA();
ClockUpdater* clockUpdater = new ClockUpdater();
ModeHandler* modeHandler;

uint32_t orangeColor = pixels->Color(255, 127, 80);

void modeHandlerInterupt() {
    modeHandler->buttonInterrupt();
}

// this should not be here
void configModeCallback (WiFiManager *myWiFiManager) {
  uint32_t redColor = pixels->Color(255, 0, 0);
  for(int i=0; i<NUMPIXELS; i++) {
    pixels->setPixelColor(i, redColor);
  }
  pixels->show();
}

void setup() {

    Serial.begin(57600);

    Serial.println("Starting setup");

    pinMode(LEDPIN, OUTPUT);
    pixels->begin();
    pixels->setBrightness(8);
    for(int i=0; i<NUMPIXELS; i++) {
      pixels->setPixelColor(i, orangeColor);
    }
    pixels->show();

    modeHandler = new ModeHandler(os, pixels);
    permanentConnection = new PermanentConnection(pixels, configModeCallback);

    //os->addProcess(blink, 1000); // Run blink every 1000 ms
    os->addProcess(permanentConnection);
    os->addProcess(ota); // Run ota as fast as possible
    os->addProcess(clockUpdater, 10 * 60 * 1000); // Run every 10 minutes
    os->addProcess(modeHandler);

    pinMode(BUTTONPIN, INPUT);
    attachInterrupt(BUTTONPIN, modeHandlerInterupt, FALLING);


}

void loop() {
    os->handle();
}
