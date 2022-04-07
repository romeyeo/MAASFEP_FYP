#ifndef RGB_H
#define RGB_H
#include <Arduino.h>
#include <FastLED.h>


void setup_rgb(void);
void showAnalogRGB( const CRGB& rgb);
//void showAnalogRGB(  int r, int g, int b);
#endif
