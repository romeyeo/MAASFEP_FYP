#include "pulsesensor.hpp"

#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>

// GPIO where the PulseSensor is connected to
int PulseSensorPin = 34 ;
int myBPM = 0;
int P_Threshold = 2000;
int pulseValue = 0;

const int numAvgBPM = 10;
int avgBPM = 0;

PulseSensorPlayground pulseSensor;


TaskHandle_t pulsesensor_loop_Handle = NULL;
void pulsesensor_loop(void * parameter);
void pulsesensor_avg(void * parameter);

void setup_pulsesensor(void) {
  pulseSensor.analogInput(PulseSensorPin);
  pulseSensor.setThreshold(P_Threshold);
  if (!pulseSensor.begin()) {
    Serial.println("Failed to start pulse sensor");
  }
  xTaskCreate(
    pulsesensor_loop,    // Function that should be called
    "Pulse task",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    10,               // Task priority
    NULL             // Task handle
  );
  xTaskCreate(
    pulsesensor_avg,    // Function that should be called
    "Pulse task",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  );
}

//int getPulse(void) {
//  pulseValue = analogRead(PulseSensorPin);
//  pulseSensor.setThreshold(P_Threshold);
//}

void print_pulse(void) {
  // read the analog in value:
  myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
    Serial.print("BPM: ");                        // Print phrase "BPM: "
    Serial.println(myBPM);                        // Print the value inside of myBPM.
    //    delay(1000);
  }
}



void pulsesensor_loop(void * parameter) {
  while (1) {
    //    print_pulse();
    //    delay(1000);
    if (pulseSensor.sawNewSample()) {
      print_pulse();
    }
    delay(1);
    //  vTaskDelete(pulse_loop_Handle);
  }
}
void pulsesensor_avg(void * parameter) {
  while (1) {
    int sumBPM = 0;
    for (int i = 0; i < numAvgBPM; i++)    //Average the 10 measurements to remove the glitch, number is variable
    {
      sumBPM += myBPM;
      delay(1000);
    }
    avgBPM = sumBPM / numAvgBPM;
    Serial.print("avgBPM: ");                        // Print phrase "BPM: "
    Serial.println(avgBPM);                        // Print the value inside of myBPM.
  }
}
