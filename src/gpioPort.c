#include "gpioPort.h"
#include "../include/sysctrl.h"


//*****************************************************************************
// Verifies that the base address is a valid GPIO base address
//*****************************************************************************
static bool verifyBaseAddr(uint32_t baseAddr)
{
   switch( baseAddr )
   {
     case GPIOA_BASE:
     case GPIOB_BASE:
     case GPIOC_BASE:
     case GPIOD_BASE:
     case GPIOE_BASE:
     case GPIOF_BASE:
     {
       return true;
     }
     default:
     {
       return false;
     }
   }
}

//*****************************************************************************
// Enabling the clock for the port that was passed in.
//*****************************************************************************
bool  gpio_enable_port(uint32_t baseAddr)
{
  uint32_t rcgc_mask = 0;
  uint32_t pr_mask = 0;
  
  // Verify that the base address and set rcgc_mask and pr_mask
	
	switch( baseAddr )
	{
	 case GPIOA_BASE:
	 {
		 // Set rcgc_mask and pr_mask for GPIO Port A
		 rcgc_mask = SYSCTL_RCGCGPIO_R0;
		 pr_mask= SYSCTL_PRGPIO_R0;
		 break;
	 }
	 case GPIOB_BASE:
	 {
		 // Set rcgc_mask and pr_mask for GPIO Port B
		 rcgc_mask = SYSCTL_RCGCGPIO_R1;
		 pr_mask= SYSCTL_PRGPIO_R1;
		 break;
	 }
	 case GPIOC_BASE:
	 {
		 // Set rcgc_mask and pr_mask for GPIO Port C
		 rcgc_mask = SYSCTL_RCGCGPIO_R2;
		 pr_mask= SYSCTL_PRGPIO_R2;
		 break;
	 }
	 case GPIOD_BASE:
	 {
		 // Set rcgc_mask and pr_mask for GPIO Port D
		 rcgc_mask = SYSCTL_RCGCGPIO_R3;
		 pr_mask= SYSCTL_PRGPIO_R3;
		 break;
	 }
	 case GPIOE_BASE:
	 {
		 // Set rcgc_mask and pr_mask for GPIO Port E
		 rcgc_mask = SYSCTL_RCGCGPIO_R4;
		 pr_mask = SYSCTL_PRGPIO_R4; 
		 break;
	 }
	 case GPIOF_BASE:
	 {
		 // Set rcgc_mask and pr_mask for GPIO Port F
		 rcgc_mask = SYSCTL_RCGCGPIO_R5;
		 pr_mask= SYSCTL_PRGPIO_R5;
		 break;
	 }
	 default:
	 {
		 return false;
	 }
	}
   
	// Turn the clock on using the rcgc_mask
	SYSCTL->RCGCGPIO |=rcgc_mask;
	
	// Wait for the PRGPIO to indicate the port is ready
	while((SYSCTL->PRGPIO & pr_mask)!=pr_mask){
		; //spin loop
	}
	
	// If PortD set the LOCK and CR registers
	if(baseAddr == GPIOD_BASE )
	{
		GPIO_PORTD_LOCK_R = 0x4C4F434B ;
		GPIO_PORTD_CR_R = 0xFF;
	}

	// If PortF set the LOCK and CR registers
	if(baseAddr == GPIOF_BASE )
	{
		GPIO_PORTF_LOCK_R = 0x4C4F434B ;
		GPIO_PORTF_CR_R = 0xFF;
	}

	return true;
}

