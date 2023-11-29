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

/* USART1 definitions                                                                                        */
vu8  USR1TxBuf[USART_BUFSIZE];
vu8  USR1RxBuf[USART_BUFSIZE];
vu32 USR1TxReadIndex = 0;
vu32 USR1TxWriteIndex = 0;
vu32 USR1RxReadIndex = 0;
vu32 USR1RxWriteIndex = 0;

/* Global functions ----------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
uint8_t uart_rx_getchar(void)
{
    uint8_t c;
    while (IS_BUFFER_EMPTY(USR1RxReadIndex, USR1RxWriteIndex) ) {;}
    c = USR1RxBuf[USR1RxReadIndex];
    USR1RxReadIndex = (USR1RxReadIndex + 1) % USART_BUFSIZE;
    return c;
}
void uart_putchar(uint8_t c)
{
	
      /*  while (IS_BUFFER_FULL(USR1TxReadIndex, USR1TxWriteIndex)) {
          USART_IntConfig(HTCFG_USART, USART_INT_TXDE, ENABLE);    // @arg USART_INT_TXDE   / USART_IER_THREIE :
          //waiting...
        }
        USR1TxBuf[USR1TxWriteIndex] = c;
      //USR1TxWriteIndex = (USR1TxWriteIndex + 1) % USART_BUFSIZE;
        USR1TxWriteIndex = (USR1TxWriteIndex + 1) &(USART_BUFSIZE-1);
    
        USART_IntConfig(HTCFG_USART, USART_INT_TXDE, ENABLE);  */    // @arg USART_INT_TXDE   / USART_IER_THREIE :
}

void uart_putchar_n(uint8_t c)
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
static void lownibble_to_ascii(uint8_t c)
{
    c &= 0x0F;
    if( c < 10 )        // 0~9
         uart_putchar('0'+c);
    else                // 10~15
         uart_putchar('A'+c-10);
}
void uart_putu8(uint8_t c)
{
    lownibble_to_ascii(c>>4);    //high nibble
    lownibble_to_ascii(c);       //low  nibble
}
void uart_putu16(uint16_t c)
{
    lownibble_to_ascii(c>>12);   //high nibble
    lownibble_to_ascii(c>>8);    //low  nibble
    lownibble_to_ascii(c>>4);    //high nibble
    lownibble_to_ascii(c);       //low  nibble
}




/**
  * @}
  */
