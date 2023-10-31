/*********************************************************************************************************//**
 * @file    nrf2401_host.h
 * @version $Rev:: 929          $
 * @date    $Date:: 2017-10-26 #$
 * @brief   The header file of the nRF24L01 HOST functions.
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
#ifndef __NRF24L01_HOST_H
#define __NRF24L01_HOST_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported constants --------------------------------------------------------------------------------------*/

// <<< Use Configuration Wizard in Context Menu >>>

// <o> RF_CH 0~127, (2400+RF_CH)Mhz
//    <0-127:1>
#define DEF_RF_CHAN         41    //channel
// <o> Mode 
//    <0x02=> PTX
//    <0x03=> PRX
#define _HOST_TESTMODE_            2

// <o> Data Rate
//    <250=> 250K bps
//    <1000=>  1M bps
//    <2000=>  2M bps
#define DATARATE                   1000

// <o> Syncword
//    <3=> 3 bytes
//    <4=> 4 bytes
//    <5=> 5 bytes
#define SYNC_LEN                   5

// <o> CRC
//    <0x00=> disabled
//    <0x01=> 1 byte
//    <0x02=> 2 bytes
#define CRCFMT                     0x02

// <<< end of configuration section >>>

#define TRMODE_TX32            0x02        // ptx   (  0 ~  32 bytes)
#define TRMODE_RX32            0x03        // prx   (  0 ~  32 bytes)


/* Exported types ------------------------------------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------------------------------------*/
void host24L01_process(void);



#endif /* __NRF24L01_HOST_H ----------------------------------------------------------------------------------*/
