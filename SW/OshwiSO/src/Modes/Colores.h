#include "Arduino.h"

struct color
{
  byte r;
  byte g;
  byte b;
  byte w;
};

color colores[10] = {
  {  0,  0,  0},
  {  0,  0,255},
  {  0,255,  0},
  {  0,255,255},
  {255,  0,  0},
  {255,  0,255},
  {255,255,  0},
  {255,255,255},
  {128,  0,128},
  {255,165,  0}
};
