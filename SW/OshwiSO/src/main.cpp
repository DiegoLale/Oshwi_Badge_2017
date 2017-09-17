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

OS* os = new OS();
Blink* blink = new Blink();

void setup() {
    os->addProcess(blink, 1000); // Run blink every 1000 ms
}

void loop() {
    os->handle();
}
