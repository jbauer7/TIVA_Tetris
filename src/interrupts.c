#include "interrupts.h"
#include <stdbool.h>
#include "led.h"
#include "game_constants.h"

extern volatile bool AlertSysTick;
extern volatile bool AlertTimer0A;
extern volatile bool AlertADC0SS3;
extern volatile bool AlertADC0SS2;
extern volatile bool UpdateScreen;
extern volatile bool UpdateLED;
extern volatile bool DropFlag;
extern volatile bool RFFlag;
extern volatile bool SendScoreFlag;
extern volatile bool GameDoneFlag;
extern PC_Buffer UART0_Rx_Buffer;
extern PC_Buffer UART0_Tx_Buffer;
extern volatile int down_pressed_cnt;
extern volatile uint8_t down_pressed, down_y;

//*****************************************************************************
// Rx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Rx_Flow(PC_Buffer *rx_buffer)
{
  // Remove entries from the RX FIFO until the HW FIFO is empty.
  // Data should be placed in the rx_buffer.  
	char data;
  // Clear the RX interrupts so it can trigger again when the hardware 
  // FIFO becomes full
	while((UART0->FR & UART_FR_RXFE)==0x00){
		data=(UART0->DR & 0xFF);
		pc_buffer_add(rx_buffer, data);
	}	
	UART0->ICR  |=(UART_ICR_RXIC|UART_ICR_RTIC);
}

//*****************************************************************************
// Tx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Tx_Flow(PC_Buffer *tx_buffer)
{
      char c;
  
        // Check to see if we have any data in the circular queue
				while(!pc_buffer_empty(tx_buffer) && (UART0->FR & UART_FR_TXFF)!= UART_FR_TXFF){
            // Move data from the circuljkar queue to the hardware FIFO
            // until the hardware FIFO is full or the circular buffer
            // is empty.
						pc_buffer_remove(tx_buffer, &c);
						UART0->DR = c;
					
				}
            // Any data in the hardware FIFO will continue to be transmitted
            // but the TX empty interrupt will not be enabled since there
            // is no data in the circular buffer.

            // Disable the TX interrupts.
				if(pc_buffer_empty(tx_buffer))
					UART0->IM &=  ~(UART_IM_TXIM);
        
        // Clear the TX interrupt so it can trigger again when the hardware
        // FIFO is empty
				UART0->ICR |=0x20;
}

//*****************************************************************************
// UART0 Interrupt Service handler
//*****************************************************************************
void UART0_Handler(void)
{
    uint32_t  status;

    // Check to see if RXMIS or RTMIS is active
    status = UART0->MIS;

    if ( status & (UART_MIS_RXMIS | UART_MIS_RTMIS ) )
    {
       UART0_Rx_Flow(&UART0_Rx_Buffer);
    }

    // Check the TX interrupts
    if ( status & UART_MIS_TXMIS )
    {
      UART0_Tx_Flow(&UART0_Tx_Buffer);
    }
    return;
}

/*
*	 Set flag and clear interrupt for SysTick Interrupt
*/
int screen_count = 0;
int drop_count = 0;
int score_count = 0;
volatile int done_count = 0;

void SysTick_Handler(void) {
	int drop_threshold;
	uint32_t val;
	AlertSysTick = true;
	
	screen_count++;
	drop_count++;
	score_count++;
	done_count++;
	
	UpdateLED = 1;
	
	if(screen_count == 40) {
		screen_count = 0;
		UpdateScreen = 1;
	}
	
	if(done_count == 400 * GAME_LENGTH_SEC) {
		done_count = 0;
		GameDoneFlag = 1;
	}
	
	if(score_count == 400) {
		score_count = 0;
		SendScoreFlag = 1;
	}

	if(down_pressed || down_y) {
		drop_threshold = 20;
	} else {
		drop_threshold = 400;
	}
	
	if(drop_count >= drop_threshold) {
		drop_count = 0;
		DropFlag = 1;
	}

	val = SysTick->VAL;
}

/*
*	 Set flag and clear interrupt for TIMER1A Interrupt
*/ 
void TIMER0A_Handler(void) {
	AlertTimer0A = true;
	TIMER0->ICR = 0x001F;
}

/*
*	 Set flag and clear interrupt for ADC0SS3 Interrupt
*/ 
void ADC0SS3_Handler(void) {
	AlertADC0SS3 = true;
	ADC0->ISC |= 0x08;
}

/*
*	 Set flag and clear interrupt for ADC0SS3 Interrupt
*/ 
void ADC0SS2_Handler(void) {
	AlertADC0SS2 = true;
	ADC0->ISC |= 0x04;
}

/*
*	Inifite loop for the Watchdog Handler
*/
void WDT0_Handler(void) {
	while(1)
		; // Do nothing
}

void GPIOD_Handler(void) {
	RFFlag = 1;
	GPIOD->ICR |= RF_IRQ_PIN;	 // Clear Interrupts
}
