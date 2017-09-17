/*
 OS.h - This class manages the life cycle of the processes emulating an
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

#ifndef OS_H
#define OS_H

#include "Process.h"
#include <vector>

class OS {
public:
  void addProcess(Process*, unsigned int=0);
  void killProcess(Process*);
  void handle();

private:
  std::vector<Process*> processes;
};

#endif
