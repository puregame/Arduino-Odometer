// based on: https://github.com/arduino/Arduino/blob/cc6d7cdbd1f97aa90ffcb759b5fc4c5ba553304e/hardware/arduino/cores/robot/wiring.c

#include <wiring.c>

unsigned long timer2_overflow_cout = 0;


ISR(TIMER2_OVF_vect) // timer 2 overflow vector, this is for stopwatch
{
  
  //************BELOW IS TIMER0 STUFF FROM ARDUINO CODE******
  
  //copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
//************END******
}


unsigned long myMillis()
{
  
  //************BELOW IS TIMER0 STUFF FROM ARDUINO CODE******
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;

//************END******
}
