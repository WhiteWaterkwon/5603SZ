/*********************************************************************************************************//**
 * @file    spi_cmd.h
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
 * @brief   The header file of the SPI command functions.
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
#ifndef __SPT_CMD_H
#define __SPT_CMD_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported types ------------------------------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------------------------------------*/
void spi_cmd0_20_set_register_bank(unsigned char bank);
void spi_cmd0_08_software_reset(void);
void spi_cmd0_29_reset_txfifo_index(void);
void spi_cmd0_09_txfifo_flush(void);
void spi_cmd0_89_rxfifo_flush(void);
void spi_cmdd_BF_rxfifo_read(unsigned char read_length, unsigned char *pch);
void spi_cmd0_0E_TX(void);
void spi_cmd0_8E_RX(void);
void spi_cmd0_0A_deepsleep(void);
void spi_cmd0_0F_middlesleep(void);
void spi_cmd0_0C_lightsleep(void);


#endif /* __SPT_CMD_H ------------------------------------------------------------------------------------*/
