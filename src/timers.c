#include "timers.h"

#define SYSTICK_DELAY 125000 // 2.5 ms
#define TIMER0_DELAY 5000 // 20 ms

//*****************************************************************************
// Initializes SysTick
//*****************************************************************************
void systick_init(void) {
	SysTick_Config(SYSTICK_DELAY);
}

//*****************************************************************************
// Initializes Timer1
//*****************************************************************************
void timer1_init(void) {
  // Turn on the clock for the timer
  SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R1;

  // Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & SYSCTL_PRTIMER_R1) == 0)
		;
	
	// Setup TIMER1 to work how we want
	// - 16 bit, Periodic, Count Down, No Interrupts, triggers ADC measurement at 50hz
	TIMER1->CTL &= ~(0x00000101);
	TIMER1->CFG = 0x4;
	TIMER1->TAMR = 0x2;
	TIMER1->TAILR = TIMER0_DELAY;
	TIMER1->ICR = 0x001F;
	
	// Set the prescale so we can count to a million using a 16 bit timmer
	TIMER1->TAPR = 0xC7;
	
	// Re-enable the timer
	TIMER1->CTL |= (0x00000021);
}
  
//*****************************************************************************
// Initializes Timer0 but does not start the timer
//*****************************************************************************
void timer0_init(void) {
  // Turn on the clock for the timer
  SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R0;

  // Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & SYSCTL_PRTIMER_R0) == 0)
		;
	
	TIMER0->CTL &= ~(0x00000101);
	
	// Set up the timer to be 32 bit, count down, periodic, generating interrupts
	TIMER0->CFG = 0x0;
	TIMER0->TAMR = 0x2;
	TIMER0->IMR |= 0x01;
	
	// Set priority to be 2
	NVIC_SetPriority(TIMER0A_IRQn, 2);
	NVIC_EnableIRQ(TIMER0A_IRQn);
	
	// Setup the count, clear the interrupts and start the timer
	TIMER0->TAILR = 250000000;
	TIMER0->ICR = 0x001F;
	TIMER0->CTL |= (0x00000001);
}

void init_timers(void) {
	timer0_init();
	timer1_init();
	systick_init();
}
