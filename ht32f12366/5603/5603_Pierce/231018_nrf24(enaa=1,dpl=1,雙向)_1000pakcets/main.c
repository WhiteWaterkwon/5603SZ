/*********************************************************************************************************//**
 * @file    main.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
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
 * <h2><center>Copyright (C) 2016 Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "usart.h"
#include "bc5602_host.h"
#include "bc5602_host2.h"
#include "doxydefgroup.h"

/* Private variables ---------------------------------------------------------------------------------------*/


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  * @details Main program as following
  *  - Enable peripheral clock of AFIO, USART0.
  *  - Config AFIO mode as USART0_Rx and USART0_Tx function.
  *  - USART0 configuration:
  *     - BaudRate = 115200 baud
  *     - Word Length = 8 Bits
  *     - One Stop Bit
  *     - None parity bit
  *  - USART0 Tx "Hello World!" string 10 times.
  *  - USART0 Rx character and transform to hex.(Loop)
  *
  ***********************************************************************************************************/
int main(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  USART_InitTypeDef USART_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* Initialize LED1 & LED2 on HT32 board                                                                   */
//HT32F_DVB_LEDInit(HT_LED1);
//HT32F_DVB_LEDInit(HT_LED2);


  /* Enable peripheral clock of AFIO, USART0, USART1                                                        */
  CKCUClock.Bit.PA         = 1;  // SPI0 PA4~7
//CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.PD         = 1;
  CKCUClock.Bit.PE         = 1;
  CKCUClock.Bit.BKP        = 1;
  CKCUClock.Bit.SPI0       = 1;
//CKCUClock.Bit.SPI1       = 0;
  CKCUClock.Bit.USART0     = 1;
//CKCUClock.Bit.USART1     = 1;
//CKCUClock.Bit.UART0      = 1;
//CKCUClock.Bit.UART1      = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.BFTM0      = 1;
//CKCUClock.Bit.BFTM1      = 1;
	HTCFG_USART_CLK(CKCUClock) = 1;
	HTCFG_SPI_MASTER_SEL_CLOCK(CKCUClock) = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Check if the backup domain is ready for access so as to disable isolation.
     Because some GPIO pins are located in backup domain. */
  if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
  {
    while (1);
  }

  /* toggel LED1 every 0.5 second by match interrupt                                                        */
  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/20000);//50us
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
  NVIC_EnableIRQ(BFTM0_IRQn);

  /* SPI0_SEL idle state is HIGH                                                                            */
   GPIO_PullResistorConfig(HTCFG_SPI_ENABLE_ID, HTCFG_SPI_MASTER_SEL_GPIO_PIN, GPIO_PR_UP);
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
  SPI_InitStructure.SPI_ClockPrescaler = 7;  //
//SPI_InitStructure.SPI_ClockPrescaler = 4;  // fSCK = fPCLK/(2*(CP+1)) = 18.182 MHz   ht32f1656 Master mode SPI speed up to 36MHz
//SPI_InitStructure.SPI_ClockPrescaler = 3;  // fSCK = fPCLK/(2*(CP+1)) = 33.333 MHz   ht32f1656 Master mode SPI speed up to 36MHz
  SPI_Init(HT_SPI0, &SPI_InitStructure);

  /* Set SEL as output mode for slave select                                                                */
  SPI_SELOutputCmd(HT_SPI0, ENABLE);

  /* Enable SPI0                                                                                            */
  SPI_Cmd(HT_SPI0, ENABLE);


  /* Configure AFIO mode of output pins                                                                     */
//AFIO_GPxConfig(GPIO_PD, AFIO_PIN_7, AFIO_MODE_DEFAULT);
//AFIO_GPxConfig(GPIO_PD, AFIO_PIN_8, AFIO_MODE_DEFAULT);
//AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_GPIO);   //configure PA5 as GPIO
//AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6, AFIO_FUN_GPIO);   //configure PA6 as GPIO
//AFIO_GPAConfig(AFIO_PIN_5, AFIO_FUN_GPIO);            //configure PA5 as GPIO
//AFIO_GPAConfig(AFIO_PIN_6, AFIO_FUN_GPIO);            //configure PA6 as GPIO

  /* Configure GPIO direction of output pins                                                                */
//GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_5, GPIO_DIR_OUT);
//GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_6, GPIO_DIR_OUT);

  /*
        USARTx configured as follow:
            - BaudRate = 256000 baud
            - Word Length = 8 Bits
            - Two Stop Bit
            - None parity bit
  */

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */

	AFIO_GPxConfig(HTCFG_USART_TX_GPIO_ID, HTCFG_USART_TX_PIN, AFIO_MODE_6);  // ht32_board_config.h
  AFIO_GPxConfig(HTCFG_USART_RX_GPIO_ID, HTCFG_USART_RX_PIN, AFIO_MODE_6);  // ht32_board_config.h

  /* Config AFIO mode as USARTx function                                                                    */
  USART_InitStructure.USART_BaudRate = 256000;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_2;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

	USART_Init(HTCFG_USART, &USART_InitStructure);
  USART_FIFOReset(HTCFG_USART, USART_FIFO_TX | USART_FIFO_RX);
  USART_IntConfig(HTCFG_USART, USART_INT_TXDE, ENABLE);
  USART_IntConfig(HTCFG_USART, USART_INT_RXDR, ENABLE);
  USART_TxCmd(HTCFG_USART, ENABLE);
  USART_RxCmd(HTCFG_USART, ENABLE);
  NVIC_EnableIRQ(HTCFG_USART_IRQ);
 // USART_Interrupt_Init(HT_USART0, &USART_InitStructure, USART_ALL_INT);
//USART_Interrupt_Init(HT_USART1, &USART_InitStructure, USART_ALL_INT);
//USART_Interrupt_Init(HT_UART1,  &USART_InitStructure, USART_ALL_INT);
	 AFIO_GPxConfig         (GPIO_PD,  AFIO_PIN_0, AFIO_MODE_DEFAULT);
  GPIO_DirectionConfig   (HT_GPIOD, GPIO_PIN_0, GPIO_DIR_OUT);
  
  /*                                                                                                        */
  host5602_process();
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
  */

  while (1)
  {
  }
}
#endif


