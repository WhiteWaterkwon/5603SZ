/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt_FIFO/ht32f5xxxx_01_it.c
 * @version $Rev:: 2970         $
 * @date    $Date:: 2018-08-03 #$
 * @brief   This file provides all interrupt service routine.
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
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "usart.h"
#include "tmr50us.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USART_Examples USART
  * @{
  */

/** @addtogroup HyperTerminal_TxRx_Interrupt_FIFO
  * @{
  */


/* Private define ------------------------------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------------------------------------*/

/* USART0 definitions                                                                                       */

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  #if 1

  static vu32 gIsContinue = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
  /* 1. Stack Checking                                                                                      */
  /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
  /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
  /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
  /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
  /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
  while (gIsContinue == 0)
  {
  }
  /* 2. Step Out to Find the Clue                                                                           */
  /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
  /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
  /*    the hard fault.                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/

  #else

  /*--------------------------------------------------------------------------------------------------------*/
  /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_SystemReset();

  #endif
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{
  tmr50us_current_ticks++;
  BFTM_ClearFlag(HT_BFTM0);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
}
/*********************************************************************************************************//**
 * @brief   This function handles UART0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void HTCFG_USART_IRQHandler(void)
{
    u32 i;
  //Tx FIFO underflow,move data from buffer to FIFO
    if(USART_GetFlagStatus(HTCFG_USART,USART_FLAG_TXDE))
    {
    if (IS_BUFFER_EMPTY(USR1TxReadIndex, USR1TxWriteIndex))
    {
      USART_IntConfig(HTCFG_USART, USART_INT_TXDE, DISABLE);
    }
    else
    {
      for (i = 0; i < 8; i++) // 52352 USART FIFO 8 bytes
    //for (i = 0; i < 1; i++) // 52352  UART FIFO 1 byte
      {
        USART_SendData(HTCFG_USART, USR1TxBuf[USR1TxReadIndex]);
        USR1TxReadIndex = (USR1TxReadIndex + 1) % USART_BUFSIZE;
        if (IS_BUFFER_EMPTY(USR1TxReadIndex, USR1TxWriteIndex))
        {
            break;
        }
      }
    }
    }

  //Rx FIFO int, move data from FIFO to buffer                                                                
    if (USART_GetFlagStatus(HTCFG_USART,USART_FLAG_RXDR))        // Receive FIFO Ready Flag
    {
        USART_ClearFlag(HTCFG_USART, USART_FLAG_RXDR);
    while(USART_GetFlagStatus(HTCFG_USART,USART_FLAG_RXDNE))     // RX FIFO Data Not Empty
    {
        if (IS_BUFFER_FULL(USR1RxReadIndex, USR1RxWriteIndex)) {
		    break;
        }
        else {
            USR1RxBuf[USR1RxWriteIndex] = USART_ReceiveData(HTCFG_USART);
            USR1RxWriteIndex = (USR1RxWriteIndex + 1) % USART_BUFSIZE;
        }
    };
    };
    if (USART_GetFlagStatus(HTCFG_USART,USART_FLAG_TOUT))        // Receive FIFO Time-Out Flag
    {
        USART_ClearFlag(HTCFG_USART, USART_FLAG_TOUT);
    while(USART_GetFlagStatus(HTCFG_USART,USART_FLAG_RXDNE))     // RX FIFO Data Not Empty
    {
        if (IS_BUFFER_FULL(USR1RxReadIndex, USR1RxWriteIndex)) {
		    break;
        }
        else {
            USR1RxBuf[USR1RxWriteIndex] = USART_ReceiveData(HTCFG_USART);
            USR1RxWriteIndex = (USR1RxWriteIndex + 1) % USART_BUFSIZE;
        }
    };
    };

}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
