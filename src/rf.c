#include "rf.h"

extern volatile int score, op_score;
int packets_sent, packets_dropped;

//*****************************************************************************
//*****************************************************************************
void init_rf(void)
{
  wireless_set_pin_config(
    RF_SPI_BASE,
    RF_PAYLOAD_SIZE,
    RF_CHANNEL,
    RF_CS_BASE,
    RF_CS_PIN,
    RF_CE_GPIO_BASE,
    RF_CE_PIN
  );
  
  gpio_enable_port(RF_GPIO_BASE);
  
  // Configure SPI CLK
  gpio_config_digital_enable(RF_GPIO_BASE, RF_CLK_PIN);
  gpio_config_alternate_function(RF_GPIO_BASE, RF_CLK_PIN);
  gpio_config_port_control(RF_GPIO_BASE, RF_CLK_PIN_PCTL);
  
  // Configure SPI MISO
  gpio_config_digital_enable(RF_GPIO_BASE, RF_MISO_PIN);
  gpio_config_alternate_function(RF_GPIO_BASE, RF_MISO_PIN);
  gpio_config_port_control(RF_GPIO_BASE, RF_MISO_PIN_PCTL);
  
  // Configure SPI MOSI
  gpio_config_digital_enable(RF_GPIO_BASE, RF_MOSI_PIN);
  gpio_config_alternate_function(RF_GPIO_BASE, RF_MOSI_PIN);
  gpio_config_port_control(RF_GPIO_BASE, RF_MOSI_PIN_PCTL);
  
  // Configure CS to be a normal GPIO pin that is controlled 
  // explicitly by software
  gpio_config_digital_enable(RF_CS_BASE,RF_CS_PIN);
  gpio_config_enable_output(RF_CS_BASE,RF_CS_PIN);
  
	// Configure IRQ to be a normal GPIO input pin
  gpio_config_digital_enable(RF_IRQ_GPIO_BASE, RF_IRQ_PIN);
  gpio_config_enable_input(RF_IRQ_GPIO_BASE, RF_IRQ_PIN);
	
	// Enable interrupts for IRQ
	GPIOD->IM |= RF_IRQ_PIN;	 // Unmask
	GPIOD->IS &= ~RF_IRQ_PIN;  // Edge sensitive
	GPIOD->IBE &= ~RF_IRQ_PIN; // Single Edge
	GPIOD->IEV &= ~RF_IRQ_PIN; // Falling Edge
	GPIOD->ICR |= RF_IRQ_PIN;	 // Clear Interrupts
	
	// Configure NVIC for RF interrupt
	NVIC_SetPriority(GPIOD_IRQn, 4);
	NVIC_EnableIRQ(GPIOD_IRQn);
	
  // Configure CE Pin as an output
  gpio_enable_port(RF_CE_GPIO_BASE);
  gpio_config_digital_enable(RF_CE_GPIO_BASE,RF_CE_PIN);
  gpio_config_enable_output(RF_CE_GPIO_BASE,RF_CE_PIN);
  
  initialize_spi(RF_SPI_BASE, 0);
    
  RF_CE_PERIH->DATA |= (1 << 1);
}

void handle_rf_interrupt(void){
	uint32_t data[10], i;
	wireless_com_status_t status;
	
	status = wireless_get_32(false, &data[0]);
	i = 1;
	
	if(status != NRF24L01_RX_SUCCESS)
		return;
	
	do {
		status = wireless_get_32(false, &data[i]);
		i++;
	} while(status == NRF24L01_RX_SUCCESS && i < 10);
	
	op_score = data[0];
	WATCHDOG0->ICR = 1; // Pet watchdog
}

void rf_send_score(void) {
	uint32_t status = wireless_send_32(false, false, score);
	
	if(status != NRF24L01_TX_SUCCESS) {
		packets_dropped++;
	}
	
	packets_sent++;
}

void print_rf_stats(void)
{
	printf("**********Wireless Stats**********\n");
	printf("       Packets Sent: %d\n", packets_sent);
	printf("      Packets Dropped: %d\n\n\n", packets_dropped);
}