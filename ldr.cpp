//#include "ldr.hpp"
////#include "RGB.hpp"
//
////added back
//const int LED_BLUE = 23 ;
//
//bool gotLight = false ; // ( a flag ) can rename.... got light, variable = true.
//int prevGotLight = false ; // Comparing current value with previous value.
//bool pgtDetect = false ; //  determining positive edge ( positive going transitions ) 
//unsigned long LDRtime = 0 ;  // default data type 
//int sensorValue = 0;        // value read from the potentio
//const int analogInPin = 36 ;  // Analog input pin that the potentiometer is attached to
//
//volatile float LDRfreq = 0 ;
//bool LDR_timeout = false;
//const float FreqThres = 5.0 ;
//
//TaskHandle_t ldr_loop_Handle = NULL;
//void ldr_loop(void * parameter);
//
//void setup_ldr(void){
//   xTaskCreate(
//    ldr_loop,    // Function that should be called
//    "LDR task",   // Name of the task (for debugging)
//    1000,            // Stack size (bytes)
//    NULL,            // Parameter to pass
//    6,               // Task priority
//    &ldr_loop_Handle             // Task handle
//  );
//  pinMode(LED_BLUE , OUTPUT); // Initializing pin                                 edited
//}
//
//void ldr_loop(void * parameter){
//  while(1){
//    // read the analog in value:
//    sensorValue = analogRead(analogInPin);
////    Serial.printf ("Current LDR value: %d\n",sensorValue );
//    Serial.print("Current LDR value:");
//    Serial.println(sensorValue );
//    if (sensorValue < 2550) { 
//    digitalWrite(LED_BLUE,HIGH) ;                                    //edited
////      showAnalogRGB(CRGB(0x0099ff)); //blue light
//      gotLight = false ; // no light
//    }
//    else {
//      digitalWrite(LED_BLUE , LOW) ;                                   //edited
////      showAnalogRGB(CRGB::Black); 
//      gotLight = true ; // got light 
//    }
//    
//    if ((prevGotLight == false) && (gotLight == true )){
//      pgtDetect = true ; // event detected 
//    }
//    prevGotLight = gotLight ; // ref point
//  
////     Serial.printf("%d %d %d\n",LDRtime,(1000.0/FreqThres),millis());
//Serial.println(1000.0/FreqThres);
//    if ((LDRtime + (1000.0/FreqThres)) < millis() ) { 
//      LDRfreq = 0;
//      LDR_timeout = true;
//    }
//  
//    if (pgtDetect == true){
//      pgtDetect = false; // respond to event & clear flag
//      LDRfreq = 1000 / (float)((millis() - LDRtime)) ; // millis = time A , LDRtime = time B and convert to seconds
//      Serial.print("Frequency Detected: ");
//      LDRtime = millis(); // ref point , millis is current time 
//      Serial.println(LDRfreq); // prints time since program started
//      LDR_timeout = false;
//    }
//    delay(1);
//  }
//  vTaskDelete(ldr_loop_Handle);
//}
#include "ldr.hpp"

int count = 0 ;
bool gotLight = false ; // ( a flag ) can rename.... got light, variable = true.
int prevGotLight = false ; // Comparing current value with previous value.
bool pgtDetect = false ; //  determining positive edge ( positive going transitions ) 
unsigned long LDRtime = 0 ;  // default data type 
const int LED_BLUE = 23 ;
int sensorValue = 0;        // value read from the potentio
const int analogInPin = 36 ;  // Analog input pin that the potentiometer is attached to

volatile float LDRfreq = 0 ;
volatile bool LDR_timeout = false;
const volatile float FreqThres = 5.0 ;

TaskHandle_t ldr_loop_Handle = NULL;
void ldr_loop(void * parameter);

void setup_ldr(void){
   xTaskCreate(
    ldr_loop,                          // Function that should be called
    "LDR task",                       // Name of the task (for debugging)
    1000,                              // Stack size (bytes)
    NULL,                            // Parameter to pass
    6,                                   // Task priority
    &ldr_loop_Handle                   // Task handle
  );    
  pinMode(LED_BLUE , OUTPUT);       /       / Initializing pin
}


//void print_ldr(void){
//// read the analog in value:
//  sensorValue = analogRead(analogInPin);
//  if (sensorValue < 700) { 
//    digitalWrite(LED_BLUE,HIGH) ;
//    gotLight = false ; // no light
//  }
//  else {
//    digitalWrite(LED_BLUE , LOW) ;
//    gotLight = true ; // got light 
//  }
//  
//  if ((prevGotLight == false) && (gotLight == true )){
//    pgtDetect = true ; // event detected 
//  }
//  prevGotLight = gotLight ; // ref point
//
////   Serial.printf("%d %d %d\n",LDRtime,(1000.0/FreqThres),millis());
//  if ((LDRtime + (1000.0/FreqThres)) < millis() ) { //
//    LDRfreq = 0;
//    LDR_timeout = true;
////    SeizureAlert(false);
//  }
//
//  if (pgtDetect == true){
//    pgtDetect = false; // respond to event & clear flag
//    LDRfreq = 1000 / (float)((millis() - LDRtime)) ; // millis = time A , LDRtime = time B and convert to seconds
//    Serial.print("Frequency Detected: ");
//    LDRtime = millis(); // ref point , millis is current time 
//    Serial.println(LDRfreq); // prints time since program started
//    LDR_timeout = false;
//  }


//}

void ldr_loop(void * parameter){
  while(1){
    // read the analog in value:
    sensorValue = analogRead(analogInPin);
//    Serial.printf ("Current LDR value: %d\n",sensorValue );
    if (sensorValue < 2550) { 
      digitalWrite(LED_BLUE,HIGH) ;
      gotLight = false ; // no light
    }
    else {
      digitalWrite(LED_BLUE , LOW) ;
      gotLight = true ; // got light 
    }
    
    if ((prevGotLight == false) && (gotLight == true )){
      pgtDetect = true ; // event detected 
    }
    prevGotLight = gotLight ; // ref point
  
  //   Serial.printf("%d %d %d\n",LDRtime,(1000.0/FreqThres),millis());
    if ((LDRtime + (1000.0/FreqThres)) < millis() ) { //
      LDRfreq = 0;
      LDR_timeout = true;
  //    SeizureAlert(false);
    }
  
    if (pgtDetect == true){
      pgtDetect = false; // respond to event & clear flag
      LDRfreq = 1000 / (float)((millis() - LDRtime)) ; // millis = time A , LDRtime = time B and convert to seconds
      Serial.print("Frequency Detected: ");
      LDRtime = millis(); // ref point , millis is current time 
      Serial.println(LDRfreq); // prints time since program started
      LDR_timeout = false;
    }
    delay(1);  //LDR & TEMP , CPU cant run things at same time,give cpu time to do other tasks . 
  }
  vTaskDelete(ldr_loop_Handle);
}
