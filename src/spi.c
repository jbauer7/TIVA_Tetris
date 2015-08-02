#include "../include/spi.h"
#include "../include/sysctrl.h"

/****************************************************************************
 * This routine transmits a character out the SPI1 port.
 ****************************************************************************/
bool 
spiVerifyBaseAddr(uint32_t base)
{
  if ( base == SSI0_BASE ||
        base == SSI1_BASE ||
        base == SSI2_BASE ||
        base == SSI3_BASE
  )
  {
    return true;
  }
  else
  {
    return false;
  }
}

//*****************************************************************************
// Configure the given SPI peripheral to be 10Mhz and the given SPI mode
//*****************************************************************************
bool initialize_spi( uint32_t base_addr, uint8_t spi_mode)
{
    SSI0_Type *mySSI = (SSI0_Type *)base_addr;
  
    // Validate that a correct base address has been passed
    // Turn on the Clock Gating Register
    switch (base_addr) 
    {
      case SSI0_BASE :
      {
          SYSCTL->RCGCSSI |= SYSCTL_RCGCSSI_R0;
          while ((SYSCTL->PRSSI & SYSCTL_PRSSI_R0) == 0){}    /* wait until SSI is ready */
          break;
      }
      case SSI1_BASE :
      {
          SYSCTL->RCGCSSI |= SYSCTL_RCGCSSI_R1;
          while ((SYSCTL->PRSSI & SYSCTL_PRSSI_R1) == 0){}    /* wait until SSI is ready */
          break;
      }
      case SSI2_BASE :
      {
          SYSCTL->RCGCSSI |= SYSCTL_RCGCSSI_R2;
          while ((SYSCTL->PRSSI & SYSCTL_PRSSI_R2) == 0){}    /* wait until SSI is ready */
          break;
      }
      case SSI3_BASE :
      {
          SYSCTL->RCGCSSI |= SYSCTL_RCGCSSI_R3;
          while ((SYSCTL->PRSSI & SYSCTL_PRSSI_R3) == 0){}    /* wait until SSI is ready */
          break;
      }
      default:
      {
          return false;
      }
    }
    
    // Disable the SSI interface (Set entire register to 0).
    mySSI->CR1 =0;

    // Set SPI clock to 10MHz for wireless and 25MHz for everything else.
    // Assume that we hvae a 50MHz clock
    // FSSIClk = FSysClk / (CPSDVSR * (1 + SCR))
		if(base_addr == SSI1_BASE)
			mySSI->CPSR = 5;
		else
			mySSI->CPSR = 2;
		
		mySSI->CR0  &= ~SSI_CR0_SCR_M;

    // Configure SPI control0 for freescale format, data width of 8 bits
    mySSI->CR0 =   (SSI_CR0_DSS_8 | SSI_CR0_FRF_MOTO);
    
    // Configure the SPI MODE in CR0
		mySSI->CR0 |= spi_mode;

    //Enable SSI
    mySSI->CR1 |= SSI_CR1_SSE;
   
	 
		return true;
}

//*****************************************************************************
//*****************************************************************************
void spiTx(uint32_t base, uint8_t *dataIn, int size, uint8_t *dataOut)
{
  uint8_t count = 0;
	SSI0_Type *mySSI = (SSI0_Type *) base;
  
  // Wait until the transmit is finished
  while((mySSI->SR & SSI_SR_TFE)!= 1){};

  // Disable the SSI interface
  mySSI->CR1 &= ~SSI_CR1_SSE;
    
  // Fill the Transmit FIFO  
  while((mySSI->SR & SSI_SR_TNF)!= 0 && (count < size) )
  {
    // Send out the first byte
    mySSI->DR = *dataIn; 
    dataIn++;
    count++;
  }
                  
  //Enable SSI
  mySSI->CR1 |= SSI_CR1_SSE;
  
  for( count = 0; count < size; count++)
  {
    // Wait until the recieve has finished  
    while((mySSI->SR & SSI_SR_RNE)==0){};// wait until response

    // Store the results  
    *dataOut =  mySSI->DR;
    dataOut++;
  }
}