#include "adc.h"

//*****************************************************************************
// Initialize the ADC as specified in the specification document
//*****************************************************************************
bool initialize_PS2_X_ADC(void) {	
	// Turn on the ADC Clock
  SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;
  
  // Wait for ADCx to become ready
  while((SYSCTL->PRADC & SYSCTL_PRADC_R0) != SYSCTL_PRADC_R0)
		;
		
	// Configure the ADC to behave how we want
	// - triggered by timer 0
	// - Use SS 3
	// - 16 sample hardware averaged
	// - Generates interrupts
	ADC0->ACTSS &= ~(0x08);
	ADC0->SSMUX3 = 0x01;
	ADC0->EMUX = 0x55FF;
	ADC0->SAC = 0x4;
	ADC0->SSCTL3 |= (0x6);
	ADC0->IM |= 0x08;
	
	// Set the priority on the interrupts generated
	NVIC_SetPriority(ADC0SS3_IRQn, 1);
	NVIC_EnableIRQ(ADC0SS3_IRQn);
		
	// Re-enable the SS3
	ADC0->ACTSS |= (0x08);
	
	return true;
}

bool initialize_PS2_Y_ADC(void) {	
	// Turn on the ADC Clock
  SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;
  
  // Wait for ADCx to become ready
  while((SYSCTL->PRADC & SYSCTL_PRADC_R0) != SYSCTL_PRADC_R0)
		;
		
	// Configure the ADC to behave how we want
	// - triggered by timer 0
	// - Use SS 3
	// - 16 sample hardware averaged
	// - Generates interrupts
	ADC0->ACTSS &= ~(0x04);
	ADC0->SSMUX2 = 0x00;
	ADC0->EMUX = 0x55FF;
	ADC0->SAC = 0x4;
	ADC0->SSCTL2 |= (0x6);
	ADC0->IM |= 0x04;
	
	// Set the priority on the interrupts generated
	NVIC_SetPriority(ADC0SS2_IRQn, 1);
	NVIC_EnableIRQ(ADC0SS2_IRQn);
		
	// Re-enable the SS3
	ADC0->ACTSS |= (0x04);
	
	return true;
}

void init_ADC(void)
{
	initialize_PS2_X_ADC();
	initialize_PS2_Y_ADC();
}
