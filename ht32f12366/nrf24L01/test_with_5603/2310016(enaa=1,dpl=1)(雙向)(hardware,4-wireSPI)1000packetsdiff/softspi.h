/*********************************************************************************************************//**
 * @file    softspi.h
 * @version $Rev:: 929          $
 * @date    $Date:: 2017-10-26 #$
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

/* Exported constants --------------------------------------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------------------------------------*/
#if !defined(_BV)
#define _BV(bit) (0x1u << bit)
#endif

#define nullptr_t (void*)0

/* Exported functions --------------------------------------------------------------------------------------*/
void init_softspi(void);
void SPI_MISO_INPUT_MDOE(void);
void SPI_MOSI_OUTPUT_MDOE(void);


typedef enum
{
    e_none,
    e_begin,
    e_end,
    e_normal,
    e_both
} SpiState;

#define SPI_DUMMY_DATA	0xff

void spi_SetCEState(bool state);
u8   spi_TransferData(u8 deta);
u8   spi_Write(u8 deta, SpiState state);
u8   spi_Read(SpiState state);

//NON-Configurable settings - DO NOT change these values
#define MAX_PIPES           6
#define MAX_DATA_SIZE       32
#define RX_TX_ADDR_LEN      5       // 3,4,5 bytes
#define RX_FULL_MASK        0x07

//Configurable settings
#define DEF_FILL_CHR        0
#define DEF_RF_CHAN         40    //channel
#define DEF_PAYLOAD_SIZE    32      //Max length 32
#define DEF_RETRY_COUNT     5       //15 is MAX
#define DEF_RETRY_DELAY     4       //in 250uS increments

#define RF24_CE_DDR         DDRH
#define RF24_CE_PORT        PORTH
#define RF24_CE_PIN         PH5

//Command mappings
#define R_REGISTER          0x00    // 000A AAAA
#define W_REGISTER          0x20    // 001A AAAA
#define R_RX_PAYLOAD        0x61
#define W_TX_PAYLOAD        0xA0
#define FLUSH_TX            0xE1
#define FLUSH_RX            0xE2
#define REUSE_TX_PL         0xE3
#define R_RX_PL_WID         0x60
#define W_ACK_PAYLOAD       0xA8    // 1010 1PPP
#define W_TX_PAYLOAD_NOACK  0xB0
#define NOP                 0xFF

#define ACTIVATE            0x50    // ???

//Register Map Table
#define CONFIG      0x00
#define             MASK_RX_DR  6
#define             MASK_TX_DS  5
#define             MASK_MAX_RT 4
#define             EN_CRC      3
#define             CRCO        2
#define             PWR_UP      1
#define             PRIM_RX     0
//Auto ACK enable for data pipes (def: all set)
// Bits 0-5
#define EN_AA       0x01
#define             ENAA_P0     0
#define             ENAA_P1     1
#define             ENAA_P2     2
#define             ENAA_P3     3
#define             ENAA_P4     4
#define             ENAA_P5     5
//RX Addresses enable (def: 0 & 1 enabled)
// Bits 0-5
#define EN_RXADDR   0x02
#define             ERX_P0      0
#define             ERX_P1      1
#define             ERX_P2      2
#define             ERX_P3      3
#define             ERX_P4      4
#define             ERX_P5      5
//Address width setup (def: 5 bytes)
#define SETUP_AW    0x03
//Setup of Auto Retransmit (def: 250uS ARD & 3 ARC)
#define SETUP_RETR  0x04
//RF Channel (def: F0 = 2400 + RF_CH[MHz] - 2402MHz)
#define RF_CH       0x05
//FR Setup register (def: 2Mbps, RF_PWR: 0dBm)
#define RF_SETUP    0x06
#define             RF_DR_LOW       5
#define             PLL_LOCK        4
#define             RF_DR_HIGH      3
#define             RF_PWR          0b00000110
//Status register
#define STATUS      0x07
#define             RX_DR           6
#define             TX_DS           5
#define             MAX_RT          4
#define             RX_P_NO_MASK    0b00001110
#define             TX_FULL         0
//Transmit observe register
#define OBSERVE_TX  0x08
//Received Power Detector - Carrier Detect
#define RPD_REG     0x09
#define             RPD             0
//Receive address data pipe
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
//Transmit address
#define TX_ADDR     0x10
//Number of bytes in RX payload for this pipe
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define             TX_REUSE        6
#define             TX_DS           5
#define             TX_EMPTY        4
#define             RX_FULL         1
#define             RX_EMPTY        0
#define DYNPD       0x1C
#define             DPL_P0          0
#define             DPL_P1          1
#define             DPL_P2          2
#define             DPL_P3          3
#define             DPL_P4          4
#define             DPL_P5          5
#define FEATURE     0x1D
#define             EN_DPL          2
#define             EN_ACK_PAY      1
#define             EN_DYN_ACK      0


