#include "RGB.hpp"

//SemaphoreHandle_t RGB_Mutex = xSemaphoreCreateMutex();

#define LED_RED 32
#define LED_GREEN 33 
#define LED_BLUE 25 
#define LED_RED_CHANNEL 0
#define LED_GREEN_CHANNEL 1
#define LED_BLUE_CHANNEL 2
#define LED_FREQ 1000
#define LED_RES 8 //library support only 8

void setup_rgb(void) {
  ledcAttachPin(LED_RED,LED_RED_CHANNEL);
  ledcAttachPin(LED_GREEN,LED_GREEN_CHANNEL);
  ledcAttachPin(LED_BLUE,LED_BLUE_CHANNEL);
  ledcSetup(LED_RED_CHANNEL ,LED_FREQ ,LED_RES);
  ledcSetup(LED_GREEN_CHANNEL ,LED_FREQ ,LED_RES);
  ledcSetup(LED_BLUE_CHANNEL ,LED_FREQ ,LED_RES);
}

void showAnalogRGB( const CRGB& rgb)
{
//  xSemaphoreTake( RGB_Mutex, portMAX_DELAY );
  ledcWrite(LED_RED_CHANNEL,   255-rgb.r );
  ledcWrite(LED_GREEN_CHANNEL, 255-rgb.g );
  ledcWrite(LED_BLUE_CHANNEL,  255-rgb.b );
//  xSemaphoreGive( RGB_Mutex );
}
