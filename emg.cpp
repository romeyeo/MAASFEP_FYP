#include "emg.hpp"
//#include "RGB.hpp"

const int MusclePin = 35;
const float MuscleThres = 500.0 ;

const int LED_GREEN = 22 ;

unsigned long Muscletime = 0;
//unsigned long pulsewidth = 0;
bool gotMuscle = false ; // ( a flag ) can rename.... got light, variable = true.
int prevGotMuscle = false ; // Comparing current value with previous value.
bool pgtDetectMuscle = false ; //  determining positive edge ( positive going transitions )
bool ngtDetectMuscle = false ;

volatile bool Tonic_Seizure = false;

TaskHandle_t emg_loop_Handle = NULL;
void emg_loop(void * parameter);

void setup_emg(void){
   xTaskCreate(
    emg_loop,    // Function that should be called
    "EMG task",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    &emg_loop_Handle             // Task handle
  );
  pinMode(LED_GREEN , OUTPUT); // Initializing pin                                                edited
}

void emg_loop(void * parameter){
  while(1){
  // read the input on analog pin 0:
  float MuscleValue = analogRead(MusclePin);
  if (MuscleValue > 3000) {
    digitalWrite(LED_GREEN, HIGH) ;                                                              // edited
//    showAnalogRGB(CRGB(0x00ff00));  //   lime green
    gotMuscle = true ; // Muscle tension detected
  }
  else {
    digitalWrite(LED_GREEN , LOW) ;                                                          //      edited
//   showAnalogRGB(CRGB::Black);
    gotMuscle = false ; //
  }
  if (gotMuscle){
//      Serial.print("gotMuscle:");                                                                 edited
//      Serial.print(4000);
//      Serial.print(" ");
  }
  else{
//    Serial.print("gotMuscle:");                                                                  edited
//      Serial.print(0);
//      Serial.print(" ");
  }

  if ((prevGotMuscle == false) && (gotMuscle == true )) {
    pgtDetectMuscle = true ;                                      // event detected
  }
  if (pgtDetectMuscle){
//      Serial.print("pgtDetectMuscle:");
//      Serial.print(4000);
//      Serial.print(" ");
  }
  else{
//      Serial.print("pgtDetectMuscle:");
//      Serial.print(0);
//      Serial.print(" ");
  }

  if ((prevGotMuscle == true) && (gotMuscle == false )) {
    ngtDetectMuscle = true ;                                      // event detected
  }
  if (ngtDetectMuscle){
//      Serial.print("ngtDetectMuscle:");
//      Serial.print(2000);
//      Serial.print(" ");
  }
  else{
//      Serial.print("ngtDetectMuscle:");
//      Serial.print(0);
//      Serial.print(" ");
  }
  

  prevGotMuscle = gotMuscle ;               // ref point

  if (pgtDetectMuscle == true) {
    pgtDetectMuscle = false;                          // respond to event & clear flag
    Muscletime = millis();
  }
      
  if (ngtDetectMuscle == true) {                                        //    pulsewidth = (millis() - Muscletime);                                
    ngtDetectMuscle = false;                                                 // respond to event & clear flag
     Tonic_Seizure=false;
  }
  if ((float)(millis() - Muscletime) > MuscleThres && gotMuscle){
      Tonic_Seizure=true;
  }
  
//  Serial.print("Tonic_Seizure");
//  Serial.println(Tonic_Seizure);

//  Serial.print("Sensor_Value:");
//  Serial.println(MuscleValue);
  delay(1);
    }
//    delay(1);
//  }
  vTaskDelete(emg_loop_Handle);
}
