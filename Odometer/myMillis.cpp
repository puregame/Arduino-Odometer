// based on: https://github.com/arduino/Arduino/blob/cc6d7cdbd1f97aa90ffcb759b5fc4c5ba553304e/hardware/arduino/cores/robot/wiring.c
#ifndef Arduino_h
#include <wiring.c>
#endif

//TIMER2 variables
volatile unsigned long timer2_millis = 0;
volatile unsigned char timer2_fraction = 0;

//TIMER1 Variables
volatile unsigned long timer1_millis = 0;
volatile unsigned char timer1_fraction = 0;

// we don't use timer0, so the regular arduino Millis() function is unchanged.

#define EXTRA_FRACT 125 // this will create a remainder of 21 to be added on each 125th overflow

#define MILLIS_PER_TIMER1_OVF 2 //2* (64*256 / 16 /1000 || prescaler * cycles per ovf / (clock / microseconds) / (millis / seconds)

#define FRACT_INC_TIMER1 2 // since ^^this^^ is an integer we need to account for the decimal part of the equation


ISR(TIMER1_OVF_vect) // timer 1 overflow vector, this is for main timer
{
  timer1_fraction += FRACT_INC_TIMER1;
  timer1_millis += MILLIS_PER_TIMER1_OVF;
  timer2_millis += MILLIS_PER_TIMER1_OVF;
  if(timer1_fraction > EXTRA_FRACT){
  	timer1_fraction -= EXTRA_FRACT;
  	timer1_millis += 3; // value found by trail and error
    timer2_millis +=3;
  }

}

void resetLapTimer(void){ // reset lap timer to 0 sec
	timer2_millis = 0;
}

unsigned long getLapTimerValue(void){ //get current timer2_millis, divide by 1000 to get seconds
    return timer2_millis; // /1000;
}

float currentLapTime(){
  
	return float(timer2_millis) / 1000.;
}

unsigned long myMillis()
{
	return timer1_millis;
}
