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

uint32_t colorVar= pixels.Color(  0,   0,   0);
uint32_t black  = pixels.Color(  0,   0,   0);
uint32_t green  = pixels.Color(  0, 255,   0);
uint32_t yellow = pixels.Color(255, 255,   0);
uint32_t red    = pixels.Color(255,   0,   0);
uint32_t white  = pixels.Color(255, 255, 255);
