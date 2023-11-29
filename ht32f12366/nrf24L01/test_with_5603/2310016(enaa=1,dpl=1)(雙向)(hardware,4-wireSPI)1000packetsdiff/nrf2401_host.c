/*********************************************************************************************************//**
 * @file    nrf2401_host.c
 * @version $Rev:: 101          $
 * @date    $Date:: 2017-10-26 #$
 * @brief   This file provides all nRF24L01 HOST functions.
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
#include "ht32_board_config.h"
#include <string.h>                          //memcmp
#include "nrf2401_host.h"
#include "softspi.h"
#include "tmr50us.h"
#include "usart.h"


/* Private variables ---------------------------------------------------------------------------------------*/
int debug_i=0;
int debug_MAX_RT=0;
int debug_TX_DS=0;
/* Global functions ----------------------------------------------------------------------------------------*/



/////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @addtogroup nRF2401_Host_FSM1_public_by_Holtek_label
 *  @{
 */
typedef struct {
    unsigned char   trmode;
    u32             delay_offset;
    RFDataRate      rfdatarate;                     //e_dr_250Kbps, e_dr_1Mbps, e_dr_2Mbps
} host24L01_fsm1_holtek_TypeDef;
static volatile host24L01_fsm1_holtek_TypeDef hfsm1;
/** @} */


/********************************************************************************/
#define IRQ_IS_SET          (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)==SET)
#define IRQ_IS_RESET        (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)==RESET)
/*
#define IRQ_IS_SET          (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)==SET)
#define IRQ_IS_RESET        (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)==RESET)
*/
/**
 *  @ingroup IRQ_to_GPIO_label
 */
static void init_GPIO_IRQ(void)
{
    //GPIO <--- IRQ
    AFIO_GPxConfig(GPIO_PD, AFIO_PIN_5, AFIO_MODE_1);
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_5, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOD, GPIO_PIN_5, ENABLE);
}
/**
 *  @ingroup IRQ_to_GPIO_label
 */

static bool GPIO_IRQ_IS_SET(void)
{
    bool result;
    if(IRQ_IS_SET) {result = TRUE;}
    else           {result = FALSE;}
    return result;
}
static bool GPIO_IRQ_IS_RESET(void)
{
    bool result;
    if(IRQ_IS_RESET) {result = TRUE;}
    else             {result = FALSE;}
    return result;
}

//============================================================
#define write_PE0_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_0, SET)
#define write_PE1_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_1, SET)
#define write_PE2_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_2, SET)
#define write_PE3_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_3, SET)
#define write_PE4_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_4, SET)
#define write_PE5_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_5, SET)
#define write_PE6_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_6, SET)
#define write_PE7_SET_1       GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_7, SET)

#define write_PE0_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_0, RESET)
#define write_PE1_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_1, RESET)
#define write_PE2_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_2, RESET)
#define write_PE3_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_3, RESET)
#define write_PE4_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_4, RESET)
#define write_PE5_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_5, RESET)
#define write_PE6_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_6, RESET)
#define write_PE7_RESET_0     GPIO_WriteOutBits(HT_GPIOE, GPIO_PIN_7, RESET)

/**
 *  @ingroup GPIO_debug_label
 */
void GPIO_ToggleOutBits(HT_GPIO_TypeDef* GPIOx,  u16 GPIO_PIN)
{
  FlagStatus Status;
//GPIO_CK_ST;
//Assert_Param(IS_GPIO(GPIOx));
//GPIO_CK_ON();
  Status = GPIO_ReadOutBit(GPIOx, GPIO_PIN);
  if (Status == RESET)
    GPIOx->SRR = GPIO_PIN;
  else
    GPIOx->RR = GPIO_PIN;
//GPIO_CK_OFF();
}
/**
 *  @ingroup GPIO_debug_label
 */
void set_PE_OUTPUT_MDOE(void)
{
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_0, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_1, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_2, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_3, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_4, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_5, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_6, AFIO_MODE_DEFAULT);
  AFIO_GPxConfig(GPIO_PE, AFIO_PIN_7, AFIO_MODE_DEFAULT);
  
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_0, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_1, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_2, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_3, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_4, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_5, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_6, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOE, GPIO_PIN_7, GPIO_DIR_OUT);
  
  write_PE0_RESET_0;
  write_PE1_RESET_0;
  write_PE2_RESET_0;
  write_PE3_RESET_0;
  write_PE4_RESET_0;
  write_PE5_RESET_0;
  write_PE6_RESET_0;
  write_PE7_RESET_0;
}

