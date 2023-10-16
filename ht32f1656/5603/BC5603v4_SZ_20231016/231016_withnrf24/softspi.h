/*********************************************************************************************************//**
 * @file    softspi.h
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
 * @brief   The header file of the software-SPI functions.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) 2016 Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SOFT_SPI_H
#define __SOFT_SPI_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported types ------------------------------------------------------------------------------------------*/
typedef struct
{
    uint32_t byte4321 ;  // syncword
    uint8_t  byte0    ;  // LSByte, the LSByte must be unique for all 6 pipes
} address5602_TypeDef;

/* Exported constants --------------------------------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------------------------------------*/
extern volatile unsigned char CSNSelect;    // bit[2]:FPGA, bit[1]:ADC, bit[0]:RF
#define macroCSN_SELECT_RF   CSNSelect=0x01
#define macroCSN_SELECT_AD   CSNSelect=0x02
#define macroCSN_SELECT_FPGA CSNSelect=0x04


/* Exported functions --------------------------------------------------------------------------------------*/
void init_softspi(void);
void SPI_SDIO_INPUT_MDOE(void);
void SPI_SDIO_OUTPUT_MDOE(void);

void spi_w(unsigned char addr, unsigned char deta);
unsigned char spi_r(unsigned char addr);
void spi_cmd0(unsigned char addr);
void spi_cmdd_10_write_pipe_0_ID(address5602_TypeDef);
void spi_cmdd_12_write_pipe_1_ID(address5602_TypeDef);
void spi_cmdd_90_read_pipe_0_ID(address5602_TypeDef *);
void spi_cmdd_92_read_pipe_1_ID(address5602_TypeDef *);
void spi_cmdd_11_txfifo_write(unsigned int Length, unsigned char *pDeta);
void spi_cmdd_13_txfifo_write_no_ack(unsigned int Length, unsigned char *pDeta);
void spi_cmdd_18_txfifo_prx_w_ack_payload(unsigned char pipe, unsigned char Length, unsigned char *pDeta);
void spi_cmdd_9F_read_chip_version(unsigned char *pVer);





#endif /* __SOFT_SPI_H ------------------------------------------------------------------------------------*/
