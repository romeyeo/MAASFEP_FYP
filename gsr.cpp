#include "gsr.hpp"

const int GSR = 39;
int gsrValue = 0;
int gsr_average = 0;



TaskHandle_t gsr_loop_Handle = NULL;
void gsr_loop(void * parameter);

void setup_gsr(void) {
  xTaskCreate(
    gsr_loop,    // Function that should be called
    "GSR task",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    &gsr_loop_Handle             // Task handle
  );
}

void gsr_loop(void * parameter) {
  while (1) {
    long sum = 0;
    for (int i = 0; i < 10; i++)    //Average the 10 measurements to remove the glitch
    {
      gsrValue = analogRead(GSR);
      sum += gsrValue;
      delay(5);
    }
    gsr_average = sum / 10;
//    Serial.println(gsr_average);
  }

vTaskDelete(gsr_loop_Handle);
}
