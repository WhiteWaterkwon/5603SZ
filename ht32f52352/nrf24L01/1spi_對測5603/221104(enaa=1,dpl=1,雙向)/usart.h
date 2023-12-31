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
/* Exported constants --------------------------------------------------------------------------------------*/
#define USART_BUFSIZE   256

extern vu8  UR0TxBuf[USART_BUFSIZE];
extern vu8  UR0RxBuf[USART_BUFSIZE];
extern vu32 UR0TxReadIndex;
extern vu32 UR0TxWriteIndex;
extern vu32 UR0RxReadIndex;
extern vu32 UR0RxWriteIndex;


extern u8   UR1TxBuf[USART_BUFSIZE];
extern u8   UR1RxBuf[USART_BUFSIZE];
extern u32  UR1TxReadIndex ;
extern vu32 UR1TxWriteIndex ;
extern vu32 UR1RxReadIndex ;
extern vu32 UR1RxWriteIndex ;

enum
{
  UART_0 = 1,
  UART_1 = 2,
};
extern u8 UART_SEL ;
/* Exported macro ------------------------------------------------------------------------------------------*/
#define IS_BUFFER_FULL(ridx, widx)    ((widx + 1) % USART_BUFSIZE == ridx)
#define IS_BUFFER_EMPTY(ridx, widx)   (widx == ridx)

/* Exported functions --------------------------------------------------------------------------------------*/
void USART_Interrupt_Init(HT_USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStructure, USART_INT_Mode Mode);

void uart_putchar(uint8_t);
u8 uart_getcmd(void);
void uart_putchar_n(uint8_t);
void uart_puts(char *);
void uart_putu8(uint8_t);
void uart_putu16(uint16_t);


#endif /* __USART_INT_H ------------------------------------------------------------------------------------*/
