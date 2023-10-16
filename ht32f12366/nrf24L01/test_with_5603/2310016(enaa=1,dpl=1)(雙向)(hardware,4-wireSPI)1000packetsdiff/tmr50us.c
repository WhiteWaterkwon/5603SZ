/*********************************************************************************************************//**
 * @file    tmr50us.c
 * @version $Rev:: 929          $
 * @date    $Date:: 2016-11-02 #$
 * @brief   This file provides all Timer-50us functions.
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
#include "tmr50us.h"


/** @addtogroup Timer_50us_label
 *  @{
 */


/* Private define ------------------------------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------------------------------------*/

/* BFTM0 definitions                                                                                        */

/* Global functions ----------------------------------------------------------------------------------------*/
vu32 tmr50us_current_ticks;

/*********************************************************************************************************//**
 * @brief   
 * @retval  None
 ************************************************************************************************************/
u32 tmr50usGetCurrentTicks(void)
{
    return tmr50us_current_ticks;
}
bool tmr50usIsExpired(u32 offset_ticks, u32 Texpire)    //Texpire in unit of 50us
{
    if (((u32)(tmr50us_current_ticks+(u32)(0x100000000-offset_ticks)) >= Texpire))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void delay_unit_50us(u32 del)
{
	vu32 offset;
	offset = tmr50usGetCurrentTicks();
	while(tmr50usIsExpired(offset, del) == FALSE) {
		;
	}
}



/**
  * @}
  */
