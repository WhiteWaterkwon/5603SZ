/*********************************************************************************************************//**
 * @file    bc5602_host.h
 * @version $Rev:: 101          $
 * @date    $Date:: 2018-10-26 #$
 * @brief   The header file of the BC5602 HOST functions.
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
#ifndef __BC5602_HOST_H
#define __BC5602_HOST_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported constants --------------------------------------------------------------------------------------*/

// <<< Use Configuration Wizard in Context Menu >>>

//<h> Board
// <o> RF_CH 0~127, (2400+RF_CH)Mhz
//    <0-127:1>
#define RFCH_FRONTEND              83
//</h>

// <o> SPI software or hardware
//    <0=> software SPI
//    <1=> hardware SPI
#define _SPI_MODE_                 1
// <o> SPI 3 or 4-wire
//    <3=> 3-wire SPI
//    <4=> 4-wire SPI
#define _SPI_WIRES_                4

// <o> Mode 
//    <0x04=> PTX
//    <0x05=> PRX
#define _HOST_TESTMODE_            5


// <o> Data Rate
//    <250=> 250 kbps
//    <1000=>  1 Mbps
//    <2000=>  2 Mbps
#define DATARATE                   250

// <o> Middle Sleep Time (unit: 50 us)
//    <0-1000000:1>
#define MIDDLE_SLEEP_TIME_UNIT_50US      80

// <o> XO2[2:0] XO_SEL
//    <0x01=> 12.8 MHz
//    <0x03=> 16 MHz
//    <0x04=> 19.2 MHz
#define XO_SEL                     0x03

//<h> Preamble
// <o> AMBLE2 [6] Preamble Length
//    <0x00=> 1 byte  Preamble
//    <0x40=> 2 bytes Preamble
#define AMBLE2                     0x00
//</h>

//<h> Syncword
// <o> DM1[5:4] Syncword Length
//    <0x01=> 3 bytes Syncword
//    <0x02=> 4 bytes Syncword
//    <0x03=> 5 bytes Syncword
#define DM1_AW                     0x03
// <o> pipe 0 Address [39:8]
//    <0x00000000-0xFFFFFFFF:0x1>
#if 0
	#define P0_SYNCWORD           0xF2E1F226
	#define P0_LSBYTE                 0x7F
#else
	#define P0_SYNCWORD            0xD14AF2EE
  #define P0_LSBYTE                 0x07
#endif 

// <o> pipe 0 LSByte
//    <0x00-0xFF:0x1>
  //#define P0_LSBYTE                 0x07
// <o> pipe 1 Address [39:8]
//    <0x00000000-0xFFFFFFFF:0x1>
#define P1_SYNCWORD          0xF2E1F623
//#define P1_SYNCWORD           0xf2f13412
//#define P1_SYNCWORD           0x76543412
// <o> pipe 1 LSByte
//    <0x00-0xFF:0x1>
#define P1_LSBYTE                 0x71
// <o> pipe 2 LSByte
//    <0x00-0xFF:0x1>
#define P2_LSBYTE                 0x72
// <o> pipe 3 LSByte
//    <0x00-0xFF:0x1>
#define P3_LSBYTE                 0x73
// <o> pipe 4 LSByte
//    <0x00-0xFF:0x1>
#define P4_LSBYTE                 0x74
// <o> pipe 5 LSByte
//    <0x00-0xFF:0x1>
#define P5_LSBYTE                 0x75
// <o> LAP0[31:0] (to generate Syncword)
//    <0x00000000-0xFFFFFFFF:0x1>
#define LAP0_TO_CALC_SYNC           0x001234
// <o> LAP1[31:0] (to generate Syncword)
//    <0x00000000-0xFFFFFFFF:0x1>
#define LAP1_TO_CALC_SYNC           0x005678
//</h>

