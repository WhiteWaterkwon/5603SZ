/*********************************************************************************************************//**
 * @file    softspi.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
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
#include "softspi.h"
#include "usart.h"      //debug
#include "bc5602_host.h"    // _SPI_WIRES_


/**
 *  @ingroup Software_SPI_label
 */
volatile unsigned char CSNSelect;    // bit[2]:FPGA, bit[1]:ADC, bit[0]:RF


/* Private define ------------------------------------------------------------------------------------------*/
/* SPI SPI DUALEN Mask                                                                                      */
#define CR1_DUALEN_SET      (u32)0x00000040    // ht32f1656
#define CR1_DUALEN_RESET    (u32)0xFFFFFFBF    // ht32f1656

/* Private variables ---------------------------------------------------------------------------------------*/
/*
#define SPI_CSN_0_SET_1     GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_0, SET)
//efine SPI_CSN_1_SET_1     GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_1, SET)
#define SPI_CSN_2_SET_1     GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_2, SET)
#define SPI_SCK_SET_1       GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_3, SET)
#define SPI_SDIO_SET_1      GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_4, SET)
#define SPI_MSEN_SET_1      GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_6, SET)
#define SPI_CSN_0_RESET_0   GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_0, RESET)
//efine SPI_CSN_1_RESET_0   GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_1, RESET)
#define SPI_CSN_2_RESET_0   GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_2, RESET)
#define SPI_SCK_RESET_0     GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_3, RESET)
#define SPI_SDIO_RESET_0    GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_4, RESET)
#define SPI_MSEN_RESET_0    GPIO_WriteOutBits(HT_GPIOD, GPIO_PIN_6, RESET)
#define SPI_SDIO_IS_SET    (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_4)==SET)
*/
#define SPI_CSN_0_SET_1     HT_GPIOD->SRR = GPIO_PIN_0
#define SPI_CSN_1_SET_1     HT_GPIOD->SRR = GPIO_PIN_1
#define SPI_CSN_2_SET_1     HT_GPIOD->SRR = GPIO_PIN_2
#define SPI_SCK_SET_1       HT_GPIOD->SRR = GPIO_PIN_3
#define SPI_SDIO_SET_1      HT_GPIOD->SRR = GPIO_PIN_4
#define SPI_MSEN_SET_1      HT_GPIOD->SRR = GPIO_PIN_6
#define SPI_CSN_0_RESET_0   HT_GPIOD->RR = GPIO_PIN_0
#define SPI_CSN_1_RESET_0   HT_GPIOD->RR = GPIO_PIN_1
#define SPI_CSN_2_RESET_0   HT_GPIOD->RR = GPIO_PIN_2
#define SPI_SCK_RESET_0     HT_GPIOD->RR = GPIO_PIN_3
#define SPI_SDIO_RESET_0    HT_GPIOD->RR = GPIO_PIN_4
#define SPI_MSEN_RESET_0    HT_GPIOD->RR = GPIO_PIN_6

#if   _SPI_WIRES_ == 3
  #define SPI_SDIO_IS_SET    (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_4)==SET)
#elif _SPI_WIRES_ == 4
 // #define SPI_SDIO_IS_SET    (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1)==SET)
//#define SPI_MISO_IS_SET    (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1)==SET)
#endif
//


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/

#if _SPI_WIRES_ == 4
void SPI_MISO_INPUT_MDOE(void)
{
 // AFIO_GPxConfig(GPIO_PD, AFIO_PIN_1, AFIO_MODE_1);
 // GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_IN);
 // GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_1, GPIO_PR_UP);
 // GPIO_InputConfig(HT_GPIOD, GPIO_PIN_1, ENABLE);
}
#endif

