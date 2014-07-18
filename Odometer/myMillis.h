
#ifndef Arduino_h
#include <stdlib.h>
#include <Arduino.h>
#include <wiring.c>
#endif

unsigned long myMillis(void);
unsigned long currentLapTime(void);

unsigned long timer2_fraction_d();


void resetLapTimer(void); // reset lap timer to 0 sec
unsigned long getLapTimerValue(void); // value in seconds