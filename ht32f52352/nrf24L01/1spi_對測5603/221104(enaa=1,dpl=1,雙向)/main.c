/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt_FIFO/main.c
 * @version $Rev:: 2351         $
 * @date    $Date:: 2017-12-12 #$
 * @brief   Main program.
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
#include "nrf2401_host.h"
#include "usart.h"
#include "doxydefgroup.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USART_Examples USART
  * @{
  */

/** @addtogroup HyperTerminal_TxRx_Interrupt_FIFO
  * @{
  */


/* Private variables ---------------------------------------------------------------------------------------*/

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  * @details Main program as following
  *  - Enable peripheral clock of AFIO, USART0.
  *  - Config AFIO mode as USARTx_Rx, USARTx_Tx function.
  *  - USARTx configuration:
  *     - BaudRate = 115200 baud
  *     - Word Length = 8 Bits
  *     - One Stop Bit
  *     - None parity bit
  *  - Config FIOF level
  *  - Waiting for inputs from HyperTerminal.
  ***********************************************************************************************************/
int main(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  USART_InitTypeDef USART_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  // Enable peripheral clock of AFIO, UART
  CKCUClock.Bit.PA         = 1;
  CKCUClock.Bit.PB         = 1;  // SPI0 PB2~5
//CKCUClock.Bit.PC         = 1;
//CKCUClock.Bit.PD         = 0;
  CKCUClock.Bit.BKP        = 1;
  CKCUClock.Bit.SPI0       = 1;
//CKCUClock.Bit.SPI1       = 0;
  CKCUClock.Bit.USART0     = 1;
  CKCUClock.Bit.USART1     = 1;
//CKCUClock.Bit.UART0      = 0;
//CKCUClock.Bit.UART1      = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.BFTM0      = 1;
//CKCUClock.Bit.BFTM1      = 0;
//CKCUClock.Bit.EXTI       = 1;   // RTC_Configuration()
  HTCFG_USART_CLK(CKCUClock) = 1;
//HTCFG_SPI_MASTER_SEL_CLOCK(CKCUClock) = 1;  // SPI
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  // Check if the backup domain is ready for access so as to disable isolation.
  // Because some GPIO pins are located in backup domain
  if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
  {
      while (1);
  }

  /* Basic Function Timer 0                                                                                 */
  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/20000);//  50us
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
  NVIC_EnableIRQ(BFTM0_IRQn);

  /* SPI0_SEL idle state is HIGH                                                                            */
  GPIO_PullResistorConfig(HTCFG_SPI_MASTER_SEL_GPIO_ID, HTCFG_SPI_MASTER_SEL_AFIO_PIN, GPIO_PR_UP);
  /* Configure related IO to SPI0 mode                                                                      */
  AFIO_GPxConfig(HTCFG_SPI_MASTER_SEL_AFIO_PORT, HTCFG_SPI_MASTER_SEL_AFIO_PIN,AFIO_MODE_5);
  AFIO_GPxConfig(HTCFG_SPI_MASTER_SCK_AFIO_PORT, HTCFG_SPI_MASTER_SCK_AFIO_PIN,AFIO_MODE_5);
  AFIO_GPxConfig(HTCFG_SPI_MASTER_MOSI_AFIO_PORT, HTCFG_SPI_MASTER_MOSI_AFIO_PIN,AFIO_MODE_5);
  AFIO_GPxConfig(HTCFG_SPI_MASTER_MISO_AFIO_PORT, HTCFG_SPI_MASTER_MISO_AFIO_PIN,AFIO_MODE_5);
  /* SPI0 configuration                                                                                     */
  SPI_InitStructure.SPI_Mode = SPI_MASTER;
  SPI_InitStructure.SPI_FIFO = SPI_FIFO_DISABLE;
  SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
  SPI_InitStructure.SPI_SELMode = SPI_SEL_SOFTWARE;
  SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_FIRST;
  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
  SPI_InitStructure.SPI_RxFIFOTriggerLevel = 0;
  SPI_InitStructure.SPI_TxFIFOTriggerLevel = 0;
  SPI_InitStructure.SPI_ClockPrescaler = 10;
  SPI_Init(HT_SPI0, &SPI_InitStructure);
  /* Set SEL as output mode for slave select                                                                */
  SPI_SELOutputCmd(HT_SPI0, ENABLE);
  /* Enable SPI0                                                                                            */
  SPI_Cmd(HT_SPI0, ENABLE);

  /* Enable peripheral clock of AFIO, USART0                                                                */

 AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8, AFIO_MODE_6);  //USR0_TX 
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10, AFIO_MODE_6);  // USR0_RX
	
  /* Config AFIO mode as HTCFG_USART_Tx function.                                                           */
  AFIO_GPxConfig(HTCFG_USART_TX_GPIO_ID, HTCFG_USART_TX_AFIO, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(HTCFG_USART_RX_GPIO_ID, HTCFG_USART_RX_AFIO, AFIO_FUN_USART_UART);
  /*
        USARTx configured as follow:
            - BaudRate = 115200 baud
            - Word Length = 8 Bits
            - One Stop Bit
            - None parity bit
  */
  USART_InitStructure.USART_BaudRate = 256000;//115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

//UART1
  USART_Init(HTCFG_USART, &USART_InitStructure);
  USART_FIFOReset(HTCFG_USART, USART_FIFO_TX | USART_FIFO_RX);
  USART_IntConfig(HTCFG_USART, USART_INT_TXDE, ENABLE); // [1]TXDEIE Transmit Data Empty Interrupt Enable
  USART_IntConfig(HTCFG_USART, USART_INT_RXDR, ENABLE); // [0]RXDRIE Receive Data Ready Interrupt Enable
  USART_IntConfig(HTCFG_USART, USART_INT_TOUT, ENABLE); // [8]RXTOIE Receive FIFO Time-Out Interrupt Enable
//  USART_TXTLConfig(HTCFG_USART, USART_TXTL_00);
//USART_RXTLConfig(HTCFG_USART, USART_RXTL_01);
//USART_RXTLConfig(HTCFG_USART, USART_RXTL_02);
//  USART_RXTLConfig(HTCFG_USART, USART_RXTL_04);
//USART_RXTLConfig(HTCFG_USART, USART_RXTL_06);
//  USART_SetTimeOutValue(HTCFG_USART, 0x7F);
  USART_TxCmd(HTCFG_USART, ENABLE);
  USART_RxCmd(HTCFG_USART, ENABLE);
  NVIC_EnableIRQ(HTCFG_USART_IRQn);
	
//UART0
	USART_Init(HT_USART0, &USART_InitStructure);
  USART_FIFOReset(HT_USART0, USART_FIFO_TX | USART_FIFO_RX);
  USART_IntConfig(HT_USART0, USART_INT_TXDE, ENABLE);
  USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);
  USART_TxCmd(HT_USART0, ENABLE);
  USART_RxCmd(HT_USART0, ENABLE);
  NVIC_EnableIRQ(USART0_IRQn);	

  /* init                                                                                                   */
  //GPIO --- IRQ
           AFIO_GPxConfig( GPIO_PA, AFIO_PIN_2, AFIO_MODE_1);
     GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_2, GPIO_DIR_IN);
  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_2, GPIO_PR_UP);
         GPIO_InputConfig(HT_GPIOA, GPIO_PIN_2, ENABLE);


  /* main loop                                                                                              */
  host24L01_process();
  while (1)
  {
  }
}



#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