//============================================================
/**
 *  @ingroup nRF2401_Host_FSM1_private_by_Holtek_label
 */
static void init_mcu(void)
{
    //GPIO for software SPI usage
    init_softspi();

    //GPIO <--- IRQ
    init_GPIO_IRQ();
    
    //GPIO debug usage
    set_PE_OUTPUT_MDOE();

    #if   _HOST_TESTMODE_ == 2 //PTX
        hfsm1.trmode = TRMODE_TX32 ;
    #elif _HOST_TESTMODE_ == 3 //PRX
        hfsm1.trmode = TRMODE_RX32 ;
    #endif
}

////////////////////////////////////////
/**
 *  @ingroup nRF2401_Host_FSM1_public_by_Holtek_label
 */
static void debug_rxpackets_from_ptx_not_consecutive(unsigned char len, unsigned char *pPayload)
{
    static unsigned char expected_length_from_ptx;
    static unsigned char expected_payload_0_from_ptx;
    unsigned char i;
    bool isCorrectPayload = TRUE;
    for(i=0; i<len; i++) {
        if( pPayload[i] != ((expected_payload_0_from_ptx+i)&0xFF) ) isCorrectPayload=FALSE;
    }
    if (//expected_length_from_ptx != len  ||
                isCorrectPayload != TRUE )
    {
GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);
        uart_puts("ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "); uart_putchar_n('\n');uart_putchar_n('\n');uart_putchar_n('\n');
    }
    expected_length_from_ptx = len+1;
    if(expected_length_from_ptx>32) {expected_length_from_ptx=1;}
    expected_payload_0_from_ptx = pPayload[0]+1;
}
static void debug_rxpackets_from_prx_not_consecutive(unsigned char len, unsigned char *pPayload)
{
    static unsigned char expected_length_from_prx;
    static unsigned char expected_payload_0_from_prx;
    unsigned char i;
    bool isCorrectPayload = TRUE;
    for(i=0; i<len; i++) {
        if( pPayload[i] != ((expected_payload_0_from_prx+i)&0xFF) ) isCorrectPayload=FALSE;
    }
    if (//expected_length_from_prx != len  ||
                isCorrectPayload != TRUE )
    {
GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);
        uart_puts("ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "); uart_putchar_n('\n');uart_putchar_n('\n');uart_putchar_n('\n');
    }
    expected_length_from_prx = len+1;
    if(expected_length_from_prx>32) {expected_length_from_prx=1;}
    expected_payload_0_from_prx = pPayload[0]+1;
}
/**
 *  @ingroup nRF2401_Host_FSM1_private_by_Holtek_label
 */
/*
	const u8 pipe0_addr[5] = { 0xe1, 0xe2, 0xf2, 0x23, 0x7F };  //pipe 0
// const u8 pipe0_addr[5] = { 0xf2, 0xf1, 0xe2, 0x23, 0x7F };  //pipe 0
//	const u8 pipe0_addr[5] = { 0xe1, 0xe2, 0xf2, 0x32, 0x7F };  //pipe 0
//	const u8 pipe0_addr[5] = { 0xf2, 0xf1, 0xe2, 0x23, 0x7f };  //pipe 0
//	const u8 pipe0_addr[5] = { 0x78, 0x56, 0x34, 0x12, 0x71 };  //pipe 0	
  const u8 pipe1_addr[5] = { 0x78, 0x56, 0x34, 0x12, 0x71 };  //pipe 1
  const u8 pipe2_addr[5] = { 0x78, 0x56, 0x34, 0x12, 0x72 };  //pipe 2
  const u8 pipe3_addr[5] = { 0x78, 0x56, 0x34, 0x12, 0x73 };  //pipe 3
  const u8 pipe4_addr[5] = { 0x78, 0x56, 0x34, 0x12, 0x74 };  //pipe 4
  const u8 pipe5_addr[5] = { 0x78, 0x56, 0x34, 0x12, 0x75 };  //pipe 5
*/
// const u8 pipe1_addr[5] = { 0xf2, 0xf1, 0xe2, 0x23, 0x71 };  //pipe 1
// const u8 pipe2_addr[5] = { 0xf2, 0xf1, 0xe2, 0x23, 0x72 };  //pipe 2
// const u8 pipe3_addr[5] = { 0xf2, 0xf1, 0xe2, 0x23, 0x73 };  //pipe 3
// const u8 pipe4_addr[5] = { 0xf2, 0xf1, 0xe2, 0x23, 0x74 };  //pipe 4
// const u8 pipe5_addr[5] = { 0xf2, 0xf1, 0xe2, 0x23, 0x75 };  //pipe 5	
	
