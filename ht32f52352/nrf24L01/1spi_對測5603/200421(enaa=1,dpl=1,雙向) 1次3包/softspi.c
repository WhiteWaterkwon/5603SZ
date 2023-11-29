/*********************************************************************************************************//**
 * @file    softspi.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2017-10-26 #$
 * @brief   This file provides all software-SPI functions.
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
#include "ht32_board_config.h"
#include "nrf2401_host.h"
#include "usart.h"                    //debug
#include "softspi.h"


// <o> SPI software or hardware
//    <1=> hardware 4-wire SPI
#define _SPI_MODE_                 1


/* Private define ------------------------------------------------------------------------------------------*/
#define SPI_CE_SET_1        GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_1, SET)
#define SPI_CE_RESET_0      GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_1, RESET)

#define SPI_CSN_SET_1       GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_3, SET)
#define SPI_CSN_RESET_0     GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_3, RESET)
void init_softspi(void)
{
    SPI_CE_RESET_0; //CE is active high and used to activate the chip in RX or TX mode
              AFIO_GPxConfig( GPIO_PA, AFIO_PIN_1, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_1, GPIO_DIR_OUT);

    SPI_CSN_SET_1;
              AFIO_GPxConfig( GPIO_PA, AFIO_PIN_3, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_3, GPIO_DIR_OUT);
}

/* Private variables ---------------------------------------------------------------------------------------*/
static volatile u8   _payload_sz = DEF_PAYLOAD_SIZE;
static volatile bool _p0_swapped_out = FALSE;
static volatile bool _use_dynamic_payload = FALSE;
static volatile CommunicationMode _comm_mode = e_mode_powered_down;

Pipe  _pipes[6] = {{ TRUE,  { 0xb7, 0x19, 0x51, 0x95, 0xe7}},
                   { TRUE,  { 0x48, 0xe1, 0xf2, 0xf1, 0xc2}},
                   { TRUE,  { 0x48, 0xe1, 0xf2, 0xf1, 0xc3}},
                   { TRUE,  { 0x48, 0xe1, 0xf2, 0xf1, 0xc4}},
                   { TRUE,  { 0x48, 0xe1, 0xf2, 0xf1, 0xc5}},
                   { TRUE,  { 0x48, 0xe1, 0xf2, 0xf1, 0xc6}}};


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   
 * @retval  
 ************************************************************************************************************/
void spi_SetCEState(bool state)
{
    if (state)
        SPI_CE_SET_1;   //CE is active high and used to activate the chip in RX or TX mode
    else
        SPI_CE_RESET_0; //CE is active high and used to activate the chip in RX or TX mode
}
u8 spi_TransferData(u8 deta)
{
    #if _SPI_MODE_ == 1 // 0:software spi,   1:hardware spi
        HT_SPI0->DR = deta;  //SPI_SendData(HT_SPI0, octet);
        while( ((HT_SPI0->SR & SPI_FLAG_TXE)==(u32)RESET)?(SET):(RESET) );  //while (!SPI_GetFlagStatus(HT_SPI0, SPI_FLAG_TXE));
        return HT_SPI0->DR ;
    #endif
}
u8 spi_Write(u8 deta, SpiState state)
{
    volatile unsigned char result;
    if (state == e_both || state == e_begin)
    {
        SPI_CSN_RESET_0;
    }
    
    result = spi_TransferData(deta);
    
    if (state == e_both || state == e_end)
    {
        SPI_CSN_SET_1;
    }
    
    return result;
}
u8 spi_Read(SpiState state)
{
    volatile unsigned char result;
    if (state == e_both || state == e_begin)
    {
        SPI_CSN_RESET_0;
    }
    
    result = spi_TransferData(SPI_DUMMY_DATA);
    
    if (state == e_both || state == e_end)
    {
        SPI_CSN_SET_1;
    }
        
    return result;
}

/*********************************************************************************************************//**
 * @brief   
 * @retval  
 ************************************************************************************************************/
