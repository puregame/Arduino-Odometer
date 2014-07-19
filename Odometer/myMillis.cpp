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


#define MILLIS_PER_TIMER2_OVF 32 //2* (1024*256) / 16 /1000 || prescaler * cycles per ovf / (clock / microseconds) / (millis / seconds)

#define FRACT_INC_TIMER2 ((MILLIS_PER_TIMER2_OVF % 1000) >> 3) // since ^^this^^ is an integer we need to account for the .348
												 // 125 happens to work out 
#define EXTRA_FRACT 125 // this will create a remainder of 21 to be added on each 125th overflow

#define MILLIS_PER_TIMER1_OVF 2 //2* (64*256 / 16 /1000 || prescaler * cycles per ovf / (clock / microseconds) / (millis / seconds)

#define FRACT_INC_TIMER1 2 // since ^^this^^ is an integer we need to account for the decimal part of the equation

ISR(TIMER2_OVF_vect) // timer 2 overflow vector, this is for stopwatch
{
  timer2_fraction += FRACT_INC_TIMER2; // increase the fractional element by specified amount
  timer2_millis += MILLIS_PER_TIMER2_OVF; // increase timer2 millis variable by appropriate amount
  if(timer2_fraction > EXTRA_FRACT){ // if we have reached 125 fractions, we need to increment millis by a little bit
  	timer2_fraction -= EXTRA_FRACT; // "reset" fraction value appropriately
  	timer2_millis += 18; // add fractional value, value found by trial and error
  }

}

ISR(TIMER1_OVF_vect) // timer 1 overflow vector, this is for main timer
{
  timer1_fraction += FRACT_INC_TIMER1;
  timer1_millis += MILLIS_PER_TIMER1_OVF;
  if(timer1_fraction > EXTRA_FRACT){
  	timer1_fraction -= EXTRA_FRACT;
  	timer1_millis += 3; // value found by trail and error
  }

}

void resetLapTimer(void){ // reset lap timer to 0 sec
	timer2_millis = 0;
}

unsigned long getLapTimerValue(void){ //get current timer2_millis, divide by 1000 to get seconds
        return timer2_millis; // /1000;
}

unsigned long currentLapTime(){
	return timer2_millis / 1000;
}

unsigned long myMillis()
{
	return timer1_millis;
}