void SPI_SDIO_INPUT_MDOE(void)
{
  #if _SPI_WIRES_ == 3
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_4, AFIO_MODE_1);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_IN);
  GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_4, GPIO_PR_UP);
  GPIO_InputConfig(HT_GPIOD, GPIO_PIN_4, ENABLE);
  #endif
  SPI_MSEN_SET_1;
}
void SPI_SDIO_OUTPUT_MDOE(void)
{
  #if _SPI_WIRES_ == 3
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_4, AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_OUT);
  #endif
  SPI_MSEN_RESET_0;
}
static void pd012346_as_outputmode(void)
{
    SPI_CSN_0_SET_1;
        AFIO_GPxConfig(GPIO_PD, AFIO_PIN_0, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_0, GPIO_DIR_OUT);
    SPI_CSN_1_SET_1;
        AFIO_GPxConfig(GPIO_PD, AFIO_PIN_1, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_OUT);
    SPI_CSN_2_SET_1;
        AFIO_GPxConfig(GPIO_PD, AFIO_PIN_2, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_2, GPIO_DIR_OUT);
    SPI_SCK_RESET_0;
        AFIO_GPxConfig(GPIO_PD, AFIO_PIN_3, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_3, GPIO_DIR_OUT);
    SPI_SDIO_RESET_0;
        AFIO_GPxConfig(GPIO_PD, AFIO_PIN_4, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_4, GPIO_DIR_OUT);
    SPI_MSEN_RESET_0;
        AFIO_GPxConfig(GPIO_PD, AFIO_PIN_6, AFIO_MODE_DEFAULT);
        GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_6, GPIO_DIR_OUT);
}
/** 
 *  @ingroup Software_SPI_label
 */
void init_softspi(void)
{
    pd012346_as_outputmode();
    #if _SPI_WIRES_ == 4
    SPI_MISO_INPUT_MDOE();
    #endif
}

