#ifndef PULSE_H
#define PULSE_H
  #include <Arduino.h>
  
  void setup_pulsesensor(void);
  void print_pulse(void);
  extern int myBPM;
  extern int avgBPM;
#endif
