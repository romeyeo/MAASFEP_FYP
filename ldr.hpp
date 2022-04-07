#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

void setup_ldr(void);
void print_ldr(void);
extern volatile float LDRfreq;
extern volatile bool LDR_timeout;
extern volatile const float FreqThres;
#endif
