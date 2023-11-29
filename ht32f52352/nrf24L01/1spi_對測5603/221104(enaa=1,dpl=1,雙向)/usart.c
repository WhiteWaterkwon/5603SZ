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
#include "tmr50us.h"

/** @addtogroup UART_PutChar_label
 *  @{
 */



/* Private define ------------------------------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------------------------------------*/

u8 UART_SEL = UART_0;

/* UART0 definitions                                                                                        */
vu8  UR0TxBuf[USART_BUFSIZE];
vu8  UR0RxBuf[USART_BUFSIZE];
vu32 UR0TxReadIndex = 0;
vu32 UR0TxWriteIndex = 0;
vu32 UR0RxReadIndex = 0;
vu32 UR0RxWriteIndex = 0;

/* UART1 definitions                                                                                        */
u8   UR1TxBuf[USART_BUFSIZE];
u8   UR1RxBuf[USART_BUFSIZE];
u32  UR1TxReadIndex = 0;
vu32 UR1TxWriteIndex = 0;
vu32 UR1RxReadIndex = 0;
vu32 UR1RxWriteIndex = 0;



/* Global functions ----------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/

void uart_putchar(uint8_t c)
{

		if(UART_SEL ==UART_0){
        while (IS_BUFFER_FULL(UR0TxReadIndex, UR0TxWriteIndex)) {
          USART_IntConfig(HT_USART0, USART_INT_TXDE, ENABLE);
          //waiting...
        }
        UR0TxBuf[UR0TxWriteIndex] = c;
        UR0TxWriteIndex = (UR0TxWriteIndex + 1) % USART_BUFSIZE;
    
        USART_IntConfig(HT_USART0, USART_INT_TXDE, ENABLE);
			}
		if(UART_SEL ==UART_1){
        while (IS_BUFFER_FULL(UR1TxReadIndex, UR1TxWriteIndex)) {
          USART_IntConfig(HT_USART1, USART_INT_TXDE, ENABLE);
          //waiting...
        }
        UR1TxBuf[UR1TxWriteIndex] = c;
        UR1TxWriteIndex = (UR1TxWriteIndex + 1) % USART_BUFSIZE;
    
        USART_IntConfig(HT_USART1, USART_INT_TXDE, ENABLE);
			}				

}


u8 uart_getcmd(void)
{
u8 cmd = 0,i;
static	u32 tmr,OLD_RxWriteIndex=0;
//static	u32 	PRI_RxWriteIndex=0;
	//0 ascii:48
	if(UR1RxWriteIndex != UR1RxReadIndex) { 
	/*		for(i=PRI_RxWriteIndex;i<UR1RxWriteIndex;i++){
			uart_puts("UR[");    uart_putu8(i);  
				uart_puts("]=");    uart_putu8(UR1RxBuf[i]);
				uart_puts("   ");
    PRI_RxWriteIndex = (PRI_RxWriteIndex + 1) % USART_BUFSIZE;
      }		*/
		  if((UR1RxBuf[(UR1RxReadIndex+1)%USART_BUFSIZE]-'0'+(UR1RxBuf[UR1RxReadIndex]-'0')*10)==
				(UR1RxWriteIndex-UR1RxReadIndex+USART_BUFSIZE)%USART_BUFSIZE -2){
					cmd=UR1RxBuf[(UR1RxReadIndex+2)%USART_BUFSIZE]-'0';
		UR1RxReadIndex=	UR1RxWriteIndex;
	return	cmd;
			}
			else if(OLD_RxWriteIndex !=UR1RxWriteIndex){
				OLD_RxWriteIndex =UR1RxWriteIndex;

					tmr = tmr50usGetCurrentTicks();			
			}
			else if(tmr50usIsExpired(tmr, 50)){			
							UR1RxReadIndex=	UR1RxWriteIndex;	
			}
//uart_puts("UR1RxWriteIndex=");    uart_putu8(UR1RxWriteIndex);      uart_putchar_n('\n');	
//uart_puts("UR1RxReadIndex=");    uart_putu8(UR1RxReadIndex);      uart_putchar_n('\n');			

	}	

	return	0xff;	
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