void rf_Init(u8 rf_chan, u8 payload_sz, RFDataRate dr, bool use_dyn)
{
    _payload_sz = payload_sz;

    if (use_dyn || (!use_dyn && payload_sz <= 32))
    {
        //If dynamic or data count >= 16 we need to set delay to 500uS
        rf_SetRetryCount(0);
      //rf_SetRetryDelay(3);   3 not large enough for 250Kbps remote PRX nrf51
        rf_SetRetryDelay(6); //5 not large enough for 250Kbps remote PRX 6502 ack_payload length 31,32
        if (use_dyn)
        {
            _use_dynamic_payload = TRUE;
            //Enable dynamic payload Length field
            rf_SendCmdMasked(FEATURE, _BV(EN_DPL), _BV(EN_DPL));
        }
        else
        {
            _use_dynamic_payload = FALSE;
            //Disable dynamic payload Length field
            rf_SendCmdMasked(FEATURE, _BV(EN_DPL), 0x00);
        }

            #if FEATURE_0_EN_DYN_ACK == 1
            rf_SendCmdMasked(FEATURE, _BV(EN_DYN_ACK), _BV(EN_DYN_ACK));
            #else
            rf_SendCmdMasked(FEATURE, _BV(EN_DYN_ACK), 0x00);
            #endif
    }
    
    rf_SendCmdMasked(FEATURE, _BV(EN_ACK_PAY), _BV(EN_ACK_PAY));
  //rf_SendCmdMasked(FEATURE, _BV(EN_ACK_PAY), 0);
    
    //Set to 1Mbps
    rf_SetDataRate(dr);
    
    //Set to 16bit CRC mode, in code also enables.
    #if   CRCFMT == 0x00
    rf_SetCRC(e_crc_disabled); // e_crc_disabled, e_crc_8bit, e_crc_16bit
    #elif CRCFMT == 0x01
    rf_SetCRC(e_crc_8bit);     // e_crc_disabled, e_crc_8bit, e_crc_16bit
    #else
    rf_SetCRC(e_crc_16bit);    // e_crc_disabled, e_crc_8bit, e_crc_16bit
    #endif
    
    //Set the Rf channel freq = 2400[MHz] + rf_ch[MHz]
    rf_SendCmd_2(RF_CH, rf_chan);
}
void rf_ConfigurePipe(u8 pipe_no, const u8 *addr, u8 payload_sz)
{
    //Pipe number must be between 0 and 5!
    if (pipe_no >= MAX_PIPES)
        return;
    
    //If nullptr use default address for the pipe
    if (addr != nullptr_t)
        rf_SetPipeAddress(pipe_no, addr);
    
    rf_SetAddressWidth(SYNC_LEN);//(RX_TX_ADDR_LEN);
    
    //Enable data pipe
    rf_SendCmdMasked(EN_RXADDR, _BV(pipe_no), _BV(pipe_no));
    rf_SendCmdMasked(EN_AA,     _BV(pipe_no), _BV(pipe_no));
  //rf_SendCmdMasked(EN_AA,     _BV(pipe_no), 0x00        );
  //rf_SendCmdMasked(EN_AA,             0x3F, 0x00        );//If any bit in the EN_AA is high, will enable EN_CRC forced high

    //Set the pipes payload size or enable dynamic size for pipe
    if (_use_dynamic_payload) {
        rf_SendCmdMasked(DYNPD, _BV(pipe_no), _BV(pipe_no));
    }
    else {
        rf_SendCmdMasked(DYNPD, _BV(pipe_no), 0x00);
        rf_SendCmd_2(RX_PW_P0 + pipe_no, payload_sz);
    }
    
    //Set the pipe at pupe_no to active, meaning it's been configured.
    _pipes[pipe_no].active = TRUE;
}

/*********************************************************************************************************//**
 * @brief   
 * @retval  
 ************************************************************************************************************/
