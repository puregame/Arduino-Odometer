

#include <stdlib.h>
#include <Arduino.h>
#include <wiring.c>

unsigned long myMillis(void);

unsigned long resetLapTimer(void); // reset lap timer to 0 sec
unsigned long getLabTimerValue(void); // value in seconds
