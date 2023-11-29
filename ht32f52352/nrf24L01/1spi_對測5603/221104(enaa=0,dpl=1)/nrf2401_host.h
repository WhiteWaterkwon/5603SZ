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

// <o> Mode 
//    <0x02=> PTX
//    <0x03=> PRX
#define _HOST_TESTMODE_            3

// <o> RF_CH 0~127, (2400+RF_CH)Mhz
//    <0-127:1>
#define DEF_RF_CHAN                83

// <o> Data Rate
//    <250=> 250K bps
//    <1000=>  1M bps
//    <2000=>  2M bps
#define DATARATE                   2000

// <o> Syncword
//    <3=> 3 bytes
//    <4=> 4 bytes
//    <5=> 5 bytes
#define SYNC_LEN                   5

// <o> CRC
/*    <0x00=> disabled  */
//    <0x01=> 1 byte  
//    <0x02=> 2 bytes 
#define CRCFMT                     0x02

// <o> RF power dBm
//    <0=> -18 dBm
//    <1=> -12 dBm
//    <2=>  -6 dBm
//    <3=>   0 dBm
#define RF_PWR_DBM                 3

// <o> FEATURE[0] EN_DYN_ACK
//    <0=> Disable
//    <1=> Enable
#define FEATURE_0_EN_DYN_ACK       0

// <o> PTX TOTAL_NUM_TEST_PKTS
//    <0=> infinitely
//    <1000=> 1000
#define TOTAL_NUM_TEST_PKTS        1000

// <o> pipe 0 Address [39:8]
//    <0x00000000-0xFFFFFFFF:0x1>
  #define P0_SYNCWORD           0xF2F1E223
// <o> pipe 0 Address [7:0]
//    <0x00-0xFF:0x1>
#define P0_LSBYTE                 0x7F
// <o> pipe 1 Address [39:8]
//    <0x00000000-0xFFFFFFFF:0x1>
  #define P1_SYNCWORD           0x78563412
// <o> pipe 1 Address [7:0]
//    <0x00-0xFF:0x1>
#define P1_LSBYTE                 0x71
// <o> pipe 2 Address [7:0]
//    <0x00-0xFF:0x1>
#define P2_LSBYTE                 0x72
// <o> pipe 3 Address [7:0]
//    <0x00-0xFF:0x1>
#define P3_LSBYTE                 0x73
// <o> pipe 4 Address [7:0]
//    <0x00-0xFF:0x1>
#define P4_LSBYTE                 0x74
// <o> pipe 5 Address [7:0]
//    <0x00-0xFF:0x1>
#define P5_LSBYTE                 0x75

// <<< end of configuration section >>>

// <o> RX_PW_P0
//    <0x00-0xFF:0x1>
#define RXPWP0                     0x01
// <o> RX_PW_P1
//    <0x00-0xFF:0x1>
#define RXPWP1                     0x02
// <o> RX_PW_P2
//    <0x00-0xFF:0x1>
#define RXPWP2                     0x0B
// <o> RX_PW_P3
//    <0x00-0xFF:0x1>
#define RXPWP3                     0x16
// <o> RX_PW_P4
//    <0x00-0xFF:0x1>
#define RXPWP4                     0x1F
// <o> RX_PW_P5
//    <0x00-0xFF:0x1>
#define RXPWP5                     0x20


/* Exported types ------------------------------------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------------------------------------*/
void host24L01_process(void);



#endif /* __NRF24L01_HOST_H ----------------------------------------------------------------------------------*/