u8 rf_SendCmd_1(u8 reg)
{
//uart_puts("rf_SendCmd_1, reg="); uart_putu8(reg);//debug
//uart_putchar_n('\n');//debug
    return spi_Write(W_REGISTER | reg, e_both);
}
u8 rf_SendCmd_2(u8 reg, u8 val)
{
    u8 result;
//uart_puts("rf_SendCmd_2, reg="); uart_putu8(reg);//debug
//uart_puts(", val=");             uart_putu8(val);//debug
//uart_putchar_n('\n');//debug
    result = spi_Write(W_REGISTER | reg, e_begin);
    spi_Write(val, e_end);
    return result;
}
u8 rf_SendCmdMasked(u8 reg, u8 mask, u8 data)
{
    u8 result;
//uart_puts("rf_SendCmdMasked, reg="); uart_putu8(reg);//debug
//uart_puts(", mask=");                uart_putu8(mask);//debug
//uart_puts(", data=");                uart_putu8(data);//debug
    result = rf_ReadRegister(reg);
//uart_puts(", r=");                   uart_putu8(result);//debug
    result = (result & ~mask) | data;
//uart_puts(", w=");                   uart_putu8(result);//debug
//uart_putchar_n('\n');//debug
    result = rf_SendCmd_2(reg, result);
    return result;
}
u8 rf_SendCmdBuffer(u8 reg, u8* buf, u8 len)
{
    u8 result, i;
//uart_puts("rf_SendCmdBuffer, reg="); uart_putu8(reg);//debug
//uart_puts(", len="); uart_putu8(len);//debug
//uart_puts(", written bytes[]="); 
    result = spi_Write(W_REGISTER | reg, e_begin);
    for (i = 1; i <= len; i++)
    {
        //got through the buffer in reverse direction since data needs to be written LSByte first
        if (i == len) {
            spi_Write(*(buf + len - i), e_end);
//uart_puts(" "); uart_putu8(buf[len-i]);//debug
        }
        else {
            spi_Write(*(buf + len - i), e_normal);
//uart_puts(" "); uart_putu8(buf[len-i]);//debug
        }
    }
//uart_putchar_n('\n');//debug
    return result;
}

