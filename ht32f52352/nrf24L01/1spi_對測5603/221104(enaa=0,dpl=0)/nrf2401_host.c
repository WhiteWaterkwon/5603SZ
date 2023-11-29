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
#define TRMODE_TX32            0x02        // ptx   (  0 ~  32 bytes)
#define TRMODE_RX32            0x03        // prx   (  0 ~  32 bytes)

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
#define IRQ_IS_SET          (GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_2)==SET)
#define IRQ_IS_RESET        (GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_2)==RESET)

/**
 *  @ingroup IRQ_to_GPIO_label
 */
static void init_GPIO_IRQ(void)
{
    //GPIO <--- IRQ
    AFIO_GPxConfig(          GPIO_PA, AFIO_PIN_2, AFIO_MODE_1);
    GPIO_DirectionConfig(   HT_GPIOA, GPIO_PIN_2, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_2, GPIO_PR_UP);
    GPIO_InputConfig(       HT_GPIOA, GPIO_PIN_2, ENABLE);
}
/**
 *  @ingroup IRQ_to_GPIO_label
 */
/*
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
}*/

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
    static unsigned char expected_length_from_ptx=1;
    static unsigned char expected_payload_0_from_ptx=0x30;
    unsigned char i;
    bool isCorrectPayload = TRUE;
    for(i=0; i<len; i++) {
        if( pPayload[i] != ((expected_payload_0_from_ptx+i)&0xFF) ) isCorrectPayload=FALSE;
    }
    if (
                isCorrectPayload != TRUE )
    {
        uart_puts("ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "); uart_putchar_n('\n');uart_putchar_n('\n');uart_putchar_n('\n');
    }
    expected_length_from_ptx = len+1;
    if(expected_length_from_ptx>32) {expected_length_from_ptx=1;}
    expected_payload_0_from_ptx = pPayload[0]+1;
}
static void debug_rxpackets_from_prx_not_consecutive(unsigned char len, unsigned char *pPayload)
{
    static unsigned char expected_length_from_prx=1;
    static unsigned char expected_payload_0_from_prx=0x30;
    unsigned char i;
    bool isCorrectPayload = TRUE;
    for(i=0; i<len; i++) {
        if( pPayload[i] != ((expected_payload_0_from_prx+i)&0xFF) ) isCorrectPayload=FALSE;
    }
    if (
                isCorrectPayload != TRUE )
    {
        uart_puts("ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "); uart_putchar_n('\n');uart_putchar_n('\n');uart_putchar_n('\n');
    }
    expected_length_from_prx = len+1;
    if(expected_length_from_prx>32) {expected_length_from_prx=1;}
    expected_payload_0_from_prx = pPayload[0]+1;
}
/**
 *  @ingroup nRF2401_Host_FSM1_private_by_Holtek_label
 */
  unsigned char buf[MAX_DATA_SIZE];
  unsigned char str2[256];
  static unsigned char zzz_pipe=0;
  
  const u8 pipe0_addr[5] = { (u8)(P0_SYNCWORD>>24), (u8)(P0_SYNCWORD>>16), (u8)(P0_SYNCWORD>>8), (u8)(P0_SYNCWORD), P0_LSBYTE };  //pipe 0
  const u8 pipe1_addr[5] = { (u8)(P1_SYNCWORD>>24), (u8)(P1_SYNCWORD>>16), (u8)(P1_SYNCWORD>>8), (u8)(P1_SYNCWORD), P1_LSBYTE };  //pipe 1
  const u8 pipe2_addr[5] = { (u8)(P1_SYNCWORD>>24), (u8)(P1_SYNCWORD>>16), (u8)(P1_SYNCWORD>>8), (u8)(P1_SYNCWORD), P2_LSBYTE };  //pipe 2
  const u8 pipe3_addr[5] = { (u8)(P1_SYNCWORD>>24), (u8)(P1_SYNCWORD>>16), (u8)(P1_SYNCWORD>>8), (u8)(P1_SYNCWORD), P3_LSBYTE };  //pipe 3
  const u8 pipe4_addr[5] = { (u8)(P1_SYNCWORD>>24), (u8)(P1_SYNCWORD>>16), (u8)(P1_SYNCWORD>>8), (u8)(P1_SYNCWORD), P4_LSBYTE };  //pipe 4
  const u8 pipe5_addr[5] = { (u8)(P1_SYNCWORD>>24), (u8)(P1_SYNCWORD>>16), (u8)(P1_SYNCWORD>>8), (u8)(P1_SYNCWORD), P5_LSBYTE };  //pipe 5

