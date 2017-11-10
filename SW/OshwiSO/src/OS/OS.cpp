/*
 OS.cpp - This class manages the life cycle of the processes emulating an
 basic operating system

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

#include "OS.h"
#include "Arduino.h"

void OS::addProcess(Process* p, unsigned int interval)
{
  //Serial.println(String("add ") + interval);
  processes.push_back(p);
  p->setInterval(interval);
  p->setup();
}

void OS::killProcess(Process* p)
{
  std::vector<Process*>::iterator it = processes.begin();
  while (it != processes.end())
  {
    if (*it == p)
    {
      //Serial.println("erase");
      processes.erase(it);
      break;
    }
    ++it;
  }
}

void OS::handle()
{
  for (std::vector<Process*>::iterator it = processes.begin(); it != processes.end(); ++it)
  {
    //Serial.println(String("test ") + (*it)->getInterval());
    unsigned long currMillis = millis();
    if ((*it)->getLastRun() + (*it)->getInterval() < currMillis)
      (*it)->run();
  }
//  Serial.println();
}