//<h> TX
// <o> TX_NOACK_PKT 
//    <0x11=> Need ACK
//    <0x13=> NO need ACK
#define TX_NOACK_PKT          0x13
// <o> TX_PKT_CNT 
//    <0-100000>
#define TX_PKT_CNT            1000
//</h>

//<h> RX
// <o> RXG
//    <0x00-0xFF:0x1>
//      <i> RXG[1:0]  GAIN_LNA[1:0] 00:ultralow gain, 01:low gain, 10:middle gain, 11:high gain
//      <i> RXG[2]    GAIN_MX       0:low gain, 1:high gain
//      <i> RXG[4:3]  GAIN_BPF[1:0] 00:12dB, 01:18dB, 10:24dB, 11:24dB
//      <i> RXG[7:5]  GAIN_PGA[2:0] 000:0dB, 001:6dB, 010:12dB, 011:18dB, 100:24dB, 101:30dB, 110/111:36dB
#define RXG_GAIN                   0x77

// <o> CFG1[6] AGC_EN
//    <0x00=> Disabled
//    <0x40=> Enabled
#define CFG1_AGC_EN                0x40
//</h>


//<h> CRC
// <o> PKT1 [5]CRC_EN
//    <0x20=> CRC Field enabled
//    <0x00=> CRC Field disabled
#define PKT1_CRCEN                0x20
// <o> PKT1 [4]CRC8_EN
//    <0x00=> CRC_16
//    <0x10=> CRC_8
#define PKT1_CRC8EN            0x00
//</h>

//<h> DPL2
// <o> DPL2[0] enable dyn ack
//    <0x00=> Disabled
//    <0x01=> Enabled
#define DPL2_EN_DYN_ACK            0x01
// <o> DPL2[1] enable ack with payload
//    <0x00=> Disabled
//    <0x02=> Enabled
#define DPL2_EN_ACK_PLD            0x02
// <o> DPL2[2] enable DPL
//    <0x00=> Disabled
//    <0x04=> Enabled
#define DPL2_EN_DPL                0x04
// <o> DPL2[7] enable INV_NOACK
//    <0x00=> Disabled
//    <0x80=> Enabled
#define DPL2_EN_INV_NOACK          0x80
//</h>

//<h> DPL
// <o> DPL1[0] enable DPL_P0
//    <0x00=> Disabled
//    <0x01=> Enabled
#define DPL_P0                     0x01
// <o> DPL1[1] enable DPL_P1
//    <0x00=> Disabled
//    <0x02=> Enabled
#define DPL_P1                     0x02
// <o> DPL1[2] enable DPL_P2
//    <0x00=> Disabled
//    <0x04=> Enabled
#define DPL_P2                     0x04
// <o> DPL1[3] enable DPL_P3
//    <0x00=> Disabled
//    <0x08=> Enabled
#define DPL_P3                     0x08
// <o> DPL1[4] enable DPL_P4
//    <0x00=> Disabled
//    <0x10=> Enabled
#define DPL_P4                     0x10
// <o> DPL1[5] enable DPL_P5
//    <0x00=> Disabled
//    <0x20=> Enabled
#define DPL_P5                     0x20
//</h>


//<h> ENAA
// <o> ENAA[0] enable ENAA_P0
//    <0x00=> Disabled
//    <0x01=> Enabled
#define ENAA_P0                    0x01
// <o> ENAA[1] enable ENAA_P1
//    <0x00=> Disabled
//    <0x02=> Enabled
#define ENAA_P1                    0x02
// <o> ENAA[2] enable ENAA_P2
//    <0x00=> Disabled
//    <0x04=> Enabled
#define ENAA_P2                    0x04
// <o> ENAA[3] enable ENAA_P3
//    <0x00=> Disabled
//    <0x08=> Enabled
#define ENAA_P3                    0x08
// <o> ENAA[4] enable ENAA_P4
//    <0x00=> Disabled
//    <0x10=> Enabled
#define ENAA_P4                    0x10
// <o> ENAA[5] enable ENAA_P5
//    <0x00=> Disabled
//    <0x20=> Enabled
#define ENAA_P5                    0x20
//</h>

