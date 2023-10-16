/*********************************************************************************************************//**
 * @file    usart.h
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
 * @brief   The header file of the UART functions.
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
#ifndef __USART_INT_H
#define __USART_INT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported types ------------------------------------------------------------------------------------------*/
typedef enum
{
  USART_TX_INT = 1,
  USART_RX_INT = 2,
  USART_ALL_INT = 3
}USART_INT_Mode;

/* Exported constants --------------------------------------------------------------------------------------*/
#define USART_BUFSIZE   1024

extern u8   UR0TxBuf[USART_BUFSIZE];
extern u8   UR0RxBuf[USART_BUFSIZE];
extern u32  UR0TxReadIndex;
extern vu32 UR0TxWriteIndex;
extern u32  UR0RxReadIndex;
extern vu32 UR0RxWriteIndex;

/* Exported macro ------------------------------------------------------------------------------------------*/
#define IS_BUFFER_FULL(ridx, widx)    ((widx + 1) % USART_BUFSIZE == ridx)
#define IS_BUFFER_EMPTY(ridx, widx)   (widx == ridx)

/* Exported functions --------------------------------------------------------------------------------------*/
void USART_Interrupt_Init(HT_USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStructure, USART_INT_Mode Mode);

void uart_putchar(unsigned char);
void uart_putchar_n(unsigned char);
void uart_puts(char *);
void uart_putu8(unsigned char);
void uart_putu16(unsigned int);


#endif /* __USART_INT_H ------------------------------------------------------------------------------------*/