//*****************************************************************************
// Setting a pins as a digital enable requires writing to DEN register
//
// Paramters
//    pins  -   A bit mask indicating which pins should be configured as digital
//              pins.  Modify only the bits where the bitmask is equal to 1.
//
// Use section 10.5 of the TM4C123 data sheet to determine the bits to set in 
// the DEN register.
//
//*****************************************************************************
bool  gpio_config_digital_enable(uint32_t baseAddr, uint8_t pins)
{
  GPIOA_Type  *gpioPort;

  // Verify that the base address is a valid GPIO base address
  // using the verifyBaseAddr function provided above
   if(!verifyBaseAddr(baseAddr))
		 return false;

  // Type Cast the base address to a GPIOA_Type pointer
   gpioPort = (GPIOA_Type *) baseAddr;

  // Turn on the digital enable
  gpioPort->DEN |=pins; 
	 
  return true;
}

//*****************************************************************************
// Setting a GPIO pin as an output requires setting the DIR register
//
// Paramters
//    pins  -   A bit mask indicating which pins should be configured as output
//              pins.  Modify only the bits where the bitmask is equal to 1.
//
// Use section 10.5 of the TM4C123 data sheet to determine the bits to set in 
// the DIR register.
//*****************************************************************************
bool  gpio_config_enable_output(uint32_t baseAddr, uint8_t pins)
{
  GPIOA_Type  *gpioPort;

  // Verify that the base address is a valid GPIO base address
  // using the verifyBaseAddr function provided above
   if(!verifyBaseAddr(baseAddr)){
		 return false;
	 }	 

  // Type Cast the base address to a GPIOA_Type pointer
	 gpioPort = (GPIOA_Type *) baseAddr;

  // Set the pins as outputs
	gpioPort->DIR |=pins;

  return true;
}

//*****************************************************************************
// Setting a GPIO pin as an output requires setting the DIR register
//
// Paramters
//    pins  -   A bit mask indicating which pins should be configured as output
//              pins.  Modify only the bits where the bitmask is equal to 1.
//
// Use section 10.5 of the TM4C123 data sheet to determine the bits to set in 
// the DIR register.
//
//*****************************************************************************
bool  gpio_config_enable_input(uint32_t baseAddr, uint8_t pins)

{
  GPIOA_Type  *gpioPort;

  // Verify that the base address is a valid GPIO base address
  // using the verifyBaseAddr function provided above
  if(!verifyBaseAddr(baseAddr)){
		 return false;
	 }	 

  // Type Cast the base address to a GPIOA_Type pointer
	gpioPort = (GPIOA_Type *) baseAddr;

  // Set the pins as inputs
	gpioPort->DIR &=~pins;

	return true;
}


//*****************************************************************************
// Enabling a pull-up resistor requires setting the PUR regsiter
//
// Paramters
//    baseAddr - Base address of GPIO port that is being enabled.
//    pins  -   A bit mask indicating which pins should be configured with a 
//              pull-up resistor
//*****************************************************************************
bool  gpio_config_enable_pullup(uint32_t baseAddr, uint8_t pins)
{
  GPIOA_Type  *gpioPort;

  // Verify that the base address is a valid GPIO base address
  // using the verifyBaseAddr function provided above
	if(!verifyBaseAddr(baseAddr)){
		 return false;
	 }	 

  // Type Cast the base address to a GPIOA_Type pointer  
	gpioPort = (GPIOA_Type *) baseAddr;

  // Enable the pull-up resistors
	gpioPort->PUR |=pins;
	 
  return true;
}

//*****************************************************************************
// Enabling a pull-up resistor requires setting the PDR regsiter
//
// Paramters
//    baseAddr - Base address of GPIO port that is being enabled.
//    pins  -   A bit mask indicating which pins should be configured with a 
//              pull-down resistor
//*****************************************************************************
bool  gpio_config_enable_pulldown(uint32_t baseAddr, uint8_t pins)
{
  GPIOA_Type  *gpioPort;

  // Verify that the base address is a valid GPIO base address
  // using the verifyBaseAddr function provided above
  if(!verifyBaseAddr(baseAddr)){
		return false;
	}	 
	
  // Type Cast the base address to a GPIOA_Type pointer
	gpioPort = (GPIOA_Type *) baseAddr;

  // Enable the pull-down resistors
	gpioPort->PDR |=pins;
	
  return true;
}

