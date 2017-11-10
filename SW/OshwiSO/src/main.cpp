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

const uint8_t BUTTONPIN = 0;

OS* os = new OS();
Blink* blink = new Blink();
PermanentConnection* permanentConnection = new PermanentConnection();
OTA* ota = new OTA();
ClockUpdater* clockUpdater = new ClockUpdater();
ModeHandler* modeHandler = new ModeHandler(os);

void modeHandlerInterupt() {
    modeHandler->buttonInterrupt();
}

void setup() {

    Serial.begin(9600);
    //os->addProcess(blink, 1000); // Run blink every 1000 ms
    os->addProcess(permanentConnection);
    //os->addProcess(ota); // Run ota as fast as possible
    //os->addProcess(clockUpdater, 10 * 60 * 1000); // Run every 10 minutes
    //os->addProcess(modeHandler);

  //  pinMode(BUTTONPIN,INPUT);
    //attachInterrupt(BUTTONPIN, modeHandlerInterupt, FALLING);
}

void loop() {
    os->handle();
}
