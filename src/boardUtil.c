#include "TM4C123GH6PM.h"
#include "boardUtil.h"

uint8_t p1_wireless_addr[]  = {0x00, 0x01, 0x02, 0xBE, 0xEF};
uint8_t p2_wireless_addr[]  = {0x00, 0x01, 0x02, 0xDE, 0xAD};

void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

//*****************************************************************************
// Configure PA0 and PA1 to be UART pins
//*****************************************************************************
void uart0_config_gpio(void)
{
   gpio_enable_port(GPIOA_BASE);
   gpio_config_digital_enable( GPIOA_BASE, PA0 | PA1);
   gpio_config_alternate_function( GPIOA_BASE, PA0 | PA1);
   gpio_config_port_control( GPIOA_BASE, GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
}

//*****************************************************************************
//*****************************************************************************
void serialDebugInit(void)
{
  uart0_config_gpio();
  uart0_init_115K();
}

void init_board(void)
{
	DisableInterrupts();
	gpio_init();
	serialDebugInit();
	init_ADC();
	init_i2c();
	init_io_expander();
	init_timers();
	init_watchdog();
	lcd_set_pin_config(SSI0_BASE, GPIOC_BASE, PC7, GPIOA_BASE, PA4);
	init_lcd_gpio();
	lcd_initialize();
	init_rf();
	EnableInterrupts();
}
