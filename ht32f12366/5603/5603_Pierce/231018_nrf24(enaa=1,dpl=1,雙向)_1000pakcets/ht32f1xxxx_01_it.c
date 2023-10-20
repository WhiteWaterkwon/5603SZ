/*********************************************************************************************************//**
 * @file    USART/Polling/ht32f1xxxx_01_it.c
 * @version $Rev:: 1372         $
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

/** @addtogroup Polling
  * @{
  */


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
 * @brief   This function handles Memory Manage exception.
 * @retval  None
 ************************************************************************************************************/
void MemManage_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Bus Fault exception.
 * @retval  None
 ************************************************************************************************************/
void BusFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Usage Fault exception.
 * @retval  None
 ************************************************************************************************************/
void UsageFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Debug Monitor exception.
 * @retval  None
 ************************************************************************************************************/
void DebugMon_Handler(void)
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
 * @brief   This function handles BFTM0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{
  tmr50us_current_ticks++;
  BFTM_ClearFlag(HT_BFTM0);
}

/*********************************************************************************************************//**
 * @brief   This function handles USART0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void USART0_IRQHandler(void)
{
  uint32_t i;

  /* Tx, move data from buffer to USART FIFO                                                                */
  if (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXDE))    // offset 0x010: USART Status and Interrupt Flag register
                                   //[7]transmit_data_FIFO Empty
  {
    if (IS_BUFFER_EMPTY(UR0TxReadIndex, UR0TxWriteIndex))
    {
      USART_IntConfig(HT_USART0, USART_INT_TXDE, DISABLE);    // @arg USART_INT_TXDE   / USART_IER_THREIE :
    }
    else
    {
      for (i = 0; i < 8; i++)  //            ht32f12366 8 bytes transmit_data_FIFO
                               //                               transmit_data_FIFO is empty
                               // then can write max 8 bytes to transmit_data_FIFO
      {
        USART_SendData(HT_USART0, UR0TxBuf[UR0TxReadIndex]);
        UR0TxReadIndex = (UR0TxReadIndex + 1) % USART_BUFSIZE;
        if (IS_BUFFER_EMPTY(UR0TxReadIndex, UR0TxWriteIndex))
        {
          break;
        }
      }
    }
  }

  /* Rx, move data from USART FIFO to buffer                                                                */
  if (USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR))
  {
    if (IS_BUFFER_FULL(UR0RxReadIndex, UR0RxWriteIndex))
    {
      /* USART Rx buffer is full                                                                            */
      return;
    }

    UR0RxBuf[UR0RxWriteIndex] = USART_ReceiveData(HT_USART0);
    UR0RxWriteIndex = (UR0RxWriteIndex + 1) % USART_BUFSIZE;
  }
}

/*********************************************************************************************************//**
 * @brief   This function handles USART1 interrupt.
 * @retval  None
 ************************************************************************************************************/
void USART1_IRQHandler(void)
{
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