static void rewrite_syncID(void)
{
    zzz_pipe++; if(zzz_pipe>5) zzz_pipe=0;
//zzz_pipe=0;
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
void ce_15us_high_then_low(void)
{
    spi_SetCEState(TRUE);
    dly_15us();
    spi_SetCEState(FALSE);
//  uart_puts("ce_15us_high_then_low"); uart_putchar_n('\n');//debug
}
void ce_keep_high(void)
{
    spi_SetCEState(TRUE);
//  uart_puts("ce_keep_high"); uart_putchar_n('\n');//debug
}
static unsigned int count_sent ;
static unsigned int count_txds ;
static unsigned int count_maxrt ;
static void foo_ptx_print_count_send(unsigned char istxds)
{
               count_sent++;
    if(istxds) count_txds++;
    else       count_maxrt++;
    #if TOTAL_NUM_TEST_PKTS != 0
    uart_puts("count_sent=");   uart_putu16(count_sent);
    uart_puts(" txds=");        uart_putu16(count_txds);
    uart_puts(" maxrt=");       uart_putu16(count_maxrt);
    uart_puts(" ");
//  uart_putchar_n('\n');
    #endif
}
void DueMain_main(void)
{
    unsigned char i;
    unsigned char status7, rrr;
    ReceiveResults res;
    static volatile unsigned char cnt = 1;
    static volatile unsigned char ini_deta;
uart_putchar_n('\n');
uart_puts("nrf24L01+  PTX   CH="); uart_putu8(DEF_RF_CHAN); uart_putchar_n('\n');

  //rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, TRUE);
    rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, FALSE ); //FALSE: _use_dynamic_payload =0
    rf_ConfigurePipe(0, pipe0_addr, RXPWP0); // RXPWP0 unused in DPL=1 mode
    
    rf_ResetStatus(e_mode_both);
    rf_FlushBuffer(e_flush_both);

    //Power up the RF in transmit mode
    rf_PowerUp(e_mode_xmit);
    
count_sent=0;
count_txds=0;
count_maxrt=0;

ini_deta=0x30;
    if     (zzz_pipe==0) {cnt=RXPWP0;  rf_ConfigurePipe(0, pipe0_addr, RXPWP0);}
    else if(zzz_pipe==1) {cnt=RXPWP1;  rf_ConfigurePipe(0, pipe1_addr, RXPWP1);}
    else if(zzz_pipe==2) {cnt=RXPWP2;  rf_ConfigurePipe(0, pipe2_addr, RXPWP2);}
    else if(zzz_pipe==3) {cnt=RXPWP3;  rf_ConfigurePipe(0, pipe3_addr, RXPWP3);}
    else if(zzz_pipe==4) {cnt=RXPWP4;  rf_ConfigurePipe(0, pipe4_addr, RXPWP4);}
    else if(zzz_pipe==5) {cnt=RXPWP5;  rf_ConfigurePipe(0, pipe5_addr, RXPWP5);}
for(i=0; i<cnt; i++) {
    str2[i]= (ini_deta+i);
}
ini_deta++;
rf_SendData(0, str2, cnt);
//rf_SendCmd_1(REUSE_TX_PL);//debug
  ce_keep_high();
//ce_15us_high_then_low();
    while (1)
    {
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
//ce_15us_high_then_low();
              //rf_FlushBuffer(e_flush_xmit);
              //rf_SendCmd_1(REUSE_TX_PL);//debug
              //rf_SendData(0, str2, cnt);
uart_putchar_n('\n');
foo_ptx_print_count_send(0);
uart_puts(" zzz_pipe=");   uart_putu8(zzz_pipe);      //debug
uart_puts(" MAXRT  SETUP_RETR=");  rrr=rf_ReadRegister(SETUP_RETR);  uart_putu8(rrr); uart_puts("  OBSERVE_TX=");  rrr=rf_ReadRegister(OBSERVE_TX);  uart_putu8(rrr); //uart_putchar_n('\n');//debug
                    #if TOTAL_NUM_TEST_PKTS != 0
                    if ( count_sent >= TOTAL_NUM_TEST_PKTS ) {
                        spi_SetCEState(FALSE);
                        rf_ResetStatus(e_mode_both);
                        rf_FlushBuffer(e_flush_both);
                        hfsm1.trmode=0;
                        break;
                    }
                    #endif
        }
        if (status7 & _BV(TX_DS))
        {
uart_putchar_n('\n');
foo_ptx_print_count_send(1);
uart_puts(" zzz_pipe=");   uart_putu8(zzz_pipe);      //debug
uart_puts(" TX_DS  SETUP_RETR=");  rrr=rf_ReadRegister(SETUP_RETR);  uart_putu8(rrr); uart_puts("  OBSERVE_TX=");  rrr=rf_ReadRegister(OBSERVE_TX);  uart_putu8(rrr); //uart_putchar_n('\n');//debug
                    #if TOTAL_NUM_TEST_PKTS != 0
                    if ( count_sent >= TOTAL_NUM_TEST_PKTS ) {
                        spi_SetCEState(FALSE);
                        rf_ResetStatus(e_mode_both);
                        rf_FlushBuffer(e_flush_both);
                        hfsm1.trmode=0;
                        break;
                    }
                    #endif
            if (++cnt > MAX_DATA_SIZE)
                cnt = 1;
            ///////////////////////////
            res = rf_IsDataAvailable();
            if (res.status == e_rs_data)
            {
                rf_SendCmd_2(STATUS, _BV(TX_DS)|_BV(RX_DR));   //STATUS.[6:4] are interrupt flags, write 1 to clear bit
                rf_ReceiveData((u8*)buf, res.packet_sz);
                debug_rxpackets_from_prx_not_consecutive(res.packet_sz, buf+0);
uart_puts(" packet_sz=");  uart_putu8(res.packet_sz); //debug
uart_puts(" ReceiveData=");//debug
for(i=0; i<res.packet_sz; i++) {
    uart_puts(" ");
    uart_putu8(buf[i]);
}
//uart_putchar_n('\n');//debug
            }
            else {
                rf_SendCmd_2(STATUS, _BV(TX_DS));              //STATUS.[6:4] are interrupt flags, write 1 to clear bit
            }
rewrite_syncID();
    if     (zzz_pipe==0) {cnt=RXPWP0;  rf_ConfigurePipe(0, pipe0_addr, RXPWP0);}
    else if(zzz_pipe==1) {cnt=RXPWP1;  rf_ConfigurePipe(0, pipe1_addr, RXPWP1);}
    else if(zzz_pipe==2) {cnt=RXPWP2;  rf_ConfigurePipe(0, pipe2_addr, RXPWP2);}
    else if(zzz_pipe==3) {cnt=RXPWP3;  rf_ConfigurePipe(0, pipe3_addr, RXPWP3);}
    else if(zzz_pipe==4) {cnt=RXPWP4;  rf_ConfigurePipe(0, pipe4_addr, RXPWP4);}
    else if(zzz_pipe==5) {cnt=RXPWP5;  rf_ConfigurePipe(0, pipe5_addr, RXPWP5);}
              //rf_SendCmd_1(REUSE_TX_PL);//debug
//cnt=32; ini_deta=0x30;//debug
                for(i=0; i<cnt; i++) {
                    str2[i]= (ini_deta+i);
                }
                ini_deta++;
                rf_SendData(0, str2, cnt);
    #if 0
    {
        static unsigned char change_channel;
        change_channel++;
        if(change_channel&0x01)
        rf_SendCmd_2(RF_CH, DEF_RF_CHAN^0x20);
        else
        rf_SendCmd_2(RF_CH, DEF_RF_CHAN);
    }
    #endif
//ce_15us_high_then_low();
        }
        delay_unit_50us(300); //200*50us=10ms > 6.3ms (remote 5602 uart print take ~6.3ms)
    }
}
static volatile unsigned int count_roger ;
int UnoClient_main(void)
{
    ReceiveResults res;
    unsigned char i;
    unsigned char status;
    static volatile unsigned char cnt = 1;
    static volatile unsigned char ini_deta=0x2F;
uart_putchar_n('\n');
uart_puts("nrf24L01+  PRX   CH="); uart_putu8(DEF_RF_CHAN); uart_putchar_n('\n');

    //rf_chan is calculated; 2400MHz + (rf_chan * 1MHz)
    //;If we pass a payload_sz of DYN_PAYLOAD_SIZE (0) this will enable
    //    dynamic payload size and if we pass DEF_PAYLOAD_SIZE 0r any value 1-32
    //    means we are using a static payload size of that size.
    //The available data reates are; 250k, 1M and 2M
    //The last param determines if we will be using dynamic payload size.
  //rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, TRUE);
    rf_Init(DEF_RF_CHAN, DEF_PAYLOAD_SIZE, hfsm1.rfdatarate, FALSE); //FALSE: _use_dynamic_payload =0
    rf_ConfigurePipe(0, pipe0_addr, RXPWP0);
    rf_ConfigurePipe(1, pipe1_addr, RXPWP1);
    rf_ConfigurePipe(2, pipe2_addr, RXPWP2);
    rf_ConfigurePipe(3, pipe3_addr, RXPWP3);
    rf_ConfigurePipe(4, pipe4_addr, RXPWP4);
    rf_ConfigurePipe(5, pipe5_addr, RXPWP5);
    
    rf_ResetStatus(e_mode_both);
    rf_FlushBuffer(e_flush_both);
    
    //Power up the RF in receive mode
    rf_PowerUp(e_mode_recv);
    
    //Start listening
    rf_Listen();
count_roger=0;
    
    while(1)
    {
            static unsigned char rrr_pipe=0;
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
          //rf_ResetStatus(e_mode_recv);
            rf_SendCmd_2(STATUS, _BV(RX_DR));  //STATUS.[6:4] are interrupt flags, write 1 to clear bit
            rf_ReceiveData((u8*)buf, res.packet_sz);
            debug_rxpackets_from_ptx_not_consecutive(res.packet_sz, buf+0);
uart_puts("count_roger=");   uart_putu16(++count_roger);
uart_puts(" pipe=");         uart_putu8(res.pipe_no);
uart_puts(" packet_sz=");    uart_putu8(res.packet_sz);
uart_puts(" ReceiveData=");
for(i=0; i<res.packet_sz; i++) {
    uart_puts(" ");
    uart_putu8(buf[i]);//debug
}
uart_putchar_n('\n');//debug
        }
        } while( !(rf_GetFifoStatus() & 0x01) ) ; // RX_EMPTY
      //}
        
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



