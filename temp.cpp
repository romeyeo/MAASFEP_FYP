#include "temp.hpp"

#include <OneWire.h>
#include <DallasTemperature.h>


SemaphoreHandle_t DS18B20_Mutex = xSemaphoreCreateMutex();
volatile float avgTemp = 0;

const int numAvg = 10;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 21 ;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

TaskHandle_t temp_loop_Handle = NULL;
void temp_loop(void * parameter);
void setup_AVGtemp(void);

void setup_temp(void){
  sensors.begin();
  delay(100);
   xTaskCreate(
    temp_loop,    // Function that should be called
    "Temp task",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    4,               // Task priority
    &temp_loop_Handle             // Task handle
  );
}

float getTemp(void){
  xSemaphoreTake( DS18B20_Mutex, portMAX_DELAY ); // new lines
  
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);

  xSemaphoreGive( DS18B20_Mutex ); // new lines
  
//  Serial.print("temperatureC ");
//  Serial.println(temperatureC);
  return temperatureC;
//  return sensors.getTempCByIndex(0); //check!!
}

//float Get_Avg_Temp(void){
// float sumTEMP = 0;
//    for (int i = 0; i < 10; i++)    //Average the 10 measurements to remove the glitch
//    {
//      sumTEMP += getTemp();
//      delay(1);
//    }
//    return sumTEMP / 10;
//     
//}

void print_temp(void){
//  sensors.requestTemperatures(); 
//  float temperatureC = sensors.getTempCByIndex(0);
//  float temperatureF = sensors.getTempFByIndex(0);
//  Serial.print(Get_Avg_Temp());
//  Serial.print(", ");
//  Serial.print(getTemp());
//  Serial.println("ºC");
//  Serial.print(temperatureF);
//  Serial.println("ºF");
}

void temp_loop(void * parameter){
  while(1){
    print_temp();
    delay(1000);
  }
  vTaskDelete(temp_loop_Handle);
}


TaskHandle_t tempAVG_loop_Handle = NULL;
void AVGtemp_loop(void * parameter);

void setup_AVGtemp(void){
//  Serial.println("random stuff 2");
   xTaskCreate(
    AVGtemp_loop,    // Function that should be called
    "avg Temp task",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    4,               // Task priority
    &tempAVG_loop_Handle             // Task handle
  );
}

float Get_Avg_Temp(void){
//  Serial.println("random stuff 3");
 return avgTemp;
}

void AVGtemp_loop(void * parameter){
  while(1){
//    Serial.println("random stuff 1");
   float sumTEMP = 0;
    for (int i = 0; i < numAvg; i++)    //Average the 50 measurements to remove the glitch, number is variable
    {
      sumTEMP += getTemp();
      delay(1000);
    }
    avgTemp = sumTEMP / (float)numAvg;
  }
  vTaskDelete(tempAVG_loop_Handle);
}
