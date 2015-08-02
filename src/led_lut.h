#ifndef __LED_LUT_H__
#define __LED_LUT_H__

#include <stdint.h>

#define LED0_ON     ~(1 << 0)
#define LED1_ON     ~(1 << 1)
#define LED2_ON     ~(1 << 2)
#define LED3_ON     ~(1 << 3)
#define LED4_ON     ~(1 << 4)
#define LED5_ON     ~(1 << 5)
#define LED6_ON     ~(1 << 6)
#define LED_ALL_OFF 0xFF

static uint8_t *next_piece;

static const uint8_t Led_LUT[7][5] = 
{
	{ // 0: I Shape
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON & LED6_ON,
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON & LED6_ON,
      LED_ALL_OFF, 
      LED_ALL_OFF, 
		  LED_ALL_OFF, 
   },
	  { // 1: J Shape
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED4_ON & LED5_ON, 
      LED4_ON & LED5_ON,
		  LED_ALL_OFF, 
   },
   { // 2: L Shape
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED0_ON & LED1_ON, 
      LED0_ON & LED1_ON,
			LED_ALL_OFF, 

   }, 
   { // 3: O Shape
			LED0_ON & LED1_ON & LED2_ON & LED3_ON, 
			LED0_ON & LED1_ON & LED2_ON & LED3_ON,
      LED0_ON & LED1_ON & LED2_ON & LED3_ON,
			LED0_ON & LED1_ON & LED2_ON & LED3_ON,
			LED_ALL_OFF,
   },
   { // 4: S Shape
      LED0_ON & LED1_ON & LED2_ON & LED3_ON, 
			LED0_ON & LED1_ON & LED2_ON & LED3_ON,
      LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED2_ON & LED3_ON & LED4_ON & LED5_ON,
			LED_ALL_OFF,
   },
   { // 5: Z Shape
      LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED2_ON & LED3_ON & LED4_ON & LED5_ON,
			LED0_ON & LED1_ON & LED2_ON & LED3_ON, 
			LED0_ON & LED1_ON & LED2_ON & LED3_ON,
      LED_ALL_OFF
	 },
   { // 6: T Shape
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED0_ON & LED1_ON & LED2_ON & LED3_ON & LED4_ON & LED5_ON,
      LED2_ON & LED3_ON, 
      LED2_ON & LED3_ON,
		  LED_ALL_OFF, 
   }
} ;


#endif