static unsigned char spi_octet(unsigned char octet)
{
    #if _SPI_MODE_ == 1 // 0:software spi,   1:hardware spi
        HT_SPI0->DR = octet;  //SPI_SendData(HT_SPI0, octet);
        while( ((HT_SPI0->SR & SPI_FLAG_TXE)==(u32)RESET)?(SET):(RESET) );  //while (!SPI_GetFlagStatus(HT_SPI0, SPI_FLAG_TXE));
        return HT_SPI0->DR ;
    #else
        if(octet & 0x80) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(octet & 0x40) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(octet & 0x20) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(octet & 0x10) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(octet & 0x08) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(octet & 0x04) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(octet & 0x02) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(octet & 0x01) {SPI_SDIO_SET_1;}
        else             {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;

        return 0x00 ;
    #endif
}

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
/** 
 *  @ingroup Software_SPI_label
 */
void spi_w(unsigned char addr, unsigned char deta)
{
        SPI_SCK_RESET_0;
        if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
        if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
        if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        ///////////////
        spi_octet((addr&0x3F)|0x40); // [7:6]=01
        spi_octet(deta);
        ///////////////
        ///////////////
        SPI_CSN_0_SET_1;
        SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
//uart_puts("spi_w("); uart_putu8(addr);  uart_putu8(deta); uart_putchar_n('\n');
}

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
/** 
 *  @ingroup Software_SPI_label
 */
unsigned char spi_r(unsigned char addr)
{
    unsigned char deta;
        SPI_SCK_RESET_0;
        if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
        if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
        if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        //
        spi_octet(addr|0xC0); // [7:6]=11
        //
    SPI_SDIO_INPUT_MDOE();            SPI_MSEN_SET_1;
    #if _SPI_MODE_ == 1 // 0:software spi,   1:hardware spi
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 |= CR1_DUALEN_SET ;
        #endif
        deta = spi_octet(0x00);
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 &= CR1_DUALEN_RESET ;
        #endif
    #else
        deta=0x00;
        if(SPI_SDIO_IS_SET) {deta |= 0x80;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x40;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x20;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x10;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x08;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x04;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x02;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x01;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        //
    #endif
    SPI_SDIO_OUTPUT_MDOE();        SPI_MSEN_RESET_0;
        SPI_CSN_0_SET_1;
        SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
//uart_puts("spi_r("); uart_putu8(addr); uart_puts(")="); uart_putu8(deta); uart_putchar_n('\n');
    return deta;
}

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
/** 
 *  @ingroup Software_SPI_label
 */
void spi_cmd0(unsigned char addr)
{
        SPI_SCK_RESET_0;
        if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
        if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
        if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        //
    #if _SPI_MODE_ == 1 // 0:software spi,   1:hardware spi
        spi_octet(addr);
    #else
        if(addr & 0x80) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(addr & 0x40) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(addr & 0x20) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(addr & 0x10) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(addr & 0x08) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(addr & 0x04) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(addr & 0x02) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(addr & 0x01) {SPI_SDIO_SET_1;}
        else            {SPI_SDIO_RESET_0;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        //
    #endif
        SPI_CSN_0_SET_1;
        SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
//uart_puts("spi_cmd0("); uart_putu8(addr); uart_putchar_n('\n');
}


/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
/** 
 *  @ingroup Software_SPI_label
 */
static void spi_write_ID(unsigned char cmd0, address5602_TypeDef addr)
{
        SPI_SCK_RESET_0;
      //if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
      //if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
      //if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
                            SPI_CSN_2_RESET_0;
        ///////////////
        spi_octet(cmd0);               //0x10:address, strobe_command_CmdD: Write ID
        ///////////////
        spi_octet(addr.byte0);         // address Byte 0, LSByte is written first
        spi_octet(addr.byte4321);      // address Byte 1
        spi_octet(addr.byte4321>>8);   // address Byte 2
        spi_octet(addr.byte4321>>16);  // address Byte 3
        spi_octet(addr.byte4321>>24);  // address Byte 4
        ///////////////
        SPI_CSN_0_SET_1;
      //SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
}
void spi_cmdd_10_write_pipe_0_ID(address5602_TypeDef addr)
{
    spi_write_ID(0x10, addr);//0x10:address, strobe_command_CmdD: Write ID
}
void spi_cmdd_12_write_pipe_1_ID(address5602_TypeDef addr)
{
    spi_write_ID(0x12, addr);//0x12:address, strobe_command_CmdD: Write ID
}

/** 
 *  @ingroup Software_SPI_label
 */
#if _SPI_MODE_ == 0 // 0:software spi,   1:hardware spi
static uint8_t spi_read_ID_foo(void)
{
    unsigned char i;
    unsigned char mask;
    unsigned char deta;
    for(i=0,mask=0x80,deta=0x00; i<8; i++)
    {
        if(SPI_SDIO_IS_SET) {deta |= mask;}
      //else                {deta this bit is 0}
        SPI_SCK_SET_1;    //rising edge
        SPI_SCK_RESET_0;
        mask>>=1;
    }
    return deta;
}
#endif
static void spi_read_ID(unsigned char cmd0, address5602_TypeDef *pAddr)
{
    unsigned char tmp[4];
        SPI_SCK_RESET_0;
      //if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
      //if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
      //if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
                            SPI_CSN_2_RESET_0;
        //
        spi_octet(cmd0); //0x90:address, strobe_command_CmdD: Read ID
        //
    SPI_SDIO_INPUT_MDOE();            SPI_MSEN_SET_1;
    #if _SPI_MODE_ == 1 // 0:software spi,   1:hardware spi
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 |= CR1_DUALEN_SET ;
        #endif
        pAddr->byte0 = spi_octet(0x00);  // address Byte 0, LSByte is written first
              tmp[0] = spi_octet(0x00);  // address Byte 1
              tmp[1] = spi_octet(0x00);  // address Byte 2
              tmp[2] = spi_octet(0x00);  // address Byte 3
              tmp[3] = spi_octet(0x00);  // address Byte 4
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 &= CR1_DUALEN_RESET ;
        #endif
    #else
        pAddr->byte0 = spi_read_ID_foo();  // address Byte 0, LSByte is written first
              tmp[0] = spi_read_ID_foo();  // address Byte 1
              tmp[1] = spi_read_ID_foo();  // address Byte 2
              tmp[2] = spi_read_ID_foo();  // address Byte 3
              tmp[3] = spi_read_ID_foo();  // address Byte 4
    #endif
        pAddr->byte4321 = tmp[3];
        pAddr->byte4321 = tmp[2]+(pAddr->byte4321<<8);
        pAddr->byte4321 = tmp[1]+(pAddr->byte4321<<8);
        pAddr->byte4321 = tmp[0]+(pAddr->byte4321<<8);
    SPI_SDIO_OUTPUT_MDOE();        SPI_MSEN_RESET_0;
        //
        SPI_CSN_0_SET_1;
      //SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
}
void spi_cmdd_90_read_pipe_0_ID(address5602_TypeDef *pAddr)
{
    spi_read_ID(0x90, pAddr);//0x90:address, strobe_command_CmdD: Read ID
}
void spi_cmdd_92_read_pipe_1_ID(address5602_TypeDef *pAddr)
{
    spi_read_ID(0x92, pAddr);//0x92:address, strobe_command_CmdD: Read ID
}

void spi_cmdd_9F_read_chip_version(unsigned char *pVer)
{
        SPI_SCK_RESET_0;
      //if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
      //if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
      //if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        SPI_CSN_2_RESET_0;
        //
        spi_octet(0x9F); //0x9F:address, strobe_command_CmdD: Read chip version
        //
    SPI_SDIO_INPUT_MDOE();            SPI_MSEN_SET_1;
    #if _SPI_MODE_ == 1 // 0:software spi,   1:hardware spi
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 |= CR1_DUALEN_SET ;
        #endif
              pVer[0] = spi_octet(0x00);  // address Byte 0, LSByte is written first
              pVer[1] = spi_octet(0x00);  // address Byte 0, LSByte is written first
              pVer[2] = spi_octet(0x00);  // address Byte 0, LSByte is written first
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 &= CR1_DUALEN_RESET ;
        #endif
    #else
              pVer[0] = spi_read_ID_foo();  // Byte 0
              pVer[1] = spi_read_ID_foo();  // Byte 1
              pVer[2] = spi_read_ID_foo();  // Byte 2
    #endif
    SPI_SDIO_OUTPUT_MDOE();        SPI_MSEN_RESET_0;
        //
        SPI_CSN_0_SET_1;
      //SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
}

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
/** 
 *  @ingroup Software_SPI_label
 */
void spi_cmdd_11_txfifo_write(unsigned int Length, unsigned char *pDeta)
{
    unsigned char deta;
    unsigned int  j;
    if(Length==0x00) {return;}
        SPI_SCK_RESET_0;
        if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
        if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
        if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        //
        spi_octet(0x11); //0x11:address, strobe command CmdD: TX FIFO read command
        //
  //if(Length>32) {Length=32;}
    for(j=0;j<Length;j++)
    {
        deta = pDeta[j];
        spi_octet(deta);
    }
        ///////////////
        SPI_CSN_0_SET_1;
        SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
}
void spi_cmdd_13_txfifo_write_no_ack(unsigned int Length, unsigned char *pDeta)
{
    unsigned char deta;
    unsigned int  j;
    if(Length==0x00) {return;}
        SPI_SCK_RESET_0;
        if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
        if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
        if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        //
        spi_octet(0x13); //0x13:address, strobe command CmdD: TX FIFO read command
        //
  //if(Length>32) {Length=32;}
    for(j=0;j<Length;j++)
    {
        deta = pDeta[j];
        spi_octet(deta);
    }
        ///////////////
        SPI_CSN_0_SET_1;
        SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
}
void spi_cmdd_18_txfifo_prx_w_ack_payload(unsigned char pipe, unsigned char Length, unsigned char *pDeta)
{
    unsigned char deta;
    unsigned char j;
    if(Length==0x00) {return;}
        SPI_SCK_RESET_0;
        if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
        if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
        if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        //
        spi_octet(0x18+pipe); //0x18:address, strobe command CmdD: TX FIFO read command
        //
    if(Length>32) {Length=32;}
    for(j=0;j<Length;j++)
    {
        deta = pDeta[j];
        spi_octet(deta);
    }
        ///////////////
        SPI_CSN_0_SET_1;
        SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
}

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
/** 
 *  @ingroup Software_SPI_label
 */
void spi_cmdd_BF_rxfifo_read(unsigned char read_length, unsigned char *pch)
{
    unsigned int  i;
        ///////////////
        SPI_SCK_RESET_0;
        if(CSNSelect&0x01) {SPI_CSN_0_RESET_0;}
        if(CSNSelect&0x02) {SPI_CSN_1_RESET_0;}
        if(CSNSelect&0x04) {SPI_CSN_2_RESET_0;}
        ///////////////
        spi_octet(0xBF); // 0xBF:address, strobe_command_CmdD: RX FIFO Read
        ///////////////
    SPI_SDIO_INPUT_MDOE();            SPI_MSEN_SET_1;
    #if _SPI_MODE_ == 1 // 0:software spi,   1:hardware spi
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 |= CR1_DUALEN_SET ;
        #endif
        for(i=0;i<read_length;i++) {
            *pch = spi_octet(0x00);//<<<<<<<<<<<
            pch++;
        }
        #if   _SPI_WIRES_ == 3
        HT_SPI0->CR0 &= CR1_DUALEN_RESET ;
        #endif
    #else
    for(i=0;i<read_length;i++) {
        unsigned char deta;
        deta=0x00;
        if(SPI_SDIO_IS_SET) {deta |= 0x80;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x40;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x20;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x10;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x08;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x04;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x02;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        if(SPI_SDIO_IS_SET) {deta |= 0x01;}
        SPI_SCK_SET_1;
        SPI_SCK_RESET_0;
        //
        *pch = deta;//<<<<<<<<<<<
        pch++;
    }
    #endif
    SPI_SDIO_OUTPUT_MDOE();        SPI_MSEN_RESET_0;
        ///////////////
        SPI_CSN_0_SET_1;
        SPI_CSN_1_SET_1;
        SPI_CSN_2_SET_1;
}

