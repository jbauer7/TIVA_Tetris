#ifndef __GPIO_PORT_F_H__
#define __GPIO_PORT_F_H__

#include <stdbool.h>
#include <stdint.h>
#include "TM4C123.h"
#include "../include/gpio.h"
#include "../include/sysctrl.h"

#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
	

#define   DIR_BTN_BASE    GPIOF_BASE
#define   PS2_GPIO_BASE   GPIOE_BASE

#define   DIR_BTN_UP      (1 << 1)
#define   DIR_BTN_RIGHT   (1 << 2)
#define   DIR_BTN_LEFT    (1 << 3)
#define   DIR_BTN_DOWN    (1 << 4)

#define   PS2_X_DIR_PIN   (1 << 2)
#define   PS2_Y_DIR_PIN   (1 << 3)

//*****************************************************************************
// Enabling the clock for the port that was passed in.
//*****************************************************************************
bool  gpio_enable_port(uint32_t baseAddr);

//*****************************************************************************
// Setting a pins as a digital enable requires writing to DEN register
//*****************************************************************************
bool  gpio_config_digital_enable(uint32_t baseAddr, uint8_t pins);

//*****************************************************************************
// Setting a GPIO pin as an output requires setting the DIR register
//*****************************************************************************
bool  gpio_config_enable_output(uint32_t baseAddr, uint8_t pins);

//*****************************************************************************
// Setting a GPIO pin as an input requires setting the DIR register
//*****************************************************************************
bool  gpio_config_enable_input(uint32_t baseAddr, uint8_t pins);

//*****************************************************************************
// Enabling a pull-up resistor requires setting the PUR regsiter
//*****************************************************************************
bool  gpio_config_enable_pullup(uint32_t baseAddr, uint8_t pins);

//*****************************************************************************
// Enabling a pull-up resistor requires setting the PDR regsiter
//*****************************************************************************
bool  gpio_config_enable_pulldown(uint32_t baseAddr, uint8_t pins);

//*****************************************************************************
// Enables a pin as an analog input for the provided pins
//*****************************************************************************
bool  gpio_config_analog_enable(uint32_t baseAddr, uint8_t pins);

//*****************************************************************************
// Turns on the alternate function for the given pins
//*****************************************************************************
bool  gpio_config_alternate_function(uint32_t baseAddr, uint8_t pins);

//*****************************************************************************
// Configure pins to set the specified port control register (PCTL
//
// Paramters
//    baseAddr - Base address of GPIO port that is being enabled.
//    pins  -   A bit mask indicating which pins should be configured for the 
//              specified port control register.
//*****************************************************************************
bool  gpio_config_port_control(uint32_t baseAddr, uint32_t pins);


//*****************************************************************************
// Opens the drain for specified pins
//*****************************************************************************
bool gpio_config_open_drain(uint32_t baseAddr, uint32_t pins);

//*****************************************************************************
// Configures the GPIO pins connected to the UP/DOWN and X-Direction of the 
// PS2 controller
//*****************************************************************************
void gpio_init(void);


#endif