/*
	const u8 pipe0_addr[5] = { 0xe1, 0xe2, 0xf2, 0x32, 0x7f };  //pipe 0
  const u8 pipe1_addr[5] = { 0x78, 0x56, 0x7f, 0x5f, 0x71 };  //pipe 1
  const u8 pipe2_addr[5] = { 0x78, 0x56, 0x7f, 0x5f, 0x72 };  //pipe 2
  const u8 pipe3_addr[5] = { 0x78, 0x56, 0x7f, 0x5f, 0x73 };  //pipe 3
  const u8 pipe4_addr[5] = { 0x78, 0x56, 0x7f, 0x5f, 0x74 };  //pipe 4
  const u8 pipe5_addr[5] = { 0x78, 0x56, 0x7f, 0x5f, 0x75 };  //pipe 5*/

/*
 // const u8 pipe0_addr[5] = { 0xe2, 0xe1, 0xf2, 0x23, 0x7f };  //pipe 0
  const u8 pipe1_addr[5] = { 0x76, 0x54, 0x34, 0x12, 0x71 };  //pipe 1
  const u8 pipe2_addr[5] = { 0x76, 0x54, 0x34, 0x12, 0x72 };  //pipe 2
  const u8 pipe3_addr[5] = { 0x76, 0x54, 0x34, 0x12, 0x73 };  //pipe 3
  const u8 pipe4_addr[5] = { 0x76, 0x54, 0x34, 0x12, 0x74 };  //pipe 4
  const u8 pipe5_addr[5] = { 0x76, 0x54, 0x34, 0x12, 0x75 };  //pipe 5
  const u8 pipe0_addr[5] = { 0x76, 0x54, 0x34, 0x12, 0x7F };  //pipe 0*/

  //const u8 pipe0_addr[5] = { 0x72, 0xE1, 0xF2, 0x26, 0x7F };  //pipe 0
// const u8 pipe0_addr[5] = { 0xe2, 0xe1, 0xf2, 0x23, 0x7f };  //pipe 0
	const u8 pipe0_addr[5] = { 0xD1,0x4A,0xF2,0xEE,0x07 };
 // const u8 pipe0_addr[5] = { 0xF2, 0xe1, 0xf2, 0x26, 0x7F };
  const u8 pipe1_addr[5] = { 0xF2, 0xE1, 0xF6, 0x23, 0x71 };  //pipe 1
  const u8 pipe2_addr[5] = { 0xF2, 0xE1, 0xF6, 0x23, 0x72 };  //pipe 2
  const u8 pipe3_addr[5] = { 0xF2, 0xE1, 0xF6, 0x23, 0x73 };  //pipe 3
  const u8 pipe4_addr[5] = { 0xF2, 0xE1, 0xF6, 0x23, 0x74 };  //pipe 4
  const u8 pipe5_addr[5] = { 0xF2, 0xE1, 0xF6, 0x23, 0x75 };  //pipe 5

unsigned char buf[MAX_DATA_SIZE];
//unsigned char* str = (unsigned char*)"012345678901234567890123456789012";
  unsigned char* str = (unsigned char*)"@ABCDEFGHI@ABCDEFGHI@ABCDEFGHI@AB";
  unsigned char str2[256];
    static unsigned char zzz_pipe=0;