//<h> PEN (Pipe ACTIVE)
// <o> PEN[0] P0ACTIVE
//    <0x00=> Disabled
//    <0x01=> Enabled
#define P0ACTIVE                   0x01
// <o> PEN[1] P1ACTIVE
//    <0x00=> Disabled
//    <0x02=> Enabled
#define P1ACTIVE                   0x02
// <o> PEN[2] P2ACTIVE
//    <0x00=> Disabled
//    <0x04=> Enabled
#define P2ACTIVE                   0x04
// <o> PEN[3] P3ACTIVE
//    <0x00=> Disabled
//    <0x08=> Enabled
#define P3ACTIVE                   0x08
// <o> PEN[4] P4ACTIVE
//    <0x00=> Disabled
//    <0x10=> Enabled
#define P4ACTIVE                   0x10
// <o> PEN[5] P5ACTIVE
//    <0x00=> Disabled
//    <0x20=> Enabled
#define P5ACTIVE                   0x20
//</h>

//<h> Whitening
// <o> PKT2[7] Whitening
//    <0x00=> Disabled
//    <0x80=> Enabled
#define PKT2_WHT_EN                0x00
// <o> PKT2[6:0] Whitening Seed
//    <0x00-0x7F:0x1>
#define PKT2_WHT_SEED              0x00
// <o> PKT3[7] Whitening PCF
//    <0x00=> Disabled
//    <0x80=> Enabled
#define PKT3_WHT_PCF_EN            0x00
//</h>

// <<< end of configuration section >>>


#define _CODE_TRMODE_PTX_
#define _CODE_TRMODE_PRX_


#define TRMODE_PTX             0x04        // ptx
#define TRMODE_PRX             0x05        // prx


#define PAYLOAD_BUFF_SIZE      (32)


/* Exported types ------------------------------------------------------------------------------------------*/

#define SPITXQ_STATUS_EMPTY                                     0x00
#define SPITXQ_STATUS_LOADED_BUT_NOT_YET_PROCESSED_BY_HFSM1     0xFF
#define SPITXQ_STATUS_WAITING4_TX_COMPLETE                      0xF4
#define SPITXQ_TX_COMPLETE                                      0x01
/**
 *  @ingroup SPI_spiTxQ_label
 */
typedef struct
{
    unsigned char   remote_pipe_no;
    unsigned char   status;                         // 0x00:SPITXQ_STATUS_EMPTY
    unsigned int    total_length;                   // max 32
    unsigned char   payload[PAYLOAD_BUFF_SIZE];
} hfsm1_spiTxQ_to_5602_TypeDef;
extern hfsm1_spiTxQ_to_5602_TypeDef  spiTxQ;        // fsm2 ---> fsm1 spiTxQ ---spi---> local 5602 ---air---> remote 5602
/**
 *  @ingroup SPI_spiRxQ_label
 */
typedef struct
{
    unsigned int    w_index;                        //at final, it is total length
    unsigned char   payload[PAYLOAD_BUFF_SIZE];     //must be > 256 to support infinite fifo
} hfsm1_spiRxQ_from_5602_TypeDef;
extern hfsm1_spiRxQ_from_5602_TypeDef  spiRxQ;      // fsm2 <--- fsm1 spiRxQ <---spi--- local 5602 <---air--- remote 5602


/* Exported macro ------------------------------------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------------------------------------*/
void host5602_process(void);

void movedata_to_spiTxQ(unsigned char pipe_no, unsigned char total_length, unsigned char *pch);
void movedata_spiTxQ_to_ptx_txfifo(unsigned char strobeCmd);


void entry_hfsm1_ptx(void);
void exit_hfsm1_ptx(void);
void entry_hfsm1_prx(void);
void exit_hfsm1_prx(void);


#endif /* __BC5602_HOST_H ----------------------------------------------------------------------------------*/