typedef enum
{
    e_flush_recv,
    e_flush_xmit,
    e_flush_both
}FlushMode;

//TODO Not used right now, do I want to use?
typedef enum
{
    e_ds_not_configured,
    e_ds_idle,
    e_ds_RX,
    e_ds_TX
}DeviceState;

typedef enum
{
    e_crc_disabled,
    e_crc_8bit,
    e_crc_16bit
}CRCMode;

typedef enum
{
    e_dr_250Kbps,
    e_dr_1Mbps,
    e_dr_2Mbps
}RFDataRate;

typedef enum
{
    e_rf_neg18dBm = 0x00,
    e_rf_neg12bBm = 0x02,
    e_rf_neg6dBm  = 0x04,
    e_rf_0dBm     = 0x06
}RFPower;

typedef enum
{
    e_mode_powered_down,
    e_mode_standby,
    e_mode_recv,
    e_mode_xmit,
    e_mode_both        //This is for ResetStatus
}CommunicationMode;

typedef enum
{
    e_tr_unknown    = 0x00,
    e_tr_fifo_full  = 0x01,
    e_tr_maxrt      = 0x02,
    e_tr_buffered   = 0x04,
    e_tr_successful = 0x80,
    e_tr_nopipe     = 0xff
}TransmitStatus;

typedef enum
{
    e_rs_nodata,
    e_rs_overflow,
    e_rs_fifo_empty,
    e_rs_data
}ReceiveStaus;

typedef struct
{
    ReceiveStaus    status;
    u8              pipe_no;
    //This field is valid when using dynamic payloads
    u8              packet_sz;
}ReceiveResults;

typedef struct
{
    bool    active;
    u8      addr[5];
}Pipe, *pPipe;


void rf_Init(u8 rf_chan, u8 payload_sz, RFDataRate dr, bool use_dyn);
void rf_ConfigurePipe(u8 pipe_no, const u8 *addr);

u8 rf_SendCmd_1(u8 reg);                                 //W_REGISTER
u8 rf_SendCmd_2(u8 reg, u8 val);                         //W_REGISTER
u8 rf_SendCmdMasked(u8 reg, u8 mask, u8 data);           // first R_REGISTER, then W_REGISTER only the mask chosen bits
u8 rf_SendCmdBuffer(u8 reg, u8* buf, u8 len);            //W_REGISTER
u8 rf_ReadRegister(u8 reg);                              //R_REGISTER
u8 rf_ReadCmdBuffer(u8 reg, u8* buf, u8 len);            //R_REGISTER
u8 rf_ReceiveData(u8 *buf, u8 len);                      //R_RX_PAYLOAD
u8 rf_SendData(u8 pipe_no, unsigned char* data, u8 len); // if(different pipe) first W_REGISTER(TX_ADDR)
                                                         // if(different pipe) first W_REGISTER(RX_ADDR_P0)
                                                         //W_TX_PAYLOAD
u8 rf_SendAckPayload(u8 pipe_no, unsigned char* data, u8 len);

void rf_Listen(void);                                    //
void rf_StopListening(void);                             //
ReceiveResults rf_IsDataAvailable(void);                 //
TransmitStatus rf_WasDataSent(void);                     //
u8 rf_GetStatus(void);                                   //
u8 rf_GetFifoStatus(void);                               //R_REGISTER(17_FIFO_STATUS)
void rf_FlushBuffer(FlushMode mode);                     //FLUSH_TX
                                                         //FLUSH_RX
void rf_PowerUp(CommunicationMode mode);                 //R_REGISTER() W_REGISTER(00_CONFIG)     [00].[1]PWR_UP and .[0]PRIM_RX
void rf_PowerDown(void);                                 //R_REGISTER() W_REGISTER(00_CONFIG)     [00].[1]PWR_UP
u8   rf_SetPipeAddress(u8 pipe_no, const u8 *addr);
void rf_SetCRC(CRCMode mode);                            //R_REGISTER() W_REGISTER(00_CONFIG)     [00].[3]EN_CRC
                                                         //                                           .[2]CRC0
void rf_SetAddressWidth(u8 width);                       //             W_REGISTER(03_SETUP_AW)   [03].[1:0]AW
void rf_SetRetryCount(u8 arc);                           //R_REGISTER() W_REGISTER(04_SETUP_RETR) [04].[3:0]ARC
void rf_SetRetryDelay(u8 ard);                           //R_REGISTER() W_REGISTER(04_SETUP_RETR) [04].[7:4]ARD
void rf_SetDataRate(RFDataRate data_rate);               //R_REGISTER() W_REGISTER(06_RF_SETUP)   [06].[5]RF_DR_LOW
                                                         //                                           .[3]RF_DR_HIGH
void rf_ResetStatus(CommunicationMode mode);             //R_REGISTER() W_REGISTER(07_STATUS)     [07].[6:4]interrupt flags



#endif /* __SOFT_SPI_H ------------------------------------------------------------------------------------*/