static void rewrite_syncID(void)
{
//  zzz_pipe++; if(zzz_pipe>3) zzz_pipe=0;
    zzz_pipe++; if(zzz_pipe>5) zzz_pipe=0;
		zzz_pipe=0;
    if     (zzz_pipe==0) {rf_SetPipeAddress(0,pipe0_addr);}
    else if(zzz_pipe==1) {rf_SetPipeAddress(0,pipe1_addr);}
    else if(zzz_pipe==2) {rf_SetPipeAddress(0,pipe2_addr);}
    else if(zzz_pipe==3) {rf_SetPipeAddress(0,pipe3_addr);}
    else if(zzz_pipe==4) {rf_SetPipeAddress(0,pipe4_addr);}
    else if(zzz_pipe==5) {rf_SetPipeAddress(0,pipe5_addr);}
}
static void dly_15us(void)
{
    unsigned char v;
    v=225;//15us
    while(v--) {;}
}
static void ce_15us_high_then_low(void)
{
    spi_SetCEState(TRUE);
    dly_15us();
    spi_SetCEState(FALSE);
     // uart_puts("ce_15us_high_then_low"); uart_putchar_n('\n');//debug
}
static void ce_keep_high(void)
{
    spi_SetCEState(TRUE);
      uart_puts("ce_keep_high"); uart_putchar_n('\n');//debug
}
#define TX_PKT_CNT 1001
void DueMain_main(void)
{
    u8 status7, rrr;
    ReceiveResults res;
    static volatile u8 cnt = 1;
		static unsigned char ini_deta=0x2F;
    int i;


    rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, TRUE);
		//rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, FALSE ); //FALSE: _use_dynamic_payload =0
    rf_ConfigurePipe(0, pipe0_addr);
    
    rf_ResetStatus(e_mode_both);
    rf_FlushBuffer(e_flush_both);

    //Power up the RF in transmit mode
    rf_PowerUp(e_mode_xmit);
		cnt =1;
		for(i=0; i<cnt; i++) {
                    str2[i]= (ini_deta+i);
    }
		//str2[0]= 0xF0;
	  ini_deta++;
	 // rf_SendData(0, str2, cnt);
	rf_SendData(0, str2, 32);
//rf_SendCmd_1(REUSE_TX_PL);//debug
 // ce_keep_high();
