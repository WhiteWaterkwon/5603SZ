/*********************************************************************************************************//**
 * @file    spi_cmd.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
 * @brief   This file provides all SPI command functions.
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

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "softspi.h"

/** @addtogroup SPI_Commands_label
 *  @{
 */


/* Private define ------------------------------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------------------------------------*/

/* Global functions ----------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
void spi_cmd0_20_set_register_bank(unsigned char bank)
{
    spi_cmd0(0x20 | bank);
}
void spi_cmd0_08_software_reset(void)
{
    spi_cmd0(0x08);
}
void spi_cmd0_29_reset_txfifo_index(void)  // ADV_SCAN_IND, SCAN_RSP
{
    spi_cmd0(0x29);
}
void spi_cmd0_09_txfifo_flush(void)
{
    spi_cmd0(0x09);
}
void spi_cmd0_89_rxfifo_flush(void)
{
    spi_cmd0(0x89);
}
void spi_cmd0_0E_TX(void)
{
    spi_cmd0(0x0E);
}
void spi_cmd0_8E_RX(void)
{
    spi_cmd0(0x8E);
}
void spi_cmd0_0A_deepsleep(void)
{
    spi_cmd0(0x0A);
}
void spi_cmd0_0F_middlesleep(void)
{
    spi_cmd0(0x0F);
}
void spi_cmd0_0C_lightsleep(void)
{
    spi_cmd0(0x0C);
}




/** @} */
