#include "watchdog.h"

// Initialize the watchdog timer so that it expires after a given
// tick count.  It also initializes the priority to a given value.
void init_watchdog(void) {
	SYSCTL->RCGCWD |= (0x01);
	
	while((SYSCTL->PRWD & 0x01) != 0x01)
		; // Spin wait
	
	WATCHDOG0->LOAD = 500000000;
	
	WATCHDOG0->CTL |= (WDT_CTL_INTEN | WDT_CTL_INTTYPE | WDT_CTL_RESEN);
	
	NVIC_SetPriority(WATCHDOG0_IRQn, 1);
	NVIC_EnableIRQ(WATCHDOG0_IRQn);
}
