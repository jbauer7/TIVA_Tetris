#ifndef __RF_H__
#define __RF_H__

#include "TM4C123GH6PM.h"
#include "boardUtil.h"
#include "../include/sysctrl.h"
#include "../include/spi.h"
#include "wireless.h"


//*****************************************************************************
// Fill out the #defines below to configure which pins are connected to
// the nordic wirless radio
//*****************************************************************************
#define   RF_GPIO_BASE       GPIOD_BASE
#define   RF_SPI_BASE        SSI1_BASE
#define   RF_CLK_PIN         PD0
#define   RF_MISO_PIN        PD2
#define   RF_MOSI_PIN        PD3

#define   RF_CLK_PIN_PCTL    GPIO_PCTL_PD0_SSI1CLK
#define   RF_MISO_PIN_PCTL   GPIO_PCTL_PD2_SSI1RX
#define   RF_MOSI_PIN_PCTL   GPIO_PCTL_PD3_SSI1TX

#define   RF_CS_BASE         GPIOD_BASE
#define   RF_CS_PIN          PD1

#define   RF_CE_GPIO_BASE     GPIOD_BASE
#define   RF_CE_PIN           PD6
#define   RF_CE_PERIH         GPIOD

#define   RF_IRQ_GPIO_BASE    GPIOD_BASE
#define   RF_IRQ_PIN          PD7

#define   RF_PAYLOAD_SIZE     0x04
#define   RF_CHANNEL          0x02

void init_rf(void);
void handle_rf_interrupt(void);
void rf_send_score(void);
void print_rf_stats(void);

#endif