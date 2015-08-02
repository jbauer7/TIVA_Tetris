#include "wireless.h"

extern void spiTx(uint32_t base, uint8_t *dataIn, int size, uint8_t *dataOut);
extern bool spiVerifyBaseAddr(uint32_t base);

NORDIC_CONFIG wirelessPinConfig;

//*****************************************************************************
//*****************************************************************************
void wait_uS_15(void)
{
  int i = 750;
  while(i >0)
  {
    i--;
  }
}

//*****************************************************************************
//*****************************************************************************
void wireless_set_pin_config(
  uint32_t  rf_base, 
  uint8_t   payload_size,
  uint8_t   channel,
  uint32_t  csn_base,
  uint8_t   csn_pin,
  uint32_t  ce_base,
  uint8_t   ce_pin
)
{
  wirelessPinConfig.rf_base = rf_base;
  wirelessPinConfig.payload_size = payload_size;
  wirelessPinConfig.channel = channel;
  wirelessPinConfig.csn_base = csn_base;
  wirelessPinConfig.csn_pin = csn_pin;
  wirelessPinConfig.ce_base = ce_base;
  wirelessPinConfig.ce_pin = ce_pin;
}

 //*****************************************************************************
//*****************************************************************************
static __INLINE void  wireless_CSN_low(void)
{
  GPIOA_Type *CSN_BASE = (GPIOA_Type *)wirelessPinConfig.csn_base;
  
  int i = 100;
  while(i >0)
  {
    i--;
  }
  CSN_BASE->DATA &= ~wirelessPinConfig.csn_pin;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE void  wireless_CSN_high(void)
{
  GPIOA_Type *CSN_BASE = (GPIOA_Type *)wirelessPinConfig.csn_base;
  int i = 100;
  while(i >0)
  {
    i--;
  }
  CSN_BASE->DATA |= wirelessPinConfig.csn_pin;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE void  wireless_CE_low(void)
{
  
  GPIOA_Type *CE_BASE = (GPIOA_Type *)wirelessPinConfig.ce_base;
  CE_BASE->DATA &= ~wirelessPinConfig.ce_pin;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE void  wireless_CE_high(void)
{
  
  GPIOA_Type *CE_BASE = (GPIOA_Type *)wirelessPinConfig.ce_base;
  CE_BASE->DATA |= wirelessPinConfig.ce_pin;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE void  wireless_CE_Pulse(void)
{
  wireless_CE_high();
  
  wait_uS_15();
  
  wireless_CE_low();
}

//*****************************************************************************
// This function reads a single byte of data from the register at the 5-bit 
// address specificed by the lower 5 bits of paramter reg. 
//
// The value at that register is returned to the  user as a uint8_t.  
//
// Page 51 of the data sheet lists the supported commnads for the nRF24L01+.  
// The first two entries entries describe how to read/write a single byte of 
// data to a register.
//
// Use spiTx() to send the data via the SPI interface.  This function can be
// found in spi.c.  You will also need to use wireless_CSN_low() and 
// wireless_CSN_high() to manually set the SPI chip select.
//*****************************************************************************
static __INLINE uint8_t wireless_reg_read(uint8_t reg)
{
	uint8_t din[2];
	uint8_t dout[2];
	
	din[0] = (reg & 0x1F);
	din[1] = 0xFF;

	wireless_CSN_low();
	
	spiTx(SSI1_BASE, din, 2, dout);
	
	wireless_CSN_high();
	
	return dout[1];
}

//*****************************************************************************
// This function writes a single byte of data from the register at the 5-bit 
// address specificed by the lower 5 bits of paramter reg. 
//  
// Page 51 of the data sheet lists the supported commnads for the nRF24L01+.  
// The first two entries entries describe how to read/write a single byte of 
// data to a register.
//
// Use spiTx() to send the data via the SPI interface.  This function can be
// found in spi.c.  You will also need to use wireless_CSN_low() and 
// wireless_CSN_high() to manually set the SPI chip select.
//*****************************************************************************
static __INLINE void wireless_reg_write(uint8_t reg, uint8_t data)
{
	uint8_t din[2];
	uint8_t dout[2];
	
	din[0] = ((reg & 0x1F) | 0x20);
	din[1] = data;

	wireless_CSN_low();
	
	spiTx(SSI1_BASE, din, 2, dout);
	
	wireless_CSN_high();
}

//*****************************************************************************
// This function writes 5 bytes of data to the TX_ADDR register found on page
// 60 of the nRF24L01+ data sheet.  
//
// Use spiTx() to send the data via the SPI interface.  This function can be
// found in spi.c.  You will also need to use wireless_CSN_low() and 
// wireless_CSN_high() to manually set the SPI chip select.
//*****************************************************************************
static __INLINE uint32_t wireless_set_tx_addr(uint8_t  *tx_addr)
{
  uint8_t din[6];
	uint8_t dout[6];
	
	din[0] = ((0x10 & 0x1F) | 0x20);
	din[1] = tx_addr[0];
	din[2] = tx_addr[1];
	din[3] = tx_addr[2];
	din[4] = tx_addr[3];
	din[5] = tx_addr[4];

	wireless_CSN_low();
	
	spiTx(SSI1_BASE, din, 6, dout);
	
	wireless_CSN_high();
}

//*****************************************************************************
// This function writes 4 bytes of data to the nRF24L01+ Tx FIFO using the 
// W_TX_PAYLOAD command found on page 51 of the nRF24L01+ datasheet.
//
// Use spiTx() to send the data via the SPI interface.  This function can be
// found in spi.c.  You will also need to use wireless_CSN_low() and 
// wireless_CSN_high() to manually set the SPI chip select.
//*****************************************************************************
static __INLINE void wireless_tx_data_payload( uint32_t data)
{
	uint8_t din[5];
	uint8_t dout[5];
	
	din[0] = 0xA0;
	din[1] = data & 0x000000FF;
	din[2] = (data & 0x0000FF00) >> 8;
	din[3] = (data & 0x00FF0000) >> 16;
	din[4] = (data & 0xFF000000) >> 24;
	

	wireless_CSN_low();
	
	spiTx(SSI1_BASE, din, 5, dout);
	
	wireless_CSN_high();
}


//*****************************************************************************
// This function reads 4 bytes of data from the nRF24L01+ Tx FIFO using the 
// R_RX_PAYLOAD command found on page 51 of the nRF24L01+ datasheet.
//
// Use spiTx() to send the data via the SPI interface.  This function can be
// found in spi.c.  You will also need to use wireless_CSN_low() and 
// wireless_CSN_high() to manually set the SPI chip select.
//*****************************************************************************
static __INLINE void wireless_rx_data_payload( uint32_t *data)
{
	uint8_t din[5];
	uint8_t dout[5];
	
	din[0] = 0x61;
	din[1] = 0xFF;
	din[2] = 0xFF;
	din[3] = 0xFF;
	din[4] = 0xFF;
	
	wireless_CSN_low();
	
	spiTx(SSI1_BASE, din, 5, dout);
	
	wireless_CSN_high();

	*data = 0;
	*data |= dout[1];
	*data |= dout[2] << 8;
	*data |= dout[3] << 16;
	*data |= dout[4] << 24;
}

//****************************************************************************
//*****************************************************************************
static __INLINE int32_t wireless_set_rx_addr(
  uint8_t  *rx_addr,
  uint8_t   pipe
)
{
  uint8_t dataIn[6];
  uint8_t dataOut[6];
  uint8_t wireless_reg = 0;
  
  switch(pipe)
  {
    case 0: { wireless_reg = NRF24L01_RX_ADDR_P0_R; break; }
    case 1: { wireless_reg = NRF24L01_RX_ADDR_P1_R; break; }
    case 2: { wireless_reg = NRF24L01_RX_ADDR_P2_R; break; }
    case 3: { wireless_reg = NRF24L01_RX_ADDR_P3_R; break; }
    case 4: { wireless_reg = NRF24L01_RX_ADDR_P4_R; break; }
    case 5: { wireless_reg = NRF24L01_RX_ADDR_P5_R; break; }
    default: {return -1;}
  }

  dataIn[0] = wireless_reg |  NRF24L01_CMD_W_REGISTER   ;
  dataIn[1] = *(rx_addr +0 );
  dataIn[2] = *(rx_addr +1 );
  dataIn[3] = *(rx_addr +2 );
  dataIn[4] = *(rx_addr +3 );
  dataIn[5] = *(rx_addr +4 );
  wireless_CSN_low();
  spiTx(wirelessPinConfig.rf_base,dataIn, 6, dataOut);
  wireless_CSN_high();
  return 0;
}


//*****************************************************************************
//*****************************************************************************
static __INLINE void wireless_flush_tx_fifo( void)
{
  uint8_t dataIn[1];
  uint8_t dataOut[1];
  
    dataIn[0] = NRF24L01_CMD_FLUSH_TX;
    wireless_CSN_low();
    spiTx(wirelessPinConfig.rf_base,dataIn, 1, dataOut);
    wireless_CSN_high();
}


//*****************************************************************************
//*****************************************************************************
static __INLINE void wireless_flush_rx_fifo( void )
{
  uint8_t dataIn[1];
  uint8_t dataOut[1];
  
    dataIn[0] = NRF24L01_CMD_FLUSH_RX;
    wireless_CSN_low();
    spiTx(wirelessPinConfig.rf_base,dataIn, 1, dataOut);
    wireless_CSN_high();
}


//*****************************************************************************
//*****************************************************************************
static __INLINE void wireless_start_tx_mode( void )
{
  uint8_t config_reg;
  
  // Read in the current config
  config_reg = wireless_reg_read( NRF24L01_CONFIG_R);
  
  // Clear PRIM_RX
  wireless_reg_write(NRF24L01_CONFIG_R, config_reg & ~NRF24L01_CONFIG_PRIM_RX_PRX);  
}

//*****************************************************************************
//*****************************************************************************
static __INLINE void wireless_start_rx_mode( void)
{
  uint8_t config_reg;
  
  // Read in the current config
  config_reg = wireless_reg_read( NRF24L01_CONFIG_R);
  
  // Clear PRIM_RX
  wireless_reg_write(NRF24L01_CONFIG_R, config_reg | NRF24L01_CONFIG_PRIM_RX_PRX); 
}

//*****************************************************************************
//*****************************************************************************
static __INLINE bool wireless_rx_fifo_empty (void )
{
  uint8_t status = wireless_reg_read(NRF24L01_FIFO_STATUS_R);
  if( (status & NRF24L01_FIFO_STATUS_RX_EMPTY_M ) != 0)
  {
    return true;
  }
  else
  {
     return false;
  }
}



//*****************************************************************************
//*****************************************************************************
static __INLINE bool wireless_status_rx_dr_asserted(uint8_t status)
{
  if ( (status & NRF24L01_STATUS_RX_DR_M) != 0)
    return true;
  else
    return false;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE bool wireless_status_tx_ds_asserted(uint8_t status)
{
  if ( (status & NRF24L01_STATUS_TX_DS_M) != 0)
    return true;
  else
    return false;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE bool wireless_status_max_rt_asserted(uint8_t status)
{
  if ( (status & NRF24L01_STATUS_MAX_RT_M) != 0)
    return true;
  else
    return false;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE bool wireless_status_tx_full_asserted(uint8_t status)
{
  if ( (status & NRF24L01_STATUS_TX_FULL_M) != 0)
    return true;
  else
    return false;
}

//*****************************************************************************
//*****************************************************************************
static __INLINE uint8_t wireless_get_status( void )
{
  uint8_t dataIn[1];
  uint8_t dataOut[1];
  
  dataIn[0] = NRF24L01_CMD_NOP;
  wireless_CSN_low();
  spiTx(wirelessPinConfig.rf_base,dataIn, 1, dataOut);
  wireless_CSN_high();
  
  return dataOut[0];
}


//*****************************************************************************
//*****************************************************************************
static __INLINE void wireless_clear_max_rt( void )
{
  wireless_reg_write( NRF24L01_STATUS_R, NRF24L01_STATUS_MAX_RT_M);
}

//*****************************************************************************
//*****************************************************************************
static __INLINE void wireless_clear_tx_ds( void )
{
  wireless_reg_write( NRF24L01_STATUS_R, NRF24L01_STATUS_TX_DS_M);
}

//*****************************************************************************
//*****************************************************************************
static __INLINE bool wireless_wait_for_tx_ds( void )
{
  uint8_t status =  wireless_get_status() ;
  
  while( (wireless_status_tx_ds_asserted(status)==false) && (wireless_status_max_rt_asserted(status))==false)
   {
      status =  wireless_get_status() ;
   }
    
    // Indicate if the data was sucessfully sent
    if(wireless_status_tx_ds_asserted(status))
    {
      return true;
    }
    else
    {
      return false;
    }
    
}

//*****************************************************************************
//*****************************************************************************
static  __INLINE void wireless_wait_for_rx_dr( void )
{
   uint8_t status = wireless_get_status() ;
  
   while( wireless_status_rx_dr_asserted(status) == false)
   {
      status = wireless_get_status() ;
   }
   
    // Clear the RX_DR bit
    wireless_reg_write( NRF24L01_STATUS_R, NRF24L01_STATUS_RX_DR_M); 
   
}



//*****************************************************************************
// Public Functions
//*****************************************************************************

//*****************************************************************************
//*****************************************************************************
wireless_com_status_t 
wireless_send_32(
  bool      blockOnFull,
  bool      retry,
  uint32_t  data
)
{
  uint8_t status;
  
  if( spiVerifyBaseAddr(wirelessPinConfig.rf_base))
  {
    // Check the status of the device
    status = wireless_get_status();
    
    if( wireless_status_tx_full_asserted(status) && (blockOnFull == false))
    {
       return NRF24L01_TX_FIFO_FULL;
    }
    
    // Wait while the TX FIFO is not full 
    while(wireless_status_tx_full_asserted(status))
    {
        status = wireless_get_status();
    }
    
    do
    {
      // Put into Standby-1
      wireless_CE_low();
      
      // Set tx_mode
      wireless_start_tx_mode();
      
      // Flush any outstanding info in the TX FIFO
      wireless_flush_tx_fifo();
      
      // Send the data to the TX_PLD
      wireless_tx_data_payload(data);
      
      // Pulse CE for a 15uS
      wireless_CE_Pulse();
      
       status = wireless_wait_for_tx_ds();
      
       if( status == false)
       {
         printf(" ** ERR ** MAX_RT exceeded\n\r");
         wireless_clear_max_rt();

       }
       else
       {
          // Clear the tx_ds bit
          wireless_clear_tx_ds();
       }
    } while( status == false && retry == true);
    

     // Set PRIM_RX High
      wireless_start_rx_mode();
      
      // Enable Wireless transmission
      wireless_CE_high();
    
    if (status == true)
    {
      return NRF24L01_TX_SUCCESS;
    }
    else
    {
      return NRF24L01_TX_PCK_LOST;
    }
  }
  else
  {
    return NRF24L01_ERR;
  }
  
}

//*****************************************************************************
//*****************************************************************************
wireless_com_status_t
wireless_get_32(
  bool      blockOnEmpty,
  uint32_t  *data
)
{
  //uint8_t status;
  if( spiVerifyBaseAddr(wirelessPinConfig.rf_base))
  {

    if( wireless_rx_fifo_empty() == false)
    {
      // Read data from Rx FIFO
      wireless_rx_data_payload(data);
      
      // If Rx FIFO is empty, clear the RX_DR bit in the status
      // register
      if ( wireless_rx_fifo_empty() ==  true)
      {
         // Clear the RX_DR bit
          wireless_reg_write(NRF24L01_STATUS_R, NRF24L01_STATUS_RX_DR_M);
      }
    
      return NRF24L01_RX_SUCCESS;
    }
    else if ( (wireless_rx_fifo_empty() == true) && blockOnEmpty)
    {
      
     // Wait until the RX_DR bit is set
      wireless_wait_for_rx_dr();
      
      // Read data from Rx FIFO
      wireless_rx_data_payload( data);
      
      // If Rx FIFO is empty, clear the RX_DR bit in the status
      // register
      if ( wireless_rx_fifo_empty() ==  true)
      {
         // Clear the RX_DR bit
          wireless_reg_write(NRF24L01_STATUS_R, NRF24L01_STATUS_RX_DR_M);
      }
      
      return NRF24L01_RX_SUCCESS;
    }
    else
    {
      return NRF24L01_RX_FIFO_EMPTY;
    }
  }
  else
  {
    return NRF24L01_ERR;
  }
}

//*****************************************************************************
//*****************************************************************************
bool wireless_configure_device( 
  uint8_t           *my_id,
  uint8_t           *dest_id
)
{
  if( spiVerifyBaseAddr(wirelessPinConfig.rf_base))
  {
    wireless_CSN_high();
    wireless_CE_low();
    
    // Configure Common RF settings
    wireless_flush_tx_fifo();
    wireless_flush_rx_fifo();
    wireless_reg_write(NRF24L01_RF_SETUP_R, NRF24L01_RF_SETUP_RF_PWR_0DB | NRF24L01_RF_SETUP_250_KBPS);
    wireless_reg_write(NRF24L01_RF_CH_R, wirelessPinConfig.channel);
    wireless_reg_write( NRF24L01_STATUS_R, NRF24L01_STATUS_CLEAR_ALL);
    wireless_reg_write(NRF24L01_SETUP_RETR_R, NRF24L01_SETUP_RETR_ARD_0750_US | NRF24L01_SETUP_RETR_ARC_15);
    
    // Configure the address to transfer data to
    wireless_set_tx_addr(dest_id);
    
    // Configure Pipe 0 to receive the AUTO ACKs from the other device
    wireless_reg_write(NRF24L01_RX_PW_P0_R, wirelessPinConfig.payload_size);
    wireless_set_rx_addr(dest_id, 0);
      
    // Configure Pipe 1
    wireless_reg_write(NRF24L01_RX_PW_P1_R, wirelessPinConfig.payload_size);
    wireless_set_rx_addr(my_id, 1);
    
    // Turn on Rx and AutoAcks for pipe 0 and 1
    wireless_reg_write(NRF24L01_EN_RXADDR_R, NRF24L01_RXADDR_ERX_P0 | NRF24L01_RXADDR_ERX_P1); 
    wireless_reg_write(NRF24L01_EN_AA_R, NRF24L01_ENAA_P0 | NRF24L01_ENAA_P1);

    // Enable the Radio in RX mode
    wireless_reg_write(NRF24L01_CONFIG_R,NRF24L01_CONFIG_PWR_UP | NRF24L01_CONFIG_EN_CRC | NRF24L01_CONFIG_PRIM_RX_PRX );
      
    wireless_CE_high();
    return true;
  }
  else
  {
    return false ;
  }
  
} 