//ce_15us_high_then_low();
		debug_i=1;
    while (1){
			
		if(debug_i < TX_PKT_CNT)
    {

				ce_15us_high_then_low();
//uart_putchar_n('\n');//debug
          //ce_15us_high_then_low();

        do 
        {
            status7 = rf_GetStatus();
        } while(!(status7 & _BV(MAX_RT)) && !(status7 & _BV(TX_DS)) && !(status7 & _BV(RX_DR))  );
//uart_puts("status7="); uart_putu8(status7); uart_putchar_n('\n');//debug
        
        if (status7 & _BV(MAX_RT))
        {
                rf_SendCmd_2(STATUS, _BV(MAX_RT));  //STATUS.[6:4] are interrupt flags, write 1 to clear bit
								debug_MAX_RT++;
								
//ce_15us_high_then_low();
//uart_puts("MAX_RT"); uart_putchar_n('\n');//debug
              //rf_FlushBuffer(e_flush_xmit);
              //rf_SendCmd_1(REUSE_TX_PL);//debug
              //rf_SendData(0, str2, cnt);
//uart_putchar_n('\n');
//uart_puts("MAXRT  SETUP_RETR=");  rrr=rf_ReadRegister(SETUP_RETR);  uart_putu8(rrr); uart_puts("  OBSERVE_TX=");  rrr=rf_ReadRegister(OBSERVE_TX);  uart_putu8(rrr); //uart_putchar_n('\n');//debug
        uart_puts("RT:");uart_putu16(debug_i);uart_puts("  MAX_RT:");uart_putu16(debug_MAX_RT);uart_puts("  TX_DS:");uart_putu16(debug_TX_DS);uart_putchar_n('\n');			
				#if 0
					if(debug_i <1000)						rf_SendData(0, str2, cnt);
									debug_i++;
				#else
							
				rf_SendCmd_1(FLUSH_TX);
				if (++cnt > MAX_DATA_SIZE)	{cnt = 1;}
				cnt = 32;
				rewrite_syncID();
				/*
					if     (zzz_pipe==0) {cnt=1;}
					else if(zzz_pipe==1) {cnt=8;}
					else if(zzz_pipe==2) {cnt=16;}
					else if(zzz_pipe==3) {cnt=24;}
					else if(zzz_pipe==4) {cnt=28;}
					else if(zzz_pipe==5) {cnt=32;}*/
				for(i=0; i<cnt; i++) {
                    str2[i]= (ini_deta+i);
                }
                ini_deta++;
 

								if(debug_i < TX_PKT_CNT)	
									{
									rf_SendData(0, str2, cnt);
												debug_i++;
									}
							  else
									{
										    rf_FlushBuffer(e_flush_both);
									}
				#endif
				}
				
        if (status7 & _BV(TX_DS))
        {
            //static unsigned char ini_deta=0x2F;
u8 i;
					debug_TX_DS++;
//uart_putchar_n('\n');
uart_puts("RT:");uart_putu16(debug_i);uart_puts("  MAX_RT:");uart_putu16(debug_MAX_RT);uart_puts("  TX_DS:");uart_putu16(debug_TX_DS);uart_putchar_n('\n');			
//uart_puts("TX_DS  SETUP_RETR=");  rrr=rf_ReadRegister(SETUP_RETR);  uart_putu8(rrr); uart_puts("  OBSERVE_TX=");  rrr=rf_ReadRegister(OBSERVE_TX);  uart_putu8(rrr); //uart_putchar_n('\n');//debug
//uart_puts("TX_DS"); uart_putchar_n('\n');//debug
            if (++cnt > MAX_DATA_SIZE)
                cnt = 1;

						cnt = 32;
            ///////////////////////////
            res = rf_IsDataAvailable();
            if (res.status == e_rs_data)
            {
                rf_SendCmd_2(STATUS, _BV(TX_DS)|_BV(RX_DR));   //STATUS.[6:4] are interrupt flags, write 1 to clear bit
                rf_ReceiveData((u8*)buf, res.packet_sz);
							
                debug_rxpackets_from_prx_not_consecutive(res.packet_sz, buf+0);
uart_puts("  packet_sz=");  uart_putu8(res.packet_sz); uart_puts(" ");//debug
uart_puts("ReceiveData=");//debug
for(i=0; i<res.packet_sz; i++) {
    uart_puts(" ");
    uart_putu8(buf[i]);
}
uart_putchar_n('\n');//debug
            }
            else {
                rf_SendCmd_2(STATUS, _BV(TX_DS));              //STATUS.[6:4] are interrupt flags, write 1 to clear bit
            }
rewrite_syncID();
						/*
					if     (zzz_pipe==0) {cnt=32;}
					else if(zzz_pipe==1) {cnt=8;}
					else if(zzz_pipe==2) {cnt=16;}
					else if(zzz_pipe==3) {cnt=24;}
					else if(zzz_pipe==4) {cnt=28;}
					else if(zzz_pipe==5) {cnt=32;}*/

						if(cnt>MAX_DATA_SIZE){cnt=1;}
              //rf_SendCmd_1(REUSE_TX_PL);//debug
//cnt=1; ini_deta=0x30;//debug
                for(i=0; i<cnt; i++) {
                    str2[i]= (ini_deta+i);
                }
                ini_deta++;
                if(debug_i < TX_PKT_CNT)	
									{
									rf_SendData(0, str2, cnt);
												debug_i++;
									}
							  else
									{
										    rf_FlushBuffer(e_flush_both);
									}
								//rf_SendData(0, str2, 01);
//ce_15us_high_then_low();
        }
  //    delay_unit_50us(20000);// 1 sec
  //    delay_unit_50us(5000);
				debug_i=1;
      //delay_unit_50us(100);
				   delay_unit_50us(200); //200*50us=10ms > 6.3ms (remote 5602 uart print take ~6.3ms)

    }
		else
		{
													    rf_FlushBuffer(e_flush_both);
		}
}
		}