u8 rf_ReadRegister(u8 reg)
{
    u8 result;
    spi_Write(R_REGISTER | reg, e_begin);
    result = spi_Read(e_end);
    return result;
}
u8 rf_ReadCmdBuffer(u8 reg, u8* buf, u8 len)
{
    u8 result, i;
    result = spi_Write(R_REGISTER | reg, e_begin);
    for (i = 1; i <= len; i++)
    {
        if (i == len)
            *(buf + len - i) = spi_Read(e_end);
        else
            *(buf + len - i) = spi_Read(e_normal);
    }
    return result;
}
u8 rf_ReceiveData(u8 *buf, u8 len)
{
    u8 result, i;
    result = spi_Write(R_RX_PAYLOAD, e_begin);
    for (i = 0; i < len; i++)
    {
        if (i == (len - 1))
            *(buf + i) = spi_Write(NOP, e_end);
        else
            *(buf + i) = spi_Write(NOP, e_normal);
    }
    //Reset status register
  //rf_ResetStatus(e_mode_recv);
    return result;
}
static volatile u8   _current_pipe = 0xff;
u8 rf_SendData(u8 pipe_no, unsigned char* data, u8 len)
{
    u8 i;
    volatile TransmitStatus status = e_tr_unknown; //???

    if (pipe_no >= MAX_PIPES || !_pipes[pipe_no].active)
    return e_tr_nopipe;

    //Set the P0, TX buffers and buffer length
//  if (_current_pipe != pipe_no)
    {
        rf_SendCmdBuffer(TX_ADDR,    (u8 *)&_pipes[pipe_no].addr, RX_TX_ADDR_LEN);
        rf_SendCmdBuffer(RX_ADDR_P0, (u8 *)&_pipes[pipe_no].addr, RX_TX_ADDR_LEN); // ??? assume pipe 0
        _current_pipe = pipe_no;
        _p0_swapped_out = TRUE;
    }
    
    //Send the data by writing the W_TX_PAYLOAD first to tell the device we are
    //    going to sending the data next.
    spi_Write(W_TX_PAYLOAD, e_begin);       // Write cmd to write payload
		//spi_Write(W_TX_PAYLOAD_NOACK, e_begin); // Write cmd to write payload
    for (i = 0; i < len; i++)
    {
        volatile u8 chr;
        //If the length of the buffer is less than the packet size pad the buffer.
        if (i < len)
            chr = *(data + i);
        else
            chr = DEF_FILL_CHR; // ???
        
        if (i == (len - 1))
            spi_Write(chr, e_end);
        else
            spi_Write(chr, e_normal);
    }
    
    return status;
}
u8 rf_SendAckPayload(u8 pipe_no, unsigned char* data, u8 len)
{
    u8 i;
    volatile TransmitStatus status = e_tr_unknown; //???

    spi_Write(W_ACK_PAYLOAD | (pipe_no&0x07), e_begin); // Write cmd to write payload
    for (i = 0; i < len; i++)
    {
        volatile u8 chr;
        //If the length of the buffer is less than the packet size pad the buffer.
        if (i < len)
            chr = *(data + i);
        else
            chr = DEF_FILL_CHR; // ???
        
        if (i == (len - 1))
            spi_Write(chr, e_end);
        else
            spi_Write(chr, e_normal);
    }
    
    return status;
}
void rf_Listen(void)
{
//  if (_p0_swapped_out)
    {
        rf_SendCmdBuffer(RX_ADDR_P0, (u8 *)&_pipes[0].addr, RX_TX_ADDR_LEN);
        _p0_swapped_out = FALSE;
    }
    
    //Begin listening
    spi_SetCEState(TRUE);
    
    _comm_mode = e_mode_recv;
}
void rf_StopListening(void)
{
    spi_SetCEState(FALSE);
    
    _comm_mode = e_mode_standby;
}
ReceiveResults rf_IsDataAvailable(void)
{
    ReceiveResults result;
    u8 status;
    status = rf_GetFifoStatus();
    if (!(status & _BV(RX_EMPTY)))
    {
        result.status =  e_rs_data;
        
        result.pipe_no = (rf_GetStatus() >> 1) & 0x07;
        
        if (_use_dynamic_payload)
        {
            result.packet_sz = rf_ReadRegister(R_RX_PL_WID);    //TEST Not sure about this
            if (result.packet_sz > MAX_DATA_SIZE)
            {
                result.status = e_rs_overflow;
                result.packet_sz = 0;
            }
        }
        else
        {
            if     (result.pipe_no==0) result.packet_sz = RXPWP0;
            else if(result.pipe_no==1) result.packet_sz = RXPWP1;
            else if(result.pipe_no==2) result.packet_sz = RXPWP2;
            else if(result.pipe_no==3) result.packet_sz = RXPWP3;
            else if(result.pipe_no==4) result.packet_sz = RXPWP4;
            else if(result.pipe_no==5) result.packet_sz = RXPWP5;
        }
    }
    else
    {
          //result.status = e_rs_fifo_empty;
            result.status = e_rs_nodata;
    }
    
    return result;
}
TransmitStatus rf_WasDataSent(void)
{
    u8 status;
    TransmitStatus result = e_tr_unknown;
    
    status = rf_GetStatus();

    if (status == 0xff)
        result = e_tr_unknown;
    else if (status & _BV(TX_DS))
        result = e_tr_successful;
    else if (status & _BV(MAX_RT))
        result = e_tr_maxrt;
    else if (status & _BV(TX_FULL))
        result |= e_tr_fifo_full;
    
    return result;
}
u8 rf_GetStatus(void)
{
    return rf_SendCmd_1(NOP);
}
u8 rf_GetFifoStatus(void)
{
    return rf_ReadRegister(FIFO_STATUS);
}
void rf_FlushBuffer(FlushMode mode)
{
    if (mode == e_flush_recv || mode == e_flush_both)
        rf_SendCmd_1(FLUSH_RX);
    
    //TEST Do I really need this?
  //TimerDelay_us(5);  todo
    
    if (mode == e_flush_xmit || mode == e_flush_both)
        rf_SendCmd_1(FLUSH_TX);
}
void rf_PowerUp(CommunicationMode mode)
{
    _comm_mode = mode;
    
    if (mode == e_mode_recv) {
        rf_SendCmdMasked(CONFIG, (u8)(_BV(PRIM_RX) | _BV(PWR_UP)), (u8)( _BV(PRIM_RX) | _BV(PWR_UP)));
    }
    else {
      //rf_SendCmdMasked(CONFIG, (u8)(_BV(PRIM_RX) | _BV(PWR_UP)), (u8)(~_BV(PRIM_RX) | _BV(PWR_UP)));
        rf_SendCmdMasked(CONFIG, (u8)(_BV(PRIM_RX) | _BV(PWR_UP)), (u8)(                _BV(PWR_UP)));
    }
    
    //Requires a 130uS settling time.
 //   TimerDelay_us(135); todo
}
void rf_PowerDown(void)
{
    rf_SendCmdMasked(CONFIG, (u8)_BV(PWR_UP), (u8)~_BV(PWR_UP));
    _comm_mode = e_mode_powered_down;
}
u8 rf_SetPipeAddress(u8 pipe_no, const u8 *addr)
{
    u8 len, i;
    if (pipe_no >= MAX_PIPES)
        return 0;
    
    //Store the pipes address in the _pipes structure
    //Addresses 0-1 are 5 byte, 2-5 are single byte that share address 0
    //    first 4 bytes.
    if (pipe_no  < 2)
    {
        for (i = 0; i < RX_TX_ADDR_LEN; i++) {
            _pipes[pipe_no].addr[i] = *addr++;
        }
        len = RX_TX_ADDR_LEN;
    }
    else
    {
//          _pipes[pipe_no].addr[RX_TX_ADDR_LEN - 1] = *addr;
            _pipes[pipe_no].addr[0]                  = *(addr+RX_TX_ADDR_LEN - 1);
        len = 1;
    }
    return rf_SendCmdBuffer(RX_ADDR_P0 + pipe_no, (u8 *)&_pipes[pipe_no].addr, len);
}
void rf_SetCRC(CRCMode mode)
{
    u8 mask = _BV(EN_CRC) | _BV(CRCO);
    u8 data = 0;
    if (mode == e_crc_disabled)
    {
        data &= ~_BV(EN_CRC);
    }
    else
    {
        if (mode == e_crc_8bit)
        data = _BV(EN_CRC) & ~_BV(CRCO);
        else
        data = _BV(EN_CRC) | _BV(CRCO);
    }
    rf_SendCmdMasked(CONFIG, mask, data);
}
void rf_SetAddressWidth(u8 width)
{
    if      (width==3) {
        rf_SendCmdMasked(SETUP_AW, 0x03, 1);
    }
    else if (width==4) {
        rf_SendCmdMasked(SETUP_AW, 0x03, 2);
    }
    else {//if (width==4) {
        rf_SendCmdMasked(SETUP_AW, 0x03, 3);
    }
}
void rf_SetRetryCount(u8 arc)
{
    rf_SendCmdMasked(SETUP_RETR, 0x0f, arc);
}
void rf_SetRetryDelay(u8 ard)
{
    rf_SendCmdMasked(SETUP_RETR, 0xf0, (ard << 4));
}
void rf_SetDataRate(RFDataRate data_rate)
{
    u8 mask = _BV(RF_DR_LOW) | _BV(RF_DR_HIGH)| _BV(RF_PWR_HIGH)| _BV(RF_PWR_LOW);
  #if   RF_PWR_DBM == 0
    u8 value = 0x00;//[2:1]00: -18 dbm
  #elif RF_PWR_DBM == 1
    u8 value = 0x02;//[2:1]01: -12 dbm
  #elif RF_PWR_DBM == 2
    u8 value = 0x04;//[2:1]10:  -6 dbm
  #elif RF_PWR_DBM == 3
    u8 value = 0x06;//[2:1]11:   0 dbm
  #endif
    switch(data_rate)
    {
        case e_dr_250Kbps:
            value |= _BV(RF_DR_LOW);
            break;
        case e_dr_1Mbps:
            value &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));
            break;
        case e_dr_2Mbps:
            value |= _BV(RF_DR_HIGH);
            break;
    }
    rf_SendCmdMasked(RF_SETUP, mask, value);
}
void rf_ResetStatus(CommunicationMode mode)
{
    if (mode == e_mode_recv || mode == e_mode_both)
        rf_SendCmdMasked(STATUS, _BV(TX_DS) | _BV(RX_DR), _BV(TX_DS) | _BV(RX_DR));  //STATUS.[6:4] are interrupt flags, write 1 to clear bit
                                                                                     //STATUS.[3:0] are READ ONLY
    if (mode == e_mode_xmit || mode == e_mode_both)
        rf_SendCmdMasked(STATUS, _BV(MAX_RT),  _BV(MAX_RT));
}