//*****************************************************************************
// Enables a pin as an analog input for the provided pins
//*****************************************************************************
bool gpio_config_analog_enable(uint32_t baseAddr, uint8_t pins){
	GPIOA_Type  *gpioPort;
	
	// Verify the address
	if(!verifyBaseAddr(baseAddr)){
		 return false;
	}
	
	// Set the AMSEL on selected pins
	gpioPort = (GPIOA_Type *) baseAddr;
	gpioPort->AMSEL |= pins;
	
	return true;	
}	

//*****************************************************************************
// Turns on the alternate function for the given pins
//*****************************************************************************
bool gpio_config_alternate_function(uint32_t baseAddr, uint8_t pins){
	GPIOA_Type  *gpioPort;
	
	// Verify the address
	if(!verifyBaseAddr(baseAddr)){
		 return false;
	}
	
	// Set the alternate function on selected pins
	gpioPort = (GPIOA_Type *) baseAddr;
	gpioPort->AFSEL |=pins;
	
	return true;	
}

//*****************************************************************************
// Configure pins to set the specified port control register (PCTL
//
// Paramters
//    baseAddr - Base address of GPIO port that is being enabled.
//    pins  -   A bit mask indicating which pins should be configured for the 
//              specified port control register.
//*****************************************************************************
bool  gpio_config_port_control(uint32_t baseAddr, uint32_t pins) {
	GPIOA_Type  *gpioPort;

  // Verify that the base address is a valid GPIO base address
  // using the verifyBaseAddr function provided above
	if(!verifyBaseAddr(baseAddr))
		return false;
    
  // Type Cast the base address to a GPIOA_Type pointer
	gpioPort = (GPIOA_Type *)baseAddr;
 
  // Set the pins as outputs
	gpioPort->PCTL |= pins;
	
	return true;
}

//*****************************************************************************
// Opens the drain for specified pins
//*****************************************************************************
bool gpio_config_open_drain(uint32_t baseAddr, uint32_t pins) {
	GPIOA_Type  *gpioPort;

  // Verify that the base address is a valid GPIO base address
  // using the verifyBaseAddr function provided above
	if(!verifyBaseAddr(baseAddr))
		return false;
    
  // Type Cast the base address to a GPIOA_Type pointer
	gpioPort = (GPIOA_Type *)baseAddr;
 
  // Set the pins as outputs
	gpioPort->ODR |= pins;
	
	return true;
}

//*****************************************************************************
// Configures the GPIO pins connected to the UP/DOWN and X-Direction of the 
// PS2 controller
//*****************************************************************************
void gpio_init(void){
		//Configures  GPIO pins for the UP/DOWN buttons
		gpio_enable_port(DIR_BTN_BASE);
		gpio_config_digital_enable(DIR_BTN_BASE,(DIR_BTN_UP | DIR_BTN_DOWN | DIR_BTN_LEFT | DIR_BTN_RIGHT));
		gpio_config_enable_input(DIR_BTN_BASE,(DIR_BTN_UP | DIR_BTN_DOWN | DIR_BTN_LEFT | DIR_BTN_RIGHT));
		gpio_config_enable_pullup(DIR_BTN_BASE,(DIR_BTN_UP | DIR_BTN_DOWN | DIR_BTN_LEFT | DIR_BTN_RIGHT));
	
		//Configures GPIO pins PS2 stick for the X-Direction
		gpio_enable_port(PS2_GPIO_BASE);
		gpio_config_analog_enable(PS2_GPIO_BASE, (PS2_X_DIR_PIN | PS2_Y_DIR_PIN));
		gpio_config_enable_input(PS2_GPIO_BASE, (PS2_X_DIR_PIN | PS2_Y_DIR_PIN));
		gpio_config_alternate_function(PS2_GPIO_BASE,(PS2_X_DIR_PIN | PS2_Y_DIR_PIN));
}	