int UnoClient_main(void)
{
    ReceiveResults res;
    u8 status;
    volatile u8 cnt = 1;
    
uart_puts("nrf24L01+  PRX   CH="); uart_putu8(DEF_RF_CHAN); uart_putchar_n('\n');

    //rf_chan is calculated; 2400MHz + (rf_chan * 1MHz)
    //;If we pass a payload_sz of DYN_PAYLOAD_SIZE (0) this will enable
    //    dynamic payload size and if we pass DEF_PAYLOAD_SIZE 0r any value 1-32
    //    means we are using a static payload size of that size.
    //The available data reates are; 250k, 1M and 2M
    //The last param determines if we will be using dynamic payload size.
    rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, TRUE);
		//rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, FALSE); //FALSE: _use_dynamic_payload =0
    rf_ConfigurePipe(0, pipe0_addr);
    rf_ConfigurePipe(1, pipe1_addr);
    rf_ConfigurePipe(2, pipe2_addr);
    rf_ConfigurePipe(3, pipe3_addr);
    rf_ConfigurePipe(4, pipe4_addr);
    rf_ConfigurePipe(5, pipe5_addr);
    
    rf_ResetStatus(e_mode_both);
    rf_FlushBuffer(e_flush_both);
    
    //Power up the RF in receive mode
    rf_PowerUp(e_mode_recv);
    
    //Start listening
    rf_Listen();
    
    //rf_SendAckPayload(0, str2, 1);
    rf_SendAckPayload(0, str2, 32);
    while(1)
    {
         unsigned char i;
            static unsigned char ini_deta=0x2F;
            static unsigned char rrr_pipe=0;
						
            if(  (rf_GetFifoStatus() & 0x20)==0x00     ) {  // available location in TX FIFO
                if (++cnt > MAX_DATA_SIZE) {cnt = 1;}
              //if (++cnt > 17) {cnt = 1;}
cnt=32;
                for(i=0; i<cnt; i++) {
                    str2[i]= (ini_deta+i);

                }
                ini_deta++;
								rrr_pipe=0;
               //rrr_pipe++; if(rrr_pipe>5) rrr_pipe=0;
                          //if(rrr_pipe==0) rrr_pipe=1;//skip pipe 0
                          //if(rrr_pipe==1) rrr_pipe=2;//skip pipe 1
//rrr_pipe=0;//debug
                rf_SendAckPayload(rrr_pipe, str2, cnt);
								//rf_SendAckPayload(rrr_pipe, str2, 1);
            }
			if(GPIO_IRQ_IS_RESET()){

        status = rf_GetStatus();
        ///////////////////////
        if( (status & _BV(TX_DS)) || (status & _BV(MAX_RT)))
        {
            rf_SendCmd_2(STATUS, _BV(TX_DS) | _BV(MAX_RT) );  //STATUS.[6:4] are interrupt flags, write 1 to clear bit
        }
        ///////////////////////
      //if( rf_GetFifoStatus() & 0x02 ) { // RX_FULL    test: wait RX_FULL then read
        do {
        res = rf_IsDataAvailable();
        if (res.status == e_rs_data)
        {
						debug_i++;
          //rf_ResetStatus(e_mode_recv);
            rf_SendCmd_2(STATUS, _BV(RX_DR));  //STATUS.[6:4] are interrupt flags, write 1 to clear bit
            rf_ReceiveData((u8*)buf, res.packet_sz);
					
            debug_rxpackets_from_ptx_not_consecutive(res.packet_sz, buf+0);
					
uart_puts("packet_i=");       uart_putu16(debug_i);   uart_puts(" ");//debug		uart_putchar_n('\n');//debug	
					
uart_puts("pipe=");       uart_putu8(res.pipe_no);   uart_puts(" ");//debug
uart_puts("packet_sz=");  uart_putu8(res.packet_sz); uart_puts(" ");//debug
uart_puts("ReceiveData=");
//for(i=0; i<res.packet_sz; i++) {
for(i=0; i<1; i++) {
    uart_puts(" ");
    uart_putu8(buf[i]);//debug
}
uart_putchar_n('\n');//debug
            /*
            if(  (rf_GetFifoStatus() & 0x20)==0x00     ) {  // available location in TX FIFO
                if (++cnt > MAX_DATA_SIZE) {cnt = 1;}
              //if (++cnt > 17) {cnt = 1;}
//cnt=1;
                for(i=0; i<cnt; i++) {
                    str2[i]= (ini_deta+i);
                }
                ini_deta++;
                rf_SendAckPayload(res.pipe_no, str2, cnt);
            }*/
        }
        } while( !(rf_GetFifoStatus() & 0x01) ) ; // RX_EMPTY
      //}
        
    }//GPIO_IRQ
	}
}

////////////////////////////////////////
/**
 *  @ingroup nRF2401_Host_FSM1_public_by_Holtek_label
 */
void host24L01_process(void)
{
    #if   DATARATE == 250
    hfsm1.rfdatarate = e_dr_250Kbps;
    #elif DATARATE == 1000
    hfsm1.rfdatarate = e_dr_1Mbps;
    #elif DATARATE == 2000
    hfsm1.rfdatarate = e_dr_2Mbps;
    #endif
        
    init_mcu();
        
    while(1)
    {
        if (hfsm1.trmode == TRMODE_TX32) {
						
            DueMain_main();
        }
        if (hfsm1.trmode == TRMODE_RX32) {
            UnoClient_main();
        }
            
    }//while
    
}



