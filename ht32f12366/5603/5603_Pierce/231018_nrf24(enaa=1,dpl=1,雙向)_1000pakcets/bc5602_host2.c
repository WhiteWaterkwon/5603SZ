/*********************************************************************************************************//**
 * @file    bc5602_host2.c
 * @version $Rev:: 101          $
 * @date    $Date:: 2018-10-26 #$
 * @brief   This file provides all BC5602 HOST functions.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., is the
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
#include <string.h>                          //memcmp
#include "softspi.h"
#include "spi_cmd.h"
#include "tmr50us.h"
#include "usart.h"
#include "bc5602_host.h"
#include "bc5602_host2.h"

/* Private variables ---------------------------------------------------------------------------------------*/
/**
 *  @addtogroup BC3601_Host_FSM2_by_Customer_label
 *  @{
 */
typedef struct
{
    unsigned char   trmode;
    unsigned char   state;
    u32             delay_offset;
    unsigned char   roger_a_rxfifo;
} host5602_fsm2_customer_TypeDef;
static host5602_fsm2_customer_TypeDef hfsm2;
/** @} */

/* Global functions ----------------------------------------------------------------------------------------*/
/**
 *  @ingroup SPI_spiTxQ_label
 */
void movedata_to_spiTxQ___ptx(unsigned char chage_ttt_pipe)
{
    static unsigned char ttt_pipe=0xFF;
    static unsigned char total_len=5;
    static unsigned char ini_deta=0x20;
           unsigned char tttttt[32];
           unsigned char i;
    if(chage_ttt_pipe) {
    ttt_pipe++; if(ttt_pipe>5) ttt_pipe=0;
    }
		ttt_pipe=0;
//  ttt_pipe= rand() % 6;

            #if DPL2_EN_DPL == 0x00
                if     (ttt_pipe==0) {total_len = 1;}
								else if(ttt_pipe==1) {total_len = 8;}
								else if(ttt_pipe==2) {total_len = 16;}
								else if(ttt_pipe==3) {total_len = 24;}
								else if(ttt_pipe==4) {total_len = 28;}
								else if(ttt_pipe==5) {total_len = 32;}
            #else
						total_len = 32;
            //total_len++; if(total_len > 32) {total_len=1;}
            #endif
								
            #if 0
            ini_deta = 0x30 ;
            #else
            ini_deta++; //change initial value for different packet
                    //if(ini_deta==0x00) ini_deta=0x01;
            #endif
        for(i=0; i<total_len; i++) {
            tttttt[i]= (ini_deta+i);
					//tttttt[i]= 0xF0;
        }
        movedata_to_spiTxQ(ttt_pipe, total_len, tttttt+0);
}
void movedata_to_spiTxQ___prx(void)
{
    static unsigned char rrr_pipe=0xFF;
    static unsigned char total_len=10;
    static unsigned char ini_deta=0x20;
           unsigned char rrrrrr[32];
           unsigned char i;
    rrr_pipe++; if(rrr_pipe>5) rrr_pipe=0;
//rrr_pipe=0;
            #if 0
            total_len = 1;//1~32
            #else
            total_len++; if(total_len > 32) {total_len=1;}
            #endif
            #if 0
            ini_deta = 0x30 ;
            #else
            ini_deta++; //change initial value for different packet
                    //if(ini_deta==0x00) ini_deta=0x01;
            #endif
        for(i=0; i<total_len; i++) {
            rrrrrr[i]= (ini_deta+i);
						//rrrrrr[i]= (0xF0);
        }
        movedata_to_spiTxQ(rrr_pipe, total_len, rrrrrr+0);
}

///////////////////////////////////////////////////////////////
static void movedata_spiRxQ_to_print2sscom(void)
{
    unsigned int i;
    uart_puts("RXDLEN=");
    uart_putu8(spiRxQ.w_index);
		
//  uart_putchar_n('\n');
    uart_puts("  RXFIFO[]= ");
    //for(i=0;i<spiRxQ.w_index;i++) {
		for(i=0;i<1;i++) {
        uart_putu8(spiRxQ.payload[i]);
        uart_puts(" ");
    }
    uart_putchar_n('\n');

    hfsm2.roger_a_rxfifo=1;
}
/**
 *  @ingroup BC3601_Host_FSM2_by_Customer_label
 */
void callback_spiRxQ_process_at_hfsm2(void)
{
    movedata_spiRxQ_to_print2sscom(); //call back
    hfsm2.roger_a_rxfifo=1;
}

///////////////////////////////////////////////////////////////
#ifdef _CODE_TRMODE_PTX_
/**
 *  @ingroup BC3601_Host_FSM2_by_Customer_label
 */
static void host5602_fsm2_ptx(void)
{
    switch(hfsm2.state)
    {
    case 0x00://entry
                entry_hfsm1_ptx();
                hfsm2.state=0x01;
            break;
    case 0x01:
            break;
    }
}
#endif

#ifdef _CODE_TRMODE_PRX_
/**
 *  @ingroup BC3601_Host_FSM2_by_Customer_label
 */
static void host5602_fsm2_prx(void)
{
    switch(hfsm2.state)
    {
    case 0x00://entry
                entry_hfsm1_prx();
                hfsm2.state=0x01;
            break;
    case 0x01:
            break;
    }
}
#endif

///////////////////////////////////////////////////////////////
/**
 *  @ingroup BC3601_Host_FSM2_by_Customer_label
 */
void host5602_fsm2(void)
{
    macroCSN_SELECT_FPGA;

hfsm2.trmode = _HOST_TESTMODE_ ;//
    
        #ifdef _CODE_TRMODE_PTX_
        if (hfsm2.trmode == TRMODE_PTX) {
            host5602_fsm2_ptx();

        }
        #endif
        #ifdef _CODE_TRMODE_PRX_
        if (hfsm2.trmode == TRMODE_PRX) {
            host5602_fsm2_prx();
        }
        #endif
}



