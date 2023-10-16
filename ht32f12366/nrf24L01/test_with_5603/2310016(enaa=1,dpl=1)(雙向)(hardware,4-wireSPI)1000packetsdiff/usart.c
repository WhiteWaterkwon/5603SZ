/*********************************************************************************************************//**
 * @file    usart.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
 * @brief   This file provides all UART functions.
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
#include "usart.h"

/** @addtogroup UART_PutChar_label
 *  @{
 */



/* Private define ------------------------------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------------------------------------*/

/* UART0 definitions                                                                                        */
u8   UR0TxBuf[USART_BUFSIZE];
u8   UR0RxBuf[USART_BUFSIZE];
u32  UR0TxReadIndex = 0;
vu32 UR0TxWriteIndex = 0;
u32  UR0RxReadIndex = 0;
vu32 UR0RxWriteIndex = 0;

/* Global functions ----------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
void uart_putchar(unsigned char c)
{
        while (IS_BUFFER_FULL(UR0TxReadIndex, UR0TxWriteIndex)) {
          USART_IntConfig(HTCFG_USART, USART_INT_TXDE, ENABLE);    // @arg USART_INT_TXDE   / USART_IER_THREIE :
          //waiting...
        }
        UR0TxBuf[UR0TxWriteIndex] = c;
        UR0TxWriteIndex = (UR0TxWriteIndex + 1) % USART_BUFSIZE;
    
        USART_IntConfig(HTCFG_USART, USART_INT_TXDE, ENABLE);      // @arg USART_INT_TXDE   / USART_IER_THREIE :
}

void uart_putchar_n(unsigned char c)
{
    if('\n' == c)
    {
        uart_putchar(0x0D);
        uart_putchar(0x0A);
    }
    else
    {
        uart_putchar(c);
    }
}
void uart_puts(char *p_str)
{
    while(*p_str) {
        uart_putchar(*p_str++);
    }
}
static void lownibble_to_ascii(unsigned char c)
{
    c &= 0x0F;
    if( c < 10 )        // 0~9
         uart_putchar('0'+c);
    else                // 10~15
         uart_putchar('A'+c-10);
}
void uart_putu8(unsigned char c)
{
    lownibble_to_ascii(c>>4);    //high nibble
    lownibble_to_ascii(c);       //low  nibble
}
void uart_putu16(unsigned int c)
{
    lownibble_to_ascii(c>>12);   //high nibble
    lownibble_to_ascii(c>>8);    //low  nibble
    lownibble_to_ascii(c>>4);    //high nibble
    lownibble_to_ascii(c);       //low  nibble
}




/**
  * @}
  */
