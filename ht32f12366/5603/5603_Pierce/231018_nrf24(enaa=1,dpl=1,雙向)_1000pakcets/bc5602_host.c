/*********************************************************************************************************//**
 * @file    bc5602_host.c
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
#include "sync_bch.h"
#include "bc5602_host.h"
#include "bc5602_host2.h"

/* Private variables ---------------------------------------------------------------------------------------*/
int debug_i=0;
int debug_MAX_RT=0;
int debug_TX_DS=0;
/* Global functions ----------------------------------------------------------------------------------------*/



/////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @addtogroup BC3601_Host_FSM1_public_by_Holtek_label
 *  @{
 */
typedef struct {
    unsigned char   trmode;
    unsigned int    state;
    u32             delay_offset;
} host5602_fsm1_holtek_TypeDef;
static volatile host5602_fsm1_holtek_TypeDef hfsm1;
/** @} */



/**
 *  @addtogroup SPI_spiTxQ_label
 *  @{
 */
hfsm1_spiTxQ_to_5602_TypeDef spiTxQ;
/** @} */

/**
 *  @addtogroup SPI_spiRxQ_label
 *  @{
 */
hfsm1_spiRxQ_from_5602_TypeDef spiRxQ;
/** @} */


/********************************************************************************/
#define IRQ3_IS_SET        (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)==SET)
#define IRQ3_IS_RESET      (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_5)==RESET)

/**
 *  @ingroup IRQ3_to_GPIO_label
 */
static void init_GPIO_IRQ3(void)
{
    //GPIO <--- IRQ3
    AFIO_GPxConfig(GPIO_PD, AFIO_PIN_5, AFIO_MODE_1);
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_5, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_5, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOD, GPIO_PIN_5, ENABLE);
    //GPIO <--- IRQ3
    AFIO_GPxConfig(GPIO_PD, AFIO_PIN_14, AFIO_MODE_1);
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_14, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_14, GPIO_PR_UP);
    GPIO_InputConfig(HT_GPIOD, GPIO_PIN_14, ENABLE);
		//DEBUG4

}


/**
 *  @ingroup IRQ3_to_GPIO_label
 */
/*
static bool GPIO_IRQ3_IS_SET(void)
{
    bool result;
    if(IRQ3_IS_SET) {result = TRUE;}
    else            {result = FALSE;}
    return result;
}*/
/**
 *  @ingroup IRQ3_to_GPIO_label
 */
static bool GPIO_IRQ3_IS_RESET(void)
{
    bool result;
    if(IRQ3_IS_RESET) {result = TRUE;}
    else              {result = FALSE;}
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
	write_PE1_RESET_0;
	write_PE1_SET_1;
	write_PE1_RESET_0;
	write_PE1_SET_1;
	write_PE1_RESET_0;
	write_PE1_SET_1;
}

//============================================================
static volatile unsigned char dm1_mdiv_sdr ;
static volatile unsigned char dm1_mdiv_sel ;

static volatile unsigned char rf_dm1_aw ;

//============================================================
                                                    //      DM1  [7:4]RSSI_CTHD
static volatile unsigned char dm1_aw ;
                                                    //0x00, DM1  [3:2]=00: -
                                                    //0x04, DM1  [3:2]=01: 3 bytes IDCODE
                                                    //0x08, DM1  [3:2]=10: 4 bytes IDCODE
                                                    //0x0C, DM1  [3:2]=11: 5 bytes IDCODE
static volatile unsigned char dm1_d_rate ;
                                                    //0x00, DM1  [1:0]=00: 250 Kbps datarate
                                                    //0x01, DM1  [1:0]=01:   1 Mbps datarate
static volatile unsigned char pkt1_crc_en ;
                                                    //      PKT1 
static volatile unsigned char pkt1_crc8_en ;
                                                    //      PKT1 
static volatile unsigned char pkt2_wht_en ;         //      PKT2 [7]Whitening, [6:0]seed
static volatile unsigned char dpl2_en_dyn_ack ;     //0x01, DPL2 [0]
static volatile unsigned char dpl2_en_ack_pld ;     //0x02, DPL2 [1]
static volatile unsigned char dpl2_en_dpl ;         //0x04, DPL2 [2]  9 bits PCF
static volatile unsigned char en_inv_noack ;        //0x80, DPL2 [7]inverse NO_ACK


  static address5602_TypeDef syncID_p0={P0_SYNCWORD, P0_LSBYTE};
  static address5602_TypeDef syncID_p1={P1_SYNCWORD, P1_LSBYTE};
  static address5602_TypeDef syncID_p2={P1_SYNCWORD, P2_LSBYTE};
  static address5602_TypeDef syncID_p3={P1_SYNCWORD, P3_LSBYTE};
  static address5602_TypeDef syncID_p4={P1_SYNCWORD, P4_LSBYTE};
  static address5602_TypeDef syncID_p5={P1_SYNCWORD, P5_LSBYTE};

//////////////////////////////////////////////////////
static void spi_read_rxfifo_to_spiRxQ(void)
{
    unsigned char read_length;
    read_length = spi_r(0x0C);      //PKT4 RO (strobe R_RX_PL_WID)
                                    //        [7:0](RO)RXDLEN
    if(read_length==0) {
        return;
    }
    spi_cmdd_BF_rxfifo_read(read_length, spiRxQ.payload + 0);
    spiRxQ.w_index = read_length;
}

/**
 *  @ingroup SPI_Commands_label
 */
static unsigned char spi_read_fifostatus(void)
{
    unsigned char status;
    status = spi_r(0x05);       // FIFO STATUS
    return status;
}
unsigned char spi_read_sta1_omst(void)
{
	/* OMST[2:0]: Operation mode state
	0h: Deep Sleep Mode
	1h: Middle Sleep Mode
	2h: Light Sleep Mode
	3h: Standby Mode
	4h: TX Mode
	5h: RX Mode
	6h: Calibration Mode
	*/
    unsigned char sta1;
//bank 0
    spi_cmd0_20_set_register_bank(0);
    sta1 = spi_r(0x26);         // sta1  <--- CR[0x26]STA1
//uart_puts("sta1="); uart_putu8(sta1); uart_putchar_n('\n');
    return sta1;
}
/**
 *  @ingroup SPI_Commands_label
 */
static void spi_clear_irq1(void)
{
    spi_w(0x04, 0x7E);          // IRQ1[7]-
                                //     [6]RX_DR,   write 1 to clear bit
                                //     [5]TX_DS,   write 1 to clear bit
                                //     [4]MAX_RT,  write 1 to clear bit
	                              //     [3:1]RX_P_NO
                                //     [0]TX_FULL
}
static unsigned char spi_read_n_clear_irq1(void)
{
    unsigned char irq1;
    irq1 = spi_r(0x04);         // IRQ1
    if(irq1)
    spi_w(0x04, irq1);          // IRQ1[7]-
                                //     [6]RX_DR,   write 1 to clear bit
                                //     [5]TX_DS,   write 1 to clear bit
                                //     [4]MAX_RT,  write 1 to clear bit
                                //     [3]ADDRESS, write 1 to clear bit
                                //     [2]BCMATCH, write 1 to clear bit
                                //     [1]RX_TO,   write 1 to clear bit
                                //     [0]-
//uart_puts("irq="); uart_putu8(irq1); uart_putchar_n('\n');
    return irq1;
}
/**
 *  @ingroup SPI_spiTxQ_label
 */
/*
static void movedata_spiTxQ_to_print2sscom(void)
{
    unsigned int i;
    uart_puts("TXDLEN=");
    uart_putu8(spiTxQ.total_length);
//  uart_putchar_n('\n');
    uart_puts(", TX FIFO[]= ");
    for(i=0;i<spiTxQ.total_length;i++) {
        uart_putu8(spiTxQ.payload[i]);
        uart_puts(" ");
    }
    uart_putchar_n('\n');
    uart_putchar_n('\n');
    uart_putchar_n('\n');
}*/
void movedata_spiTxQ_to_ptx_txfifo(unsigned char strobeCmd)
{
        if     (strobeCmd==0x11) {
            spi_cmdd_11_txfifo_write(spiTxQ.total_length, spiTxQ.payload+0);
        }
        else if(strobeCmd==0x13) {
            spi_cmdd_13_txfifo_write_no_ack(spiTxQ.total_length, spiTxQ.payload+0);
        }
            spiTxQ.status = SPITXQ_STATUS_WAITING4_TX_COMPLETE;
}
static void movedata_spiTxQ_to_prx_txfifo(unsigned char pipe)
{
            spi_cmdd_18_txfifo_prx_w_ack_payload(pipe, spiTxQ.total_length, spiTxQ.payload+0);
            spiTxQ.status = SPITXQ_STATUS_WAITING4_TX_COMPLETE;
}
/**
 *  @ingroup BC3601_Host_FSM1_public_by_Holtek_label
 */
void movedata_to_spiTxQ(unsigned char pipe_no, unsigned char total_length, unsigned char *pch)
{
    unsigned char i;
        spiTxQ.remote_pipe_no = pipe_no;
        spiTxQ.total_length   = total_length;
    for(i=0; i<total_length; i++) {
        spiTxQ.payload[i] = *pch;    pch++;
    }
        
        spiTxQ.status = SPITXQ_STATUS_LOADED_BUT_NOT_YET_PROCESSED_BY_HFSM1;
}

#if   _FRONTEND_V_ == 23 // 5602 v3 fullmask
static void init_setting_5602v3_FCF(void)    // Fxtal=16MHz
{
//bank1
    spi_cmd0_20_set_register_bank(1);
  #if USE_IF_FREQ_K == 600
    spi_w(0x24, 0x03);              //FCF1
                                    //        [7:4]MOD_INDEX2[3:0]
                                    //        [3:0]MOD_INDEX1[3:0]
//  spi_w(0x25, 0x44);              //FCF2
    spi_w(0x25, 0xCC);//IF 600K     //FCF2
//  spi_w(0x25, 0x00);//IF   1M     //FCF2
                                    //        [7:0]FSCALE[7:0]
//  spi_w(0x26, 0x4C);//IF 600K 16M xtal //FCF3        RX's Fscale(change IF) = IF * 2^20 / (2 * Fxtal) = (600000 * 2^20)/(2*16000000) = 19660.8   19660=0x4CCC
    spi_w(0x26, 0x1C);//IF 600K 32M xtal //FCF3        RX's Fscale(change IF) = IF * 2^20 / (2 * Fxtal) = (600000 * 2^20)/(2*32000000) =  9830.4    9830=0x2666   1CCC
//  spi_w(0x26, 0x36);//IF   1M 32M xtal //FCF3        RX's Fscale(change IF) = IF * 2^20 / (2 * Fxtal) = (600000 * 2^20)/(2*32000000) =  9830.4    9830=0x2666   1CCC
                                    //        [7]-
                                    //        [6:0]FSCALE[14:8]
    spi_w(0x27, 0x80);//IF 600K     //FCF4
                                    //        [7]RXFD_EN
                                    //        [6]TXFD_EN
                                    //        [5:0]-
  #elif USE_IF_FREQ_K == 1000
    spi_w(0x24, 0x03);              //FCF1
                                    //        [7:4]MOD_INDEX2[3:0]
                                    //        [3:0]MOD_INDEX1[3:0]
//  spi_w(0x25, 0x44);              //FCF2
//  spi_w(0x25, 0xCC);//IF 600K     //FCF2
    spi_w(0x25, 0x00);//IF   1M     //FCF2
                                    //        [7:0]FSCALE[7:0]
//  spi_w(0x26, 0x4C);//IF 600K 16M xtal //FCF3        RX's Fscale(change IF) = IF * 2^20 / (2 * Fxtal) = (600000 * 2^20)/(2*16000000) = 19660.8   19660=0x4CCC
//  spi_w(0x26, 0x1C);//IF 600K 32M xtal //FCF3        RX's Fscale(change IF) = IF * 2^20 / (2 * Fxtal) = (600000 * 2^20)/(2*32000000) =  9830.4    9830=0x2666   1CCC
    spi_w(0x26, 0x36);//IF   1M 32M xtal //FCF3        RX's Fscale(change IF) = IF * 2^20 / (2 * Fxtal) = (600000 * 2^20)/(2*32000000) =  9830.4    9830=0x2666   1CCC
                                    //        [7]-
                                    //        [6:0]FSCALE[14:8]
    spi_w(0x27, 0x80);//IF 600K     //FCF4
                                    //        [7]RXFD_EN
                                    //        [6]TXFD_EN
                                    //        [5:0]-
  #endif
}
#endif


#if 1
static void __read_fpga_all_ctrl_registers(void)
{
    unsigned char addr, value;
//GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_0);
//GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_1);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_1);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_1);
//GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_2);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_2);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_2);
//GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_3);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_3);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_3);
//GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_4);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_4);    GPIO_ToggleOutBits(HT_GPIOE, GPIO_PIN_4);
    for(addr=0x00; addr<=0x1F; addr++)
    {
        value = spi_r(0xC0+addr);
        uart_puts("read reg["); uart_putu8(addr); uart_puts("]= "); uart_putu8(value); uart_puts("   ");
        if(addr==0x00) {uart_puts("CFG1");}
        if(addr==0x01) {uart_puts("RC1");}
        if(addr==0x02) {uart_puts("RC2");}
        if(addr==0x03) {uart_puts("MASK [7]-, [6]MASK_RX, [5]MASK_TX, [4]MASK_MAX_RT, [3]MASK_ADDRESS, [2]MASK_BCMATCH, [1]MASK_RX_TO, [0]PRIM_RX");}
        if(addr==0x04) {uart_puts("IRQ1 STATUS");}
        if(addr==0x05) {uart_puts("IRQ2 FIFO_STATUS read-only");}
        if(addr==0x06) {uart_puts("IO1 [7:6]PADDS,[5:3]GIO2S,[2:0]GIO1S");}
        if(addr==0x07) {uart_puts("IO2 [7:4]GIO4S,[3:0]GIO3S");}
        if(addr==0x08) {uart_puts("IO3 [7]SDO_TEN,[6]SPIPU,[5:0]GIOPU");}
        if(addr==0x09) {uart_puts("PKT1 [5]CRC_EN,[4]CRC8_EN,[3]PLLEN_EN");}
        if(addr==0x0A) {uart_puts("PKT2 [7]WHT_EN,[6:0]WHTSD");}
        if(addr==0x0B) {uart_puts("PKT3 [7]WHT_PCF_EN,[6:0]-");}
        if(addr==0x0C) {uart_puts("PKT4 read-only[7:0]RXDLEN");}
        if(addr==0x0D) {uart_puts("PKT5 [7]-[6:4]DLY_DEMOD, [3]-[2:0]DLY_TXS");}
        if(addr==0x0E) {uart_puts("MOD1 DTR[7:0]");}
        if(addr==0x0F) {uart_puts("MOD2 [7:5]-,[4]DITHER_EN,[3:2]DITHER,[1]DTR_EN,[0]DTR[8]");}
        if(addr==0x10) {uart_puts("RF_CH [7]-[6:0]RF_CH");}
        if(addr==0x11) {uart_puts("DM1 [7:6]AW,[5:3]-,[2]MDIV_SEL,[1:0]SDR");}
        if(addr==0x12) {uart_puts("DM2 [7:0]FD_HOLD");}
        if(addr==0x13) {uart_puts("RT1 [7:4]ARD, [3:0]ARC");}
        if(addr==0x14) {uart_puts("RT2 read-only[7:4]CNT_PLOS, [3:0]CNT_ARC");}
        if(addr==0x15) {uart_puts("CE [0]CE");}
        uart_putchar_n('\n');
    }
uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(0);
    for(addr=0x20; addr<=0x3A; addr++)
    {
        value = spi_r(0xC0+addr);
        uart_puts("bank 0 read reg["); uart_putu8(addr); uart_puts("]= "); uart_putu8(value); uart_puts("   ");
        uart_putchar_n('\n');
    }
uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(1);
    for(addr=0x20; addr<=0x27; addr++)
    {
        value = spi_r(0xC0+addr);
        uart_puts("bank 1 read reg["); uart_putu8(addr); uart_puts("]= "); uart_putu8(value); uart_puts("   ");
        if(addr==0x20) {uart_puts("AGC1");}
        if(addr==0x21) {uart_puts("AGC2");}
        if(addr==0x22) {uart_puts("AGC3");}
        if(addr==0x23) {uart_puts("AGC4");}
        if(addr==0x24) {uart_puts("FCF1");}
        if(addr==0x25) {uart_puts("FCF2");}
        if(addr==0x26) {uart_puts("FCF3");}
        if(addr==0x27) {uart_puts("FCF4");}
        uart_putchar_n('\n');
    }
uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(2);
    for(addr=0x20; addr<=0x3F; addr++)
    {
        value = spi_r(0xC0+addr);
        uart_puts("bank 2 read reg["); uart_putu8(addr); uart_puts("]= "); uart_putu8(value); uart_puts("   ");
        uart_putchar_n('\n');
    }
uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(3);
    for(addr=0x30; addr<=0x36; addr++)
    {
        value = spi_r(0xC0+addr);
        uart_puts("bank 3 read reg["); uart_putu8(addr); uart_puts("]= "); uart_putu8(value); uart_puts("   ");
        uart_putchar_n('\n');
    }
}
#endif
#if 1
static void check_ctrl_reg(unsigned char addr, unsigned char mask)
{
    unsigned char value, read[2];
    spi_cmd0_20_set_register_bank(2);
    for(value=0x00; value<=0xFE; value++) {
        spi_w(0x40|addr,value);
tmr50usGetCurrentTicks();//delay
        read[0] = spi_r(0xC0|addr);
        if( (value&mask)!=(read[0]&mask) ) {
            uart_puts("addr="); uart_putu8(addr); uart_puts("   w="); uart_putu8(value); uart_puts("   r= "); uart_putu8(read[0]); uart_putchar_n('\n');
        }
    }
}
static void __check_v1_all_ctrl_registers(void)
{
    unsigned char addr, value;

        check_ctrl_reg(0x00, 0x53);   spi_w(0x40,0x00);  //CFG1
//        check_ctrl_reg(0x01, 0xFF); //RC1
        check_ctrl_reg(0x02, 0x01);   spi_w(0x42,0x00);  //RC2
        check_ctrl_reg(0x03, 0x71);   spi_w(0x43,0x00);  //MASK
//      check_ctrl_reg(0x04, 0xFF); irq1
//RO    check_ctrl_reg(0x05, 0xFF); status
        check_ctrl_reg(0x06, 0xFF);   spi_w(0x46, 0x80|0x00); //IO1
        check_ctrl_reg(0x07, 0xFF);   spi_w(0x47, 0x50);      //IO2
        check_ctrl_reg(0x08, 0xFF);                           //IO3
        check_ctrl_reg(0x09, 0x30);   spi_w(0x49, 0x30);  //PKT1
        check_ctrl_reg(0x0A, 0xFF);   spi_w(0x4A, 0x00);  //[7]WHT_EN,     [6:0]seed
        check_ctrl_reg(0x0B, 0x80);   spi_w(0x4B, 0x00);  //[7]WHT_PCF_EN  [6:0]-
//RO    check_ctrl_reg(0x0C, 0xFF); RXDLEN
        check_ctrl_reg(0x0D, 0x77);   spi_w(0x4D, 0x27);  //PKT5
        check_ctrl_reg(0x0E, 0xFF);   spi_w(0x4E, 0x01);       //MOD1
        check_ctrl_reg(0x0F, 0x1F);   spi_w(0x4F, 0x14|0x00);  //MOD2
        check_ctrl_reg(0x10, 0x7F);   spi_w(0x50, RFCH_FRONTEND);
        check_ctrl_reg(0x11, 0xC7);   spi_w(0x51, dm1_aw|dm1_d_rate);//DM1
        check_ctrl_reg(0x12, 0xFF);   spi_w(0x52, 0x08);              //DM2
        check_ctrl_reg(0x13, 0xFF);   spi_w(0x53, 0xF1);              //RT1
//RO    check_ctrl_reg(0x14, 0xFF); RT2 (08_OBSERVE_TX)(Read-Only)
        check_ctrl_reg(0x15, 0x01);   spi_w(0x55, 0x00);              //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE

uart_putchar_n('\n');uart_putchar_n('\n');  uart_puts("bank 0"); uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(0);
        check_ctrl_reg(0x20, 0x8F);   spi_w(0x60, 0x00);  //OM
        check_ctrl_reg(0x21, 0xCF);   spi_w(0x61, 0x4F);              //CFO1
        check_ctrl_reg(0x25, 0x8F);   spi_w(0x65, 0x80);//SX4
        check_ctrl_reg(0x22, 0x7F);   //SX1
        check_ctrl_reg(0x23, 0xFF);   //SX2
        check_ctrl_reg(0x24, 0xFF);   //SX3
//RO    check_ctrl_reg(0x26, 0xFF); //STA1 (RO read only)
        check_ctrl_reg(0x27, 0x0F);   spi_w(0x67, 0x0A);              //RSSI1
//RO    check_ctrl_reg(0x28, 0xFF); //RSSI2 (RO read only)
//RO    check_ctrl_reg(0x29, 0xFF); //RSSI3 (RO read only)
        check_ctrl_reg(0x2A, 0x3F);   spi_w(0x6A, 0x00);              //DPL1
        check_ctrl_reg(0x2B, 0x07);   spi_w(0x6B, 0x00);              //DPL2
        #if 1
        check_ctrl_reg(0x2C, 0x3F);   spi_w(0x6C, 0x01);              //RXPW0
        check_ctrl_reg(0x2D, 0x3F);   spi_w(0x6D, 0x01);              //RXPW1
        check_ctrl_reg(0x2E, 0x3F);   spi_w(0x6E, 0x01);              //RXPW2
        check_ctrl_reg(0x2F, 0x3F);   spi_w(0x6F, 0x01);              //RXPW3
        check_ctrl_reg(0x30, 0x3F);   spi_w(0x70, 0x01);              //RXPW4
        check_ctrl_reg(0x31, 0x3F);   spi_w(0x71, 0x01);              //RXPW5
        check_ctrl_reg(0x32, 0x3F);   spi_w(0x72, 0x01);              //ENAA
        check_ctrl_reg(0x33, 0xFF);   spi_w(0x73, 0x01);              //P2B0
        check_ctrl_reg(0x34, 0xFF);   spi_w(0x74, 0x01);              //P3B0
        check_ctrl_reg(0x35, 0xFF);   spi_w(0x75, 0x01);              //P4B0
        check_ctrl_reg(0x36, 0xFF);   spi_w(0x76, 0x01);              //P5B0
        check_ctrl_reg(0x37, 0x3F);   spi_w(0x77, 0x01);              //ACTIVE
        #endif
        check_ctrl_reg(0x38, 0xFF);   spi_w(0x78, 0x13);              //XO1
        check_ctrl_reg(0x39, 0x18);   spi_w(0x79, 0x10);              //XO2
        check_ctrl_reg(0x3A, 0xFF);   spi_w(0x7A, 0x01);              //TX2

uart_putchar_n('\n');uart_putchar_n('\n');  uart_puts("bank 1"); uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(1);
        check_ctrl_reg(0x20, 0x1F);   //AGC1
        check_ctrl_reg(0x21, 0x07);   //AGC2
//RO    check_ctrl_reg(0x22, 0xF7);   //AGC3
        check_ctrl_reg(0x23, 0xFF);   //AGC4
        check_ctrl_reg(0x24, 0xFF);   //FCF1
        check_ctrl_reg(0x25, 0xFF);   //FCF2
        check_ctrl_reg(0x26, 0x7F);   //FCF3
        check_ctrl_reg(0x27, 0xC0);   //FCF4

uart_putchar_n('\n');uart_putchar_n('\n');  uart_puts("bank 2"); uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(2);
        check_ctrl_reg(0x20, 0x8F);   spi_w(0x60, 0x00);  //OM
        check_ctrl_reg(0x21, 0xFF);   spi_w(0x61, 0x97);              //RXG
        check_ctrl_reg(0x26, 0xFF);   spi_w(0x66, 0x01);              //CP1
        check_ctrl_reg(0x27, 0xFF);   spi_w(0x67, 0x66);              //CP2
        check_ctrl_reg(0x28, 0xFF);   spi_w(0x68, 0x0B);              //CP3
        check_ctrl_reg(0x29, 0xFF);   spi_w(0x69, 0x13);              //OD1
        check_ctrl_reg(0x2A, 0xFF);   spi_w(0x6A, 0x61);              //OD2
        check_ctrl_reg(0x2B, 0x5F);   spi_w(0x6B, 0x03);              //VC1
        check_ctrl_reg(0x2C, 0x1F);   
        check_ctrl_reg(0x2D, 0x3F);   spi_w(0x6D, 0x16);              //VC3
        check_ctrl_reg(0x2E, 0xFF);   spi_w(0x6E, 0xEF);              //RX1
        check_ctrl_reg(0x2F, 0x7F);   spi_w(0x6F, 0x5C);              //RX2
      //check_ctrl_reg(0x30, 0xFF);   spi_w(0x70, 0x0B);              //RX3
        check_ctrl_reg(0x31, 0xFF);   spi_w(0x71, 0x07);              //RX4
        check_ctrl_reg(0x32, 0x0F);   spi_w(0x72, 0x08);              //RX5
        check_ctrl_reg(0x33, 0x7F);   spi_w(0x73, 0x25);              //TX1
        check_ctrl_reg(0x34, 0xFF);   spi_w(0x74, 0x4F);              //TX2
        check_ctrl_reg(0x35, 0x7F);   spi_w(0x75, 0x23);              //TX3
        check_ctrl_reg(0x36, 0x0F);   spi_w(0x76, 0x02);              //TX4
        check_ctrl_reg(0x37, 0x9F);   
        check_ctrl_reg(0x38, 0x3F);   
        check_ctrl_reg(0x39, 0x1F);   spi_w(0x79, 0x67);              //CA3
        check_ctrl_reg(0x3A, 0xFF);   spi_w(0x7A, 0x57);              //LD1
        check_ctrl_reg(0x3B, 0xFF);   spi_w(0x7B, 0x57);              //LD2
        check_ctrl_reg(0x3C, 0xCF);   spi_w(0x7C, 0x45);              //LD3
        check_ctrl_reg(0x3D, 0xFF);   spi_w(0x7D, 0x13);              //XO1
        check_ctrl_reg(0x3E, 0x18);   spi_w(0x7E, 0x10);              //XO2
        check_ctrl_reg(0x3F, 0x2F);   //RTM1


uart_putchar_n('\n');uart_putchar_n('\n');  uart_puts("bank 3"); uart_putchar_n('\n');
    spi_cmd0_20_set_register_bank(3);
}
#endif

static void test_read_chip_version(void)
{
    unsigned char tmp[4];
    spi_cmdd_9F_read_chip_version(tmp+0);
    
    uart_puts("chip version= ");
    uart_putu8(tmp[0]);
    uart_putu8(tmp[1]);
    uart_putu8(tmp[2]);
    uart_putchar_n('\n');
}

static void init_syncID(void)
{
    address5602_TypeDef id;
    // set pipe 0 address
    #if 0
    if     (dm1_aw==0x04) // 3 bytes Address
        id.byte4321 = calc_syncword_16(LAP0_TO_CALC_SYNC); // (3-1) bytes syncword byte4321
    else if(dm1_aw==0x08) // 4 bytes Address
        id.byte4321 = calc_syncword_24(LAP0_TO_CALC_SYNC); // (4-1) bytes syncword byte4321
    else if(dm1_aw==0x0C) // 5 bytes Address
        id.byte4321 = calc_syncword_32(LAP0_TO_CALC_SYNC); // (5-1) bytes syncword byte4321
        id.byte0    = P0_LSBYTE;                           //<<<<<<<<<<<<<<
    syncID_p0.byte4321 = id.byte4321 ;

    uart_puts("pipe 0 BCH addr =");
    uart_putu8(id.byte4321>>24);
    uart_putu8(id.byte4321>>16);
    uart_putu8(id.byte4321>>8);
    uart_putu8(id.byte4321);
    uart_putu8(id.byte0);
    uart_putchar_n('\n');
    #endif
        spi_cmdd_10_write_pipe_0_ID (syncID_p0); //<<<<<<<<<<<
        spi_cmdd_90_read_pipe_0_ID(&id);
    uart_puts("read pipe 0 ID get ");
    uart_putu8(id.byte4321>>24);
    uart_putu8(id.byte4321>>16);
    uart_putu8(id.byte4321>>8);
    uart_putu8(id.byte4321);
    uart_putu8(id.byte0);
    uart_putchar_n('\n');
    // set pipe 1 address
    #if 0
    if     (dm1_aw==0x04) // 3 bytes Address
        id.byte4321 = calc_syncword_16(LAP1_TO_CALC_SYNC); // (3-1) bytes syncword byte4321
    else if(dm1_aw==0x08) // 4 bytes Address
        id.byte4321 = calc_syncword_24(LAP1_TO_CALC_SYNC); // (4-1) bytes syncword byte4321
    else if(dm1_aw==0x0C) // 5 bytes Address
        id.byte4321 = calc_syncword_32(LAP1_TO_CALC_SYNC); // (5-1) bytes syncword byte4321
        id.byte0    = P1_LSBYTE;                           //<<<<<<<<<<<<<<
    syncID_p1.byte4321 = id.byte4321 ;
    syncID_p2.byte4321 = id.byte4321 ;
    syncID_p3.byte4321 = id.byte4321 ;
    syncID_p4.byte4321 = id.byte4321 ;
    syncID_p5.byte4321 = id.byte4321 ;

    uart_puts("pipe 1 BCH addr =");
    uart_putu8(id.byte4321>>24);
    uart_putu8(id.byte4321>>16);
    uart_putu8(id.byte4321>>8);
    uart_putu8(id.byte4321);
    uart_putu8(id.byte0);
    uart_putchar_n('\n');
    #endif
        spi_cmdd_12_write_pipe_1_ID(syncID_p1); //<<<<<<<<<<<
        spi_cmdd_92_read_pipe_1_ID(&id);
    uart_puts("read pipe 1 ID get ");
    uart_putu8(id.byte4321>>24);
    uart_putu8(id.byte4321>>16);
    uart_putu8(id.byte4321>>8);
    uart_putu8(id.byte4321);
    uart_putu8(id.byte0);
    uart_putchar_n('\n');
}

static void init_5603_v4_sz_ic_20230615(void)
{
		
		int i;
		macroCSN_SELECT_FPGA;
    spi_cmd0_0A_deepsleep();
    spi_cmd0_0C_lightsleep();
    spi_cmd0_08_software_reset();
		//spi_w(0x00, 0x00|0x00); 
		spi_w(0x00, 0x00|0x40);              //CFG1,    Direct mode
                                    //[1:0]BANK[1:0] control register Bank select, 00:bank0, 01 : bank1, 10 : bank2, 11 : bank3
                                    //[3:2] -
                                    //[4]  DIR_EN direct mode enable
                                    //[5]  PN9_EN
                                    //[6]  AGC_EN
                                    //[7]  -

//bank 0		
		spi_cmd0_20_set_register_bank(0);
		//delay50 

		spi_w(0x02, 0x5C);              //RC2     
                                    //      v3[4]  PSAVE  Power saving mode control. XCLK's gating clock
                                    //      v3[5]  PSAVE1 Power saving mode control. XCLK's gating clock for RFCTRL
                                    //        [6]- XCLK_GEN		
		spi_w(0x03, 0x00);              //MASK (00_CONFIG) [7]-, [6]MASK_RX, [5]MASK_TX, [4]MASK_MAX_RT, [3]-, [2]-, [1]-, [0]PRIM_RX
                                    //        [6]  MASK_RX
                                    //        [5]  MASK_TX
                                    //        [4]  MASK_MAX_RT
                                    //        [0]  PRIM_RX
		spi_w(0x04, 0x70);              //IRQ1 (07_STATUS) [7]-, [6]RX_DR,   [5]TX_DS,   [4]MAX_RT, [3:1]RX_P_NO [0] TX_FULL(R)
                                    //        [7]  -
                                    //        [6]  RX_DR
                                    //        [5]  TX_DS
                                    //        [4]  MAX_RT
  //spi_w(0x05, 0x__);              //IRQ2 (17_FIFO_STATUS) (RO read only)
                                    //        [7]  -
                                    //        [6]  -        todo:TX_REUSE
                                    //        [5]  TX_FULL
                                    //        [4]  TX_EMPTY
                                    //        [3:2]-
                                    //        [1]  RX_FULL
                                    //        [0]  RX_EMPTY
																		
    spi_w(0x06, 0x80|0x28|0x00);         //IO1     [5:3]GIO2S[2:0]=0h:-     , [2:0]GIO1S[2:0]=0h:-      , [7:6]PADDS[1:0] PAD drive strength:0:0.5mA,1:1mA,2:5mA,3:10mA
    
  #if   _SPI_WIRES_ == 3
    spi_w(0x07, 0x00);              //IO2     [7:4]GIO4S[3:0]=5h:IRQ   , [3:0]GIO3S[3:0]=0h:-
  #elif _SPI_WIRES_ == 4
    //spi_w(0x07, 0x15);              //IO2     [7:4]GIO4S[3:0]=5h:IRQ   , [3:0]GIO3S[3:0]=0h:-
		spi_w(0x07, 0x10);
  #endif
		  
    spi_w(0x08, 0x7F);            //IO3     [7]  SDO_TEN
                                    //        [6]  SPIPU
                                    //        [5:0]GIOPU
		spi_w(0x09, PKT1_CRCEN| PKT1_CRC8EN | DM1_AW);              //PKT1 
                                    //        [7:6]-
                                    //        [5]  CRC_EN   (00_CONFIG.3)
                                    //        [4]  CRC8_EN  (00_CONFIG.2)
                                    //        [1:0]AW- Sync words 01:3 Bytes  10:4 Bytes. 11:5 Bytes.

																		
    spi_w(0x0A, PKT2_WHT_EN|PKT2_WHT_SEED);              //PKT2    [7]  WHT_EN: Data whitening enable, [6:0]WHTSD[6:0]:Data whitening seed
		spi_w(0x0B, PKT3_WHT_PCF_EN|0x03);   //PKT3    [7]  WHT_PCF_EN  
																		//	 [5]- SKIP_PRE
																		//        [6:3]-
																		// 				[2:0]DLY_CONTI
    //spi_w(0x0C, 0x__);            //PKY4 [R]RXDLEN 

			spi_w(0x0D, 0x2C);            //PKT5    [7]-
                                    //        [6:4]DLY_RXS             0:4us, 1: 8us, 2:12us, 3:16us, 4:20us, 5:32us, 6: 64us, 7:100us
                                    //        [3:0]DLY_TXS            0: illegal, 1: 5us, 2: 10us, 3: 15us, 4: 20us, 5: 25us, 6: 30us, 7: 35us 8: 40us, 9: 50us, 10: 60us, 11: 70us, 12: 80us, 13: 90us, 14: 100us, 15: 120us
			spi_w(0x0E, 0x01);            //MOD1    DTR[7:0]    DTR[8:0]=0x001, Data Rate = Fxtal /32/(DTR+1) = 16000000 / (32*2) = 250K
			spi_w(0x0F, 0x1C);            //MOD2    [7:5]-      DTR_EN=0: then use setting DM1[2:0] MDIV_SEL,SDR
			                              //        [4]  DITHER_EN
                                    //        [3:2]DITHER
                                    //        [1]  DTR_EN
                                    //        [0]  DTR[8]
		  spi_w(0x10, RFCH_FRONTEND);       //RF_CH (05_RF_CH)
                                    //        [7]  -
                                    //        [6:0]RF_CH		
			spi_w(0x11, dm1_mdiv_sel|dm1_mdiv_sdr|dm1_d_rate);//DM1
                                    //        [7:6]MDIV_SEL
                                    //        [5:4]SDR
                                    //        [3] 
                                    //        [2:0]D_RATE      
																		
		#if   DATARATE == 250
			spi_w(0x12, 0x20);          //DM2     [7:0]FD_HOLD  
		#elif DATARATE == 1000
			spi_w(0x12, 0x20);          //DM2     [7:0]FD_HOLD  
		#elif DATARATE == 2000
			spi_w(0x12, 0x1E);          //DM2     [7:0]FD_HOLD  
		#endif
  
			spi_w(0x13, 0x10);            //RT1 (04_SETUP_RETR)
                                    //        [7:4]ARD: auto retransmit delay
                                    //        [3:0]ARC: auto retransmit count			
      spi_w(0x14, 0x00);           //RT2 (08_OBSERVE_TX)(Read-Only)			
		 	spi_w(0x15, 0x00);            //CE      [7:1]-, [0]CE		
			spi_w(0x16, 0x1F);            //IO4     [4:0]GIOPU[10:6]		

//bank 0		
		spi_cmd0_20_set_register_bank(0);		
		spi_w(0x21, 0x00|AMBLE2);              //ATR1     
																		//				[6]  AMBLE2
																		//				[0]CRC_PASS [R]
		spi_w(0x27, 0x0A);              //RSSI1    RSSI_CTHD[3:0]			
  //spi_w(0x28, 0x__);              //RSSI2 (RO read only)
  //spi_w(0x29, 0x__);              //RSSI3 (RO read only)	
	
   if(DPL2_EN_DPL==0)
    spi_w(0x2A, 0x00);              //DPL1 (1C_DYNPD)
   else
    spi_w(0x2A, DPL_P5 | DPL_P4 | DPL_P3 | DPL_P2 | DPL_P1 | DPL_P0);    //DPL1 (1C_DYNPD)
                                    //        [7:6]-
                                    //        [5]  DPL_P5
                                    //        [4]  DPL_P4
                                    //        [3]  DPL_P3
                                    //        [2]  DPL_P2
                                    //        [1]  DPL_P1
                                    //        [0]  DPL_P0
	   spi_w(0x2B, DPL2_EN_INV_NOACK|DPL2_EN_DPL|DPL2_EN_ACK_PLD|DPL2_EN_DYN_ACK);  //DPL2 (1D_FEATURE)
																	//        [8]- INV_NOACK: 
                                    //        [7:3]-
                                    //        [2]  EN_DPL    : enable dynamic payload length
                                    //        [1]  EN_ACK_PLD: enable ack_packet with payload
                                    //        [0]  EN_DYN_ACK: enable W_TX_PAYLOAD_NOACK command													
    spi_w(0x2C, 0x01);              //RXPW0 (11_RX_PW_P0)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P0
    spi_w(0x2D, 0x08);              //RXPW1 (12_RX_PW_P1)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P1
    spi_w(0x2E, 0x10);              //RXPW2 (13_RX_PW_P2)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P2
    spi_w(0x2F, 0x18);              //RXPW3 (14_RX_PW_P3)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P3
    spi_w(0x30, 0x1C);              //RXPW4 (15_RX_PW_P4)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P4
    spi_w(0x31, 0x20);              //RXPW5 (16_RX_PW_P5)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P5
    spi_w(0x32, ENAA_P5 | ENAA_P4 | ENAA_P3 | ENAA_P2 | ENAA_P1 | ENAA_P0);		
		spi_w(0x33, 0x72);              //P2B0 (0C_RX_ADDR_P2)
                                    //        [7:0]P2B0
    spi_w(0x34, 0x73);              //P3B0 (0D_RX_ADDR_P3)
                                    //        [7:0]P3B0
    spi_w(0x35, 0x74);              //P4B0 (0E_RX_ADDR_P4)
                                   //        [7:0]P4B0
    spi_w(0x36, 0x75);              //P5B0 (0F_RX_ADDR_P5)
                                    //        [7:0]P5B0
	 #if   _HOST_TESTMODE_ == TRMODE_PTX
    spi_w(0x37, 0x01);              //DM4  (02_EN_RXADDR)
   #elif _HOST_TESTMODE_ == TRMODE_PRX
    spi_w(0x37, P5ACTIVE | P4ACTIVE | P3ACTIVE | P2ACTIVE | P1ACTIVE | P0ACTIVE); //DM4  (02_EN_RXADDR)
   #endif		
     spi_w(0x38, 0x10);              //XO1 duplicated bank2[0x3d]
                                    //        [7:6]XSHIFT
                                    //        [5]  XO_IL
                                    //        [4:0]XO_TRIM
     spi_w(0x39, 0x00);              //XO2 duplicated bank2[0x3e]
                                    //        [7:6]-
																		//				[5]XO_SW
                                    //        [4]  XO_SEL
                                    //        [3]  XODIV2
                                    //        [2:0]-
     spi_w(0x3A, 0xAF);              //TX2 duplicated bank2[0x34]
                                    //        [7:5]CT_VDD_PAD
                                    //        [4:3]CT_PRE_PAD
                                    //        [2:0]CT_DRV_PAD	 
		#if   DATARATE == 250
    spi_w(0x3C, 0x00);              //SAD1     [1:0]CPF_M
		#elif DATARATE == 1000
    spi_w(0x3C, 0x01);             //SAD1     [1:0]CPF_M
		#elif DATARATE == 2000
    spi_w(0x3C, 0x02);              //SAD1     [1:0]CPF_M
		#endif																
		spi_w(0x3D, 0x04);              //AGC5 
                                    //        [3]  MPT_0DB_EN
                                    //        [2:0]LB_OFFSET_SEL											

//bank 1
    spi_cmd0_20_set_register_bank(1);		
		#if   DATARATE == 250
			spi_w(0x20, 0x08); 					  //AGC1		
		#elif DATARATE == 1000
		spi_w(0x20, 0x08);              //AGC1    [1:0]AGC_CMP_THD[1:0]AGC comparison number threshold, 0:continuos AGC until SYNCWORD is detected, 1~3:comparison number thrshold
                                    //        [4:2]OFFSET_SEL[2:0]
                                    //        [6]AGC_TS
																		//				[7]YAVG_FSEL	
		#elif DATARATE == 2000
		spi_w(0x20, 0x08);              //AGC1    [1:0]AGC_CMP_THD[1:0]AGC comparison number threshold, 0:continuos AGC until SYNCWORD is detected, 1~3:comparison number thrshold
                                    //        [4:2]OFFSET_SEL[2:0]
                                    //        [6]AGC_TS
																		//				[7]YAVG_FSEL
		#endif
		#if   DATARATE == 250
			spi_w(0x21, 0x71);              //AGC2
                                    //        [2:0]IFDET_OK_THD[2:0]: IF detection OK threshold
                                    //        [7:3]OFFSET	
		#elif DATARATE == 1000
			spi_w(0x21, 0x74);              //AGC2
		#elif DATARATE == 2000
			spi_w(0x21, 0x74);              //AGC2
		#endif

	  spi_w(0x22, 0x11);              //AGC3(RO read only)
                                    //        [2:0]AGC_ST[2:0]:AGC state machine's state, 0x7:AGC_complete
                                    //        [3]-
                                    //        [7:4]GAIN_SEL[2:0]:gain curve select
		#if   DATARATE == 250
		spi_w(0x23, 0x1A);									//AGC4 GAIN_STB////////////////////
		#elif DATARATE == 1000
		  spi_w(0x23, 0x20);             //AGC4 GAIN_STB /////////////////		
		#elif DATARATE == 2000
			 spi_w(0x23, 0x20);           //AGC4  GAIN_STB////////////////
		#endif		


                                    //        [7:0]GAIN_STB[7:0]
    spi_w(0x24, 0x00);              //FCF1
                                    //        [7:4]MOD_INDEX2[3:0]
                                    //        [3:0]MOD_INDEX1[3:0]
		spi_w(0x25, 0x00);              //FCF2 // Datarate 250k    __Diviation:170k			
		//spi_w(0x25, 0x13);              //FCF2 // Datarate 250k    __Diviation:200k	
                                      //        [7:0]FSCALE[7:0]
																		
		spi_w(0x26, 0x40);  					//// Datarate 250k    __Diviation:170k
		//spi_w(0x26, 0x06);  					//// Datarate 250k    __Diviation:200k 
		spi_w(0x27, 0x00);             //FCF4
                                    //        [7]RXFD
                                    //        [6]-
																		//        [5]FMOD_TS
                                    //        [0]FSCALE[16]		
		spi_w(0x28, 0x42);							//SAR2: 
																		//[7]-
																		//[6]AD_ENC
																		//[5:2]-
																		//[1:0]AD_CLOS
																		
		spi_w(0x29, 0x04);	  					//SAD3:
																		//[7:5]AD_CR
																		//[4]AD_ROF
																		//[3]AD_CLOK
																		//[2:0]AD_CRW		
															
	
				spi_w(0x2B, 0x02);					//for agc DAC_R_OW//////////////////////////////////////
		#if   DATARATE == 250
			spi_w(0x31, 0x04);   				  //DA8   
																		//[7:4]-
																		//[3]TPCDNK_BP
																		//[2]DAC_I_OW
																		//[1]-
																		//[0]DAC_EN						
		#elif DATARATE == 1000
		  	spi_w(0x31, 0x00);   				  //DA8                              
		#elif DATARATE == 2000
				spi_w(0x31, 0x00);   				  //DA8 
		#endif		
				spi_w(0x32, 0x00);						//DA9 [7:0]-DAC_I[7:0]
				spi_w(0x33, 0x02);   				  //DA10
				                              //[1:0]DAC_I[9:8]		
				spi_w(0x35, 0x03);   				  //AGC9 DBFS_OFFSET [2:0]		PDIFF_THDUPH [1:0]	AGC_TS_EN [1:0]	 
		
//bank 2
    spi_cmd0_20_set_register_bank(2);
		#if   DATARATE == 250
		spi_w(0x21, 0x2C);
		//spi_w(0x21, 0x57);              //RXG
                                    //        [1:0]GAIN_LNA[1:0] 00:ultralow gain, 01:low gain, 10:middle gain, 11:high gain
                                    //        [2]  GAIN_MX       0:low gain, 1:high gain
                                    //        [4:3]GAIN_BPF[1:0] 00:12dB, 01:18dB, 10:24dB, 11:24dB
                                    //        [7:5]GAIN_PGA[2:0] 000:0dB, 001:6dB, 010:12dB, 011:18dB, 100:24dB, 101:30dB, 110/111:36dB
		#elif DATARATE == 1000
			//spi_w(0x21, 0x57); 
			spi_w(0x21, 0x2C);
		#elif DATARATE == 2000
			spi_w(0x21, 0x77); 
		#endif	
		//spi_w(0x25, 0x20);//debug       //SX4     [5]  TX_MORE_EN=1
		spi_w(0x26, 0x01);              //CP1
																		//[7:6]CM
																		//[5:3]IB_OFFSET
																		//[2:1]PFD_DLY					
																		//[0]CP_EN
		#if   DATARATE == 250
		spi_w(0x27, 0xAA);              //CP2   v3[3:0]CX_M
                                    //        [7:4]IB_CP_M
		#elif DATARATE == 1000
			spi_w(0x27, 0x66); 
		#elif DATARATE == 2000
			spi_w(0x27, 0x66); 
		#endif

	 #if   DATARATE == 250
			spi_w(0x28, 0xA3);   				  //CP3     [0]  ALPF_EN
			                              //        [2:1]PZ
                                    //        [4:3]P3RD
                                    //        [7:5]DLY_SYN[2:0] 000:16us, 001:20us, 010:24us, 011:28us, 100:32us, 101:36us, 110:40us, 111:100us
		#elif DATARATE == 1000
		  spi_w(0x28, 0xA0);   				  //CP3     [0]  ALPF_EN                          
		#elif DATARATE == 2000
			spi_w(0x28, 0xA1);   				  //CP3     [0]  ALPF_EN
		#endif																

																		

		spi_w(0x29, 0x93);              //OD1     [5]  RESET_DSM: Reset DSM modulator 

		//spi_w(0x2A, 0x11);	

		spi_w(0x2A, 0x11);			        //pierce XO_11 //OD2     [7:6]DLY_SXPD   00:55us, 01:70us, 10:90us, 11:127us
    spi_w(0x2B, 0x13);              //VC1
		//spi_w(0x2C, 0x26);              //VC2
		spi_w(0x2D, 0x18);				       //VC3     [3:1]CT_VTMID=100  5602 v3 full mask (2019sept6 Jacky)
		spi_w(0x2E, 0xEF);              //RX1
    spi_w(0x2F, 0x56);              //RX2
    spi_w(0x30, 0x09);              //RX3
		spi_w(0x31, 0x21);              //RX4
		spi_w(0x32, 0x08);              //RX5
		spi_w(0x33, 0x25);			       //TX1     [0]  PAD_EN: TX PA driver enable
                                    //        [3:1]RAMP_DN   000:x  001:30u  010:40u  011:50u  100:65u  101:80u  110:100u  111:120u
                                    //        [6:4]RAMP_UP   000:x  001:25u  010:35u  011:45u  100:60u  101:75u  110: 95u  111:115u
                                    //        [7]-
		//spi_w(0x34, 0xFE);// 0dbm       //TX2 duplicated bank0[0x3F]
                                    //        [7:5]CT_VDD_PAD
                                    //        [4:3]CT_PRE_PAD
                                    //        [2:0]CT_DRV_PAD
		//spi_w(0x35, 0x0F);// 0dbm       //TX3     [7]-
                                    //        [6:4]IBP_PAD[2:0]
                                    //        [3:0]IBN_PAD[3:0]
		spi_w(0x34, 0xCF);// 7dbm       //TX2 duplicated bank0[0x3F]
		spi_w(0x35, 0x30);// 7dbm       //TX3     [7]-			
		//spi_w(0x34, 0x7D);// -5dbm       //TX2 duplicated bank0[0x3F]
		//spi_w(0x35, 0x0F);// -5dbm       //TX3     [7]-			
		#if   DATARATE == 250
		spi_w(0x36, 0x02);              //TX4     [7:4]-
                                    //        [3]  PAD_OW
                                    //        [2:0]DLY_PAD, PAD delay. 000:6us  001:20us  010:30us  011:40us  100:55us  101:75us  110:100us  111:128us	
		#elif DATARATE == 1000
			spi_w(0x36, 0x02);   
		#elif DATARATE == 2000
			spi_w(0x36, 0x02);    
		#endif
	
	//	spi_w(0x37, 0x50);              //CA1			
		spi_w(0x38, 0x20);              //CA2				#RC calibration manual setting  22 16
		spi_w(0x39, 0x90);              //CA3  		new[7]  FDCOC_EN
                                    //        [6:5]-
                                    //        [4:2]DLY_DCOCTB: DCOC turbo mode timing (switch RC) 000:0us, 001:2us, 010:4us, 011:5us, 100:6us, 101:7us, 110:8us, 111:10us
                                    //        [1]  DCOCTB_EN: DCOC turbo mode trigger as PWR_SOFT=1
                                    //        [0]  DCOC_ENB: RX DCOC (server loop) disable
		spi_w(0x3A, 0x57);              //LD1: SXLDO_EN[7:6]=01(1.5V),VCOLDO_EN[5:4]=01(1.5V),XOLDO[3:2]=01(1.5V)
		spi_w(0x3B, 0x57);							//LD2: DIG_LDO[7:6]=01(1.2V), IFLDO_EN[5:4]=01(1.5V),ADCLDO_EN[3:2]=01(1.5V)	  57
		spi_w(0x3C, 0x4B);							//pierce XO_4B//LD3: IB_BG[1:0]=2 LNALDO_EN[2:1]=01(1.5V) with Reg34=6F
		#if _5603Board_ == 28		
			spi_w(0x3D, 0x3A);							//XO1
		#elif _5603Board_ == 36
			spi_w(0x3D, 0x3F);							//XO1
		#endif

		spi_w(0x3E, 0x02); 					  //XO2
                                    //        [7:5]-
                                    //        [4]  XO_SEL
                                    //        [3]  XODIV2
                                    //        [2:0]-
		//spi_w(0x3F, 0x00);							//RTM1
			
//bank 3
		spi_cmd0_20_set_register_bank(3);			
    spi_w(0x2C, 0x01);              //EFU2    [7:3]-
                                    //        [2:0] 
		spi_w(0x2D, 0x00);							//RFT1
		                                //        [6:4]  CPA 
                                    //        [2:0]  CMA 	
																

		spi_w(0x2E, 0x40);							//RFT1
		                                //        [6:4]  IB_PGA 
                                    //        [2:0]  CLA  						
	  #if   DATARATE == 250
			spi_w(0x2F, 0x18);   				  //TEST1				//Divation 170k
		                                //        [7]  DACFD_EN 
                                    //        [6]  MMDFD_EN  
		                                //        [4]  DLY_BPS_EN 
                                    //        [3]  DAC_DLY_EN  
                                    //        [2:0] DAC_DLY			
			//spi_w(0x2F, 0xD8);				//Diviation 200k																		
		#elif DATARATE == 1000
				spi_w(0x2F, 0x0E);  		 //TEST1	
		#elif DATARATE == 2000
			spi_w(0x2F, 0x0A);   				  //TEST1	
		#endif	
		
		//spi_w(0x30, 0x00);						//TXFD   DACFD_EN,   MMDFD_EN, DLY_BPS_EN=1=by pass delay,  DAC_DLY_EN(SEL),  DAC_DLY,  18(default)=No DAC delay,  08=MMD delay max.														
	
		spi_w(0x39, 0x40);   					//DM3 
		spi_w(0x3A, 0x00);						//DM4
                                  //        [0] STEP_EN	
	 #if   DATARATE == 250
			spi_w(0x3B, 0x1F);  					//AGC6 		ABORT TIME	
		#elif DATARATE == 1000
			spi_w(0x3B, 0x1F);					//AGC6      	ABORT TIME 			
		#elif DATARATE == 2000
			spi_w(0x3B, 0x1F);						//AGC6	ABORT TIME	
		#endif

		                              //[4:0]ABORT_TIME 
		spi_w(0x33, 0x80|0x06);         			//TEST5  				
		
/////////////////////////////////  VCO Calibration 

//bank 2
    spi_cmd0_20_set_register_bank(2);
		spi_w(0x20, 0x08);              //OM	ACL_EN
    delay_unit_50us(2);

//////////////////////////////// 2-point
//bank 1
    spi_cmd0_20_set_register_bank(1);
		#if   DATARATE == 250
				spi_w(0x3D, 0x50);   				  //DA20 
			  spi_w(0x3E, 0x5D);						//DA21   
			  spi_w(0x2A, 0x34);  					//DA1    
		#elif DATARATE == 1000		
		  	spi_w(0x3D, 0x57);   				  //DA20   
				spi_w(0x3E, 0x5D);
				spi_w(0x2A, 0x35); 	
				//uart_puts("1M");				
		#elif DATARATE == 2000
				spi_w(0x3D, 0x57);   				  //DA20
				spi_w(0x3E, 0x5D);
				spi_w(0x2A, 0x35);  	
		#endif
	 #if   _HOST_TESTMODE_ == TRMODE_PTX
    		uart_puts("PTX");
   #elif _HOST_TESTMODE_ == TRMODE_PRX
				uart_puts("PRX");
	 #endif
    while( (spi_r(0x2A)&0x01)==0x01 ) {;}	
		//delay_unit_50us(2);
////////////////////////////////		
		
		init_syncID();
//    test_read_chip_version();
			//__read_fpga_all_ctrl_registers();
//bank 0
    spi_cmd0_20_set_register_bank(0);																									
			
}
#if 0
static void init_5602_fifomode(void)
{
    spi_cmd0_0A_deepsleep();
    spi_cmd0_0C_lightsleep();
    spi_cmd0_08_software_reset();
    spi_w(0x00, CFG1_AGC_EN | 0x00);//CFG1
                                    //[1:0]BANK[1:0] control register Bank select, 00:bank0, 01 : bank1, 10 : bank2, 11 : bank3
                                    //[3:2]-
                                    //[4]DIR_EN direct mode enable
                                    //[5]RXCON_EN:RX continuous mode enable
                                    //[6]AGC_EN=0
                                    //[7]-
	spi_w(0x02, 0x5C);              //RC2     [7:6]-, [5]PSAVE1, [4]PSAVE, [3]PSAVE_FIFO, [2]MDCG_EN [1:0]

   #if   _HOST_TESTMODE_ == TRMODE_PTX
    spi_w(0x03, 0x00);              //MASK (00_CONFIG) [7]-, [6]MASK_RX, [5]MASK_TX, [4]MASK_MAX_RT, [3:1]-,           [0]PRIM_RX=0:PTX
   #elif _HOST_TESTMODE_ == TRMODE_PRX
    spi_w(0x43, 0x01);              //MASK (00_CONFIG) [7]-, [6]MASK_RX, [5]MASK_TX, [4]MASK_MAX_RT, [3:1]-,           [0]PRIM_RX=1:PRX
   #else
    spi_w(0x43, 0x00);              //MASK (00_CONFIG) [7]-, [6]MASK_RX, [5]MASK_TX, [4]MASK_MAX_RT, [3:1]-,           [0]PRIM_RX=0:PTX
   #endif
                                    //        [7]  -
                                    //        [6]  MASK_RX
                                    //        [5]  MASK_TX
                                    //        [4]  MASK_MAX_RT
                                    //        [3:2]-         crc_en moved to PKT1[5:4]
                                    //        [1]  -         todo:00_CONFIG.1:PWR_UP
                                    //        [0]  PRIM_RX
    spi_w(0x04, 0x70);              //IRQ1 (07_STATUS) [7]-, [6]RX_DR,   [5]TX_DS,   [4]MAX_RT,      [3:1](RO)RX_P_NO, [0](RO)TX_FULL
                                    //        [7]  -
                                    //        [6]  RX_DR
                                    //        [5]  TX_DS
                                    //        [4]  MAX_RT
                                    //        [3:1](RO)RX_P_NO
                                    //        [0]  (RO)TX_FULL
  //spi_w(0x45, 0x__);              //IRQ2 (17_FIFO_STATUS) (RO read only)
                                    //        [7]  -
                                    //        [6]  -        todo:TX_REUSE
                                    //        [5]  TX_FULL
                                    //        [4]  TX_EMPTY
                                    //        [3:2]-
                                    //        [1]  RX_FULL
                                    //        [0]  RX_EMPTY
    spi_w(0x06, 0x80|0x05);         //IO1     [5:3]GIO2S[2:0]=0h:-     , [2:0]GIO1S[2:0]=0h:-      , [7:6]PADDS[1:0] PAD drive strength:0:0.5mA,1:1mA,2:5mA,3:10mA

#if   _SPI_MODE_ == 0 // software 3-wire SPI
    spi_w(0x07, 0x50);              //IO2     [7:4]GIO4S[3:0]=5h:IRQ   , [3:0]GIO3S[3:0]=0h:-
#elif _SPI_MODE_ == 1 // hardware 4-wire SPI
    spi_w(0x07, 0x10);//4-wire SPI  //IO2     [7:4]GIO4S[3:0]=5h:IRQ   , [3:0]GIO3S[3:0]=1h:SDO 4-wire SPI data
  #endif
  //spi_w(0x48, 0xFF);              //IO3     [7]  SDO_TEN
                                    //        [6]  SPIPU
                                    //        [5:0]GIOPU
		spi_w(0x09, PKT1_CRCEN| PKT1_CRC8EN | DM1_AW);
                                    //        [7:6]-
                                    //        [5]  CRC_EN   (00_CONFIG.3)
                                    //        [4]  CRC8_EN  (00_CONFIG.2)
                                    //        [3]  PLLEN_EN (erased)
                                    //        [2]-
                                    //        [1:0]-	AW 
    spi_w(0x0A, PKT2_WHT_EN|PKT2_WHT_SEED);              //PKT2    [7]  WHT_EN: Data whitening enable, [6:0]WHTSD[6:0]:Data whitening seed
    spi_w(0x0B, PKT3_WHT_PCF_EN|0x23);   //PKT3    [7]  WHT_PCF_EN   
    //spi_w(0x4C, 0x00);              //PKT4 RO (strobe R_RX_PL_WID)
                                    //        [7:0](RO)RXDLEN
    spi_w(0x0D, 0x2C);              //PKT5    [7]-
   // spi_w(0x4D, 0x0C);//2021may11   //PKT5    [7]-
                                    //        [6:4]DLY_RXS             0:4us, 1: 8us, 2:12us, 3:16us, 4:20us, 5:32us, 6: 64us, 7:100us
                                    //        [3]- 
                                    //        [3:0]DLY_TXS             0:40us,1:50us, 2:60us, 3:70us, 4:80us, 5:90us, 6:100us, 7:120us
//    if     (dm1 == 0xA0)   // 250 Kbps
//    spi_w(0x4E, 4-1);               //MOD1    DTR[7:0]    DTR[8:0]=0x003, Data Rate = Fxtal /16/(DTR+1) = 16000000 / (16*4) = 250K
 //   else if(dm1 == 0x51)   // 1   Mbps
 //   spi_w(0x4E, 1-1);               //MOD1    
 //   else if(dm1 == 0x02)   // 2   Mbps
//    spi_w(0x4E, 1-1);               //MOD1    

    spi_w(0x4F, 0x1C|0x00);         //MOD2    [7:5]-
                                    //        [4]  DITHER_EN
                                    //        [3:2]DITHER
                                    //        [1]  DTR_EN     0:use setting DM1[2:0] MDIV_SEL,SDR
                                    //        [0]  DTR[8]=0
    spi_w(0x10, RFCH_FRONTEND);         //RF_CH (05_RF_CH)
                                    //        [7]  -
                                    //        [6:0]RF_CH
    spi_w(0x11, dm1_mdiv_sel|dm1_mdiv_sdr|dm1_d_rate);//DM1
                                    //0xA0, DM1  MDIV_SEL[7:6]=10 & SDR[5:4]=10  datarate D_RATE[1:0]=00: 250Kbps
                                    //0x51, DM1  MDIV_SEL[7:6]=01 & SDR[5:4]=01  datarate D_RATE[1:0]=01: 1Mbps                                   
                                    //0x02, DM1  MDIV_SEL[7:6]=00 & SDR[5:4]=00  datarate D_RATE[1:0]=10: 2Mbps  
													
		

    spi_w(0x12, 0x30);              //DM2     [7:0]FD_HOLD    500K, 400K(cannot be tested)
    spi_w(0x13, 0x03);              //RT1 (04_SETUP_RETR)
                                    //        [7:4]ARD: auto retransmit delay
                                    //        [3:0]ARC: auto retransmit maximum times allowed
  //spi_w(0x54, 0x__);              //RT2 (08_OBSERVE_TX)(Read-Only)
                                    //        [7:4](RO)CNT_PLOS
                                    //        [3:0](RO)CNT_ARC
    spi_w(0x15, 0x00);              //CE      [7:1]-, [0]CE


//bank 0
    spi_cmd0_20_set_register_bank(0);
    //spi_w(0x20, 0x08);              //OM    calibration enable, band:01
                                        //[0]SX_EN: Synthesizer (standby mode) enable
                                        //[1]RTX_SEL: 0:Rx, 1:TX mode
                                        //[2]RTX_EN: RX or TX enable 
                                        //[3]ACAL_EN: Auto calibration enable, execute once the VCO calibration and RC calibration
                                        //                                     execution need clock by setting CE=1 or RC2[4]PSAVE=0
                                        //[6:4]-
                                        //[7]PWR_SOFT
//debug
//  while( (spi_r(0xC0)&0x08)==0x08 ) {;}  // OM[3]ACAL_EN: Auto calibration enable


    spi_w(0x21, 0x00|AMBLE2);               //CFO1    [6]  AMBLE2
    spi_w(0x27, 0x0A);              //RSSI1    RSSI_CTHD[3:0] RSSI threshold for carrier detection. (RSSI_CTHD*2+1)+74 = RSSI threshold for Carrier detection
  //spi_w(0x68, 0x__);              //RSSI2 (RO read only)
  //spi_w(0x69, 0x__);              //RSSI3 (RO read only)
  if(DPL2_EN_DPL==0)
    spi_w(0x2A, 0x00);              //DPL1 (1C_DYNPD)
  else
    spi_w(0x2A, DPL_P5 | DPL_P4 | DPL_P3 | DPL_P2 | DPL_P1 | DPL_P0);    //DPL1 (1C_DYNPD)
                                    //        [7:6]-
                                    //        [5]  DPL_P5
                                    //        [4]  DPL_P4
                                    //        [3]  DPL_P3
                                    //        [2]  DPL_P2
                                    //        [1]  DPL_P1
                                    //        [0]  DPL_P0
    spi_w(0x2B, DPL2_EN_INV_NOACK|DPL2_EN_DPL|DPL2_EN_ACK_PLD|DPL2_EN_DYN_ACK);  //DPL2 (1D_FEATURE)
                                    //      v3[7]  INV_NOACK
                                    //        [6:3]-
                                    //        [2]  EN_DPL    : enable dynamic payload length
                                    //        [1]  EN_ACK_PLD: enable ack_packet with payload
                                    //        [0]  EN_DYN_ACK: enable W_TX_PAYLOAD_NOACK command
    spi_w(0x2C, 0x01);              //RXPW0 (11_RX_PW_P0)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P0
    spi_w(0x2D, 0x02);              //RXPW1 (12_RX_PW_P1)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P1
    spi_w(0x2E, 0x0B);              //RXPW2 (13_RX_PW_P2)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P2
    spi_w(0x2F, 0x16);              //RXPW3 (14_RX_PW_P3)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P3
    spi_w(0x30, 0x1F);              //RXPW4 (15_RX_PW_P4)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P4
    spi_w(0x31, 0x20);              //RXPW5 (16_RX_PW_P5)
                                    //        [7:6]-
                                    //        [5:0]RX_PW_P5
    spi_w(0x32, ENAA_P5 | ENAA_P4 | ENAA_P3 | ENAA_P2 | ENAA_P1 | ENAA_P0);
                                    //ENAA (01_EN_AA)
                                    //        [7:6]-
                                    //        [5]  ENAA_P5
                                    //        [4]  ENAA_P4
                                    //        [3]  ENAA_P3
                                    //        [2]  ENAA_P2
                                    //        [1]  ENAA_P1
                                    //        [0]  ENAA_P0
                                    //____ (10_TX_ADDR)    replaced by strobe command 0x10:write syncword PTX(PTX only use Pipe 0)
                                    //                                                0x91:read  syncword PTX(PTX only use Pipe 0)
                                    //P0__ (0A_RX_ADDR_P0) replaced by strobe command 0x10:write syncword P0
                                    //                                                0x91:read  syncword P0
                                    //P1__ (0B_RX_ADDR_P1) replaced by strobe command 0x12:write syncword P1
                                    //                                                0x92:read  syncword P2
    spi_w(0x33, syncID_p2.byte0);   //P2B0 (0C_RX_ADDR_P2)
                                    //        [7:0]P2B0
    spi_w(0x34, syncID_p3.byte0);   //P3B0 (0D_RX_ADDR_P3)
                                    //        [7:0]P3B0
    spi_w(0x35, syncID_p4.byte0);   //P4B0 (0E_RX_ADDR_P4)
                                    //        [7:0]P4B0
    spi_w(0x36, syncID_p5.byte0);   //P5B0 (0F_RX_ADDR_P5)
                                    //        [7:0]P5B0
   #if   _HOST_TESTMODE_ == TRMODE_PTX
    spi_w(0x37, 0x01);              //PEN  (02_EN_RXADDR)
   #elif _HOST_TESTMODE_ == TRMODE_PRX
    spi_w(0x77, P5ACTIVE | P4ACTIVE | P3ACTIVE | P2ACTIVE | P1ACTIVE | P0ACTIVE); //PEN  (02_EN_RXADDR)
   #endif
                                    //        [7:6]-
                                    //        [5]  P5ACTIVE
                                    //        [4]  P4ACTIVE
                                    //        [3]  P3ACTIVE
                                    //        [2]  P2ACTIVE
                                    //        [1]  P1ACTIVE
                                    //        [0]  P0ACTIVE
    spi_w(0x78, 0x30);              //XO1 duplicated bank2[0x3d]
                                    //        [7:6]XSHIFT
                                    //        [5]  XO_IL
                                    //        [4:0]XO_TRIM
    spi_w(0x79, 0x00);              //XO2 duplicated bank2[0x3e]
                                    //        [7:5]-
                                    //        [4]  XO_SEL
                                    //        [3]  XODIV2
                                    //        [2:0]-
    spi_w(0x3A, 0xAF);//-5dbm       //TX2 duplicated bank2[0x34]    -5dbm:TX2=DF,TX3=2F
  //spi_w(0x7A, 0x63);// 0dbm       //TX2 duplicated bank2[0x34]     0dbm:TX2=63,TX3=21
  //spi_w(0x7A, 0x87);// 5dbm       //TX2 duplicated bank2[0x34]     5dbm:TX2=87,TX3=21
  //spi_w(0x7A, 0xAF);// 7dbm       //TX2 duplicated bank2[0x34]     7dbm:TX2=AF,TX3=21
                                    //        [7:5]CT_VDD_PAD
                                    //        [4:3]CT_PRE_PAD
                                    //        [2:0]CT_DRV_PAD
		spi_w(0x3B, 0x00);															
#if   DATARATE == 250	
		 spi_w(0x3C, 0x00);              //SAD1 [1:0]CPF_M 
#elif DATARATE == 1000
		 spi_w(0x3C, 0x01);              //SAD1 [1:0]CPF_M 
#elif DATARATE == 2000
		 spi_w(0x3C, 0x02);              //SAD1 [1:0]CPF_M 
#endif			
		 spi_w(0x3D, 0x04);              //[3]MPT_0
																		// [2:0]DB_EN	LB_OFFSET_EN
			
//bank 1
    spi_cmd0_20_set_register_bank(1);
    spi_w(0x20, 0x00);              //AGC1    [1:0]AGC_CMP_THD[1:0]AGC comparison number threshold, 0:continuos AGC until SYNCWORD is detected, 1~3:comparison number thrshold
                                    //        [4:2]OFFSET_SEL[2:0]
                                    //        [5]
																		//				[6]AGC_TS
																		//				[7]YAVG_FSEL
  //spi_w(0x61, 0x04);              //AGC2           [2:0]IFDET_OK_THD=4: 4.5us
   spi_w(0x21, 0x74);              //AGC2           [2:0]IFDET_OK_THD=3: 3.5us
                                    //        [2:0]IFDET_OK_THD[2:0]: IF detection OK threshold
                                    //      v3[7:3]OFFSET
  //spi_w(0x62, 0x11);              //AGC3(RO read only)
                                    //        [2:0]AGC_ST[2:0]:AGC state machine's state, 0x7:AGC_complete
                                    //        [3]-
                                    //        [7:4]GAIN_SEL[2:0]:gain curve select
  //spi_w(0x63, 0x30);              //AGC4     ((48*2)-1)/8000000=0.000011875    11.875 us
		spi_w(0x23, 0x10);              //AGC4     ((32*2)-1)/8000000=0.000007875     7.875 us
																		//        [7:0]GAIN_STB[7:0]
    spi_w(0x24, 0x00);              //FCF1
                                    //        [7:4]MOD_INDEX2[3:0]
                                    //        [3:0]MOD_INDEX1[3:0]
    spi_w(0x25, 0x00);              //FCF2
                                    //        [7:0]FSCALE[7:0]
#if   DATARATE == 250				 
		spi_w(0x66, 0x20);              //FCF3
#elif DATARATE == 1000			 
		spi_w(0x26, 0x40);              //FCF3
#elif DATARATE == 2000			 
		spi_w(0x66, 0x80);              //FCF3
#endif
                                    //        [7]-
                                    //        [6:0]FSCALE[14:8]
    spi_w(0x27, 0x80);              //FCF4
                                    //        [7]RXFD_EN
                                    //        [6]TXFD_EN
                                    //        [5:0]-
    spi_w(0x28, 0x42);              //SAD2		[1:0]		AD_CLOS 
    //spi_w(0x29, 0x04);              //			
		#if   DATARATE == 250
			spi_w(0x31, 0x04);   				  //DA8   
																		//[7:4]-
																		//[3]TPCDNK_BP
																		//[2]DAC_I_OW
																		//[1]-
																		//[0]DAC_EN						
		#elif DATARATE == 1000
		  	spi_w(0x31, 0x00);   				  //DA8                              
		#elif DATARATE == 2000
				spi_w(0x31, 0x00);   				  //DA8 
		#endif		
				spi_w(0x32, 0x00);						//DA9 [7:0]-DAC_I[7:0]
				spi_w(0x33, 0x02);   				  //DA10
				                              //[1:0]DAC_I[9:8]	

//bank 2
    spi_cmd0_20_set_register_bank(2);
		#if   DATARATE == 250
		spi_w(0x21, 0x57);              //RXG
                                    //        [1:0]GAIN_LNA[1:0] 00:ultralow gain, 01:low gain, 10:middle gain, 11:high gain
                                    //        [2]  GAIN_MX       0:low gain, 1:high gain
                                    //        [4:3]GAIN_BPF[1:0] 00:12dB, 01:18dB, 10:24dB, 11:24dB
                                    //        [7:5]GAIN_PGA[2:0] 000:0dB, 001:6dB, 010:12dB, 011:18dB, 100:24dB, 101:30dB, 110/111:36dB
		#elif DATARATE == 1000
			spi_w(0x21, 0x57); 
		#elif DATARATE == 2000
			spi_w(0x21, 0x77); 
		#endif	            

    spi_w(0x26, 0x01);              //CP1

		#if   DATARATE == 250
		spi_w(0x27, 0xAA);              //CP2   v3[3:0]CX_M
                                    //        [7:4]IB_CP_M
		#elif DATARATE == 1000
			spi_w(0x27, 0x66); 
		#elif DATARATE == 2000
			spi_w(0x27, 0x66); 
		#endif
		 
	 #if   DATARATE == 250
			spi_w(0x28, 0xA3);   				  //CP3     [0]  ALPF_EN
			                              //        [2:1]PZ
                                    //        [4:3]P3RD
                                    //        [7:5]DLY_SYN[2:0] 000:16us, 001:20us, 010:24us, 011:28us, 100:32us, 101:36us, 110:40us, 111:100us
		#elif DATARATE == 1000
		  spi_w(0x28, 0xA1);   				  //CP3     [0]  ALPF_EN                          
		#elif DATARATE == 2000
			spi_w(0x28, 0xA1);   				  //CP3     [0]  ALPF_EN
		#endif			



    spi_w(0x29, 0x93);              //OD1     [5]  RESET_DSM: Reset DSM modulator
    spi_w(0x2A, 0x7D);              //OD2     [7:6]DLY_SXPD   00:25us, 01:40us, 10:55us, 11:70us
    spi_w(0x2B, 0x13);              //VC1
  //spi_w(0x6C, 0x06);              //VC2
    spi_w(0x2D, 0x18);//5602 v3     //VC3     [3:1]CT_VTMID=100  5602 v3 full mask (2019sept6 Jacky)
    spi_w(0x2E, 0xEF);              //RX1
    spi_w(0x2F, 0x56);              //RX2
    spi_w(0x70, 0x09);              //RX3
    spi_w(0x71, 0x21);              //RX4
    spi_w(0x72, 0x08);              //RX5
    spi_w(0x73, 0x25);//debug       //TX1     [0]  PAD_EN: TX PA driver enable
                                    //        [3:1]RAMP_DN   000:x  001:30u  010:40u  011:50u  100:65u  101:80u  110:100u  111:120u
                                    //        [6:4]RAMP_UP   000:x  001:25u  010:35u  011:45u  100:60u  101:75u  110: 95u  111:115u
                                    //        [7]-
  //spi_w(0x74, 0xB2);//-5dbm       //TX2 duplicated bank0[0x3A]    -5dbm:TX2=DF,TX3=2F
  spi_w(0x74, 0xF6);// 0dbm       //TX2 duplicated bank0[0x3A]     0dbm:TX2=63,TX3=21
  //spi_w(0x74, 0xAE);// 5dbm       //TX2 duplicated bank0[0x3A]     5dbm:TX2=87,TX3=21
  //spi_w(0x74, 0xAF);// 7dbm       //TX2 duplicated bank0[0x3A]     7dbm:TX2=AF,TX3=21
                                    //        [7:5]CT_VDD_PAD
                                    //        [4:3]CT_PRE_PAD
                                    //        [2:0]CT_DRV_PAD
  //spi_w(0x75, 0x0E);//-5dbm       //TX3     [7]-                  -5dbm:TX2=DF,TX3=2F
  spi_w(0x75, 0x0C);// 0dbm       //TX3     [7]-                   0dbm:TX2=63,TX3=21
  //spi_w(0x75, 0x11);// 5dbm       //TX3     [7]-                   5dbm:TX2=87,TX3=21
  //spi_w(0x75, 0x31);// 7dbm       //TX3     [7]-                   7dbm:TX2=AF,TX3=21
                                    //        [6:4]IBP_PAD[2:0]
                                    //        [3:0]IBN_PAD[3:0]
    spi_w(0x76, 0x02);//011:40us    //TX4     [7:4]-
                                    //        [3]  PAD_OW
                                    //        [2:0]DLY_PAD, PAD delay. 000:6us  001:20us  010:30us  011:40us  100:55us  101:75us  110:100us  111:128us
    //spi_w(0x77, 0x40);//calibrate   //CA1
                                    //        [7]  RTXAC_EN: RX/TX auto calibration enables. 0: disable, 1: auto calibration before each TX/RX packet
                                    //        [6]  CHCH_AC_EN:  Channel Change VCO Auto Calibration Enable.
                                    //        [5]-
                                    //        [4:2]DLY_VCOCAL: Delay time for VCO calibration module, 000:1us, 001:2us, 010:3us, 011:4us, 100:5us, 101:6us, 110:7us, 111:20us
                                    //        [1]  CALBUF_EN: 
                                    //        [0]  VCOCAL_EN: VCO calibration module enable
    spi_w(0x78, 0x20);              //CA2
    spi_w(0x79, 0x90);              //CA3     [7:5]-
                                    //        [4:2]DLY_DCOCTB: DCOC turbo mode timing (switch RC) 000:0us, 001:2us, 010:4us, 011:5us, 100:6us, 101:7us, 110:8us, 111:10us
                                    //        [1]  DCOCTB_EN: DCOC turbo mode trigger as PWR_SOFT=1
                                    //        [0]  DCOC_ENB: RX DCOC (server loop) disable
    spi_w(0x7A, 0x57);              //LD1
		spi_w(0x7B, 0x57); //1.3        //LD2     [7:6]CT_DIGLDO  00:1.1V, 01:1.2V, 10:1.3V, 11:1.4V
    spi_w(0x7C, 0x43); // 01        //LD3     [7:6]IB_BG=01
    spi_w(0x7D, 0x2F);              //XO1 duplicated bank0[0x38]
                                    //        [7:6]XSHIFT
                                    //        [5]  XO_IL
                                    //        [4:0]XO_TRIM
    spi_w(0x7E, 0x03);              //XO2 duplicated bank0[0x39]
                                    //        [7:5]-
                                    //        [4]  XO_SEL
                                    //        [3]  XODIV2
                                    //        [2:0]-
    //spi_w(0x7F, 0x01);              //RTM1

//bank 1	
    spi_cmd0_20_set_register_bank(1);
#if   DATARATE == 1000	||	DATARATE == 2000			
		spi_w(0x7D, 0x57);  					//DA20																			
		spi_w(0x7E, 0x1D);  					//DA21	
		spi_w(0x2A, 0x35);  					//DA1	
#elif DATARATE == 250														
		spi_w(0x6A, 0x34);  					//DA1	
#endif		

	while(spi_r(0x2A) & 0x01);
#if   DATARATE == 250																	
		spi_w(0x71, 0x04);              //DA8																		
		spi_w(0x72, 0x00);              //DA9		
#elif	DATARATE == 1000	||	DATARATE == 2000										
		spi_w(0x71, 0x00);              //DA8																		
		spi_w(0x72, 0x00);              //DA9		
#endif															
		spi_w(0x73, 0x02);              //DA10	

#if   DATARATE == 1000	||	DATARATE == 2000																				
		spi_w(0x7D, 0x57);  					//DA20																			
		spi_w(0x7E, 0x1D);  					//DA21	
#elif DATARATE == 250																														
		spi_w(0x7D, 0x50);  					//DA20																			
		spi_w(0x7E, 0x5D);  					//DA21		
#endif		

//bank 3
    spi_cmd0_20_set_register_bank(3);
		spi_w(0x6C, 0x01); 						//
		spi_w(0x6D, 0x00); 						//RFT1
		spi_w(0x6E, 0x40); 						//RFT2 
#if   DATARATE == 250	
		spi_w(0x6F, 0x18);  					//TEST1
#elif DATARATE == 1000
		spi_w(0x6F, 0x08);  					//TEST1
#elif DATARATE == 2000
		spi_w(0x6F, 0x0A);  					//TEST1
#endif
  spi_w(0x73, 0x80|0x06);         //TEST5  TXPKT           {1'b1, txp_state[2:0](out), TXD_EN(out), TXD(out))}


#if   DATARATE == 250	
    spi_w(0x79, 0x20);              //DM3     [7:0]PHASE_DSTEP	
#elif DATARATE == 1000
    spi_w(0x79, 0x40);              //DM3     [7:0]PHASE_DSTEP	
#elif DATARATE == 2000
    spi_w(0x79, 0x80);              //DM3     [7:0]PHASE_DSTEP	
#endif
    spi_w(0x7A, 0x01);              //DM4     [0]STEP_EN
    spi_w(0x7B, 0x00);              //AGC5     [4:0]ABORT_TIME 
    init_syncID();
//bank 2
    spi_cmd0_20_set_register_bank(2);
}
#endif
#define ST_0                           0x0000
/**
 *  @ingroup BC3601_Host_FSM1_private_by_Holtek_label
 */
static void init_mcu(void)
{
    //GPIO for software SPI usage
    init_softspi();

    //GPIO <--- IRQ3
    init_GPIO_IRQ3();
    
    //GPIO debug usage
    set_PE_OUTPUT_MDOE();

    hfsm1.trmode=0;
    hfsm1.state=ST_0;
    spiTxQ.status = SPITXQ_STATUS_EMPTY;
}
/**
 *  @ingroup BC3601_Host_FSM1_private_by_Holtek_label
 */
static void init_5602(void)
{
		write_PE1_SET_1;
		write_PE1_RESET_0;
		write_PE1_SET_1;
		write_PE1_RESET_0;
		write_PE1_SET_1;
		write_PE1_RESET_0;
		macroCSN_SELECT_FPGA;
		init_5603_v4_sz_ic_20230615();
//init_5602_fifomode();
}

////////////////////////////////////////

static void read_rt2_upon_txds(void)
{
    unsigned char rt1, rt2;
    rt1 = spi_r(0x08);         // RT1
    rt2 = spi_r(0x09);         // RT2
//uart_puts("RT1="); uart_putu8(rt1); uart_puts("  TX_DS   RT2="); uart_putu8(rt2); uart_putchar_n('\n');
	
}
static void read_rt2_upon_maxrt(void)
{
    unsigned char rt1, rt2;
    rt1 = spi_r(0x08);         // RT1
    rt2 = spi_r(0x09);         // RT2
uart_puts("RT1="); uart_putu8(rt1); uart_puts("  MAX_RT  RT2="); uart_putu8(rt2); uart_putchar_n('\n');
}

////////////////////////////////////////
#define ST_PTX_SETTING              0x0400
#define ST_PTX_ENTRY_MIDDLE         0x0401
#define ST_PTX_MIDDLE_SLEEP         0x0402
#define ST_PTX_LIGHT_SLEEP          0x0403
#define ST_PTX_W4_TXCOMPLETE        0x0404
#define ST_PTX___________           0x0405
#define ST_PTX_MAXRT_DELAY_BEFORE_NEXT_TRANS 0x0406
#ifdef _CODE_TRMODE_PTX_
/**
 *  @ingroup BC3601_Host_FSM1_public_by_Holtek_label
 */
void entry_hfsm1_ptx(void)
{
    hfsm1.trmode = TRMODE_PTX ;
    hfsm1.state = ST_0;
}
/**
 *  @ingroup BC3601_Host_FSM1_public_by_Holtek_label
 */
void exit_hfsm1_ptx(void)
{
    spi_cmd0_0C_lightsleep();    //exit
    hfsm1.trmode = 0;
    hfsm1.state = ST_0;
}
/**
 *  @ingroup BC3601_Host_FSM1_private_by_Holtek_label
 */
static void change_syncword_ptxPipe0_to_prxPipeYouwant(unsigned char pipe)
{
    if     (pipe==0) {spi_cmdd_10_write_pipe_0_ID(syncID_p0);}
    else if(pipe==1) {spi_cmdd_10_write_pipe_0_ID(syncID_p1);}
    else if(pipe==2) {spi_cmdd_10_write_pipe_0_ID(syncID_p2);}
    else if(pipe==3) {spi_cmdd_10_write_pipe_0_ID(syncID_p3);}
    else if(pipe==4) {spi_cmdd_10_write_pipe_0_ID(syncID_p4);}
    else if(pipe==5) {spi_cmdd_10_write_pipe_0_ID(syncID_p5);}
}
 static void change_amble2(void)  //preamble length 1 or 2 bytes
{
    static unsigned char zzz;
//bank 0
    spi_cmd0_20_set_register_bank(0);
    if((zzz++)&0x01)
    spi_w(0x61, 0x40);              //CFO1    [6]  AMBLE2
    else
    spi_w(0x61, 0x00);              //CFO1    [6]  AMBLE2
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
    if (	//expected_length_from_prx != len  ||
                isCorrectPayload != TRUE )
    {
        uart_puts("ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "); uart_putchar_n('\n');uart_putchar_n('\n');uart_putchar_n('\n');
    }
    expected_length_from_prx = len+1;
    if(expected_length_from_prx>32) {expected_length_from_prx=1;}
    expected_payload_0_from_prx = pPayload[0]+1;
}
#define TX_MIDSLEEP     0
#define TX_METHOD_CE    0  // 0: CE=0
                           // 1: CE=1
static void host5602_fsm1_ptx(void)
{
    unsigned char r_irq1, r_irq2;
    switch(hfsm1.state)
    {
    case ST_PTX_SETTING:
                spi_w(0x03, 0x80|0x00);             //MASK CONFIG [7]-, [6]MASK_RX, [5]MASK_TX, [4]MASK_MAX_RT, [3]MASK_ADDRESS, [2]MASK_BCMATCH, [1]MASK_RX_TO, [0]PRIM_RX=0:PTX
                spi_clear_irq1();
                spi_w(0x15, 0x00);                  //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE
                spi_cmd0_20_set_register_bank(0);
                #if 1
                spi_w(0x37, 0x01);                  //DM4  (P0ACTIVE)
                #else
                spi_w(0x37, P5ACTIVE | P4ACTIVE | P3ACTIVE | P2ACTIVE | P1ACTIVE | P0ACTIVE); //DM4  (02_EN_RXADDR)
                #endif
               #if TX_MIDSLEEP == 1
                hfsm1.state = ST_PTX_ENTRY_MIDDLE;
               #else
                hfsm1.state = ST_PTX_LIGHT_SLEEP;
               #endif
        break;
    case ST_PTX_ENTRY_MIDDLE:
                spi_w(0x15, 0x00);                  //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE
                spi_cmd0_0F_middlesleep();
                hfsm1.delay_offset = tmr50usGetCurrentTicks();
                hfsm1.state = ST_PTX_MIDDLE_SLEEP;
spi_cmd0_20_set_register_bank(0);
while( (spi_r(0x26)&0x07)!=0x01 ) {;}
spi_read_sta1_omst();
        break;
    case ST_PTX_MIDDLE_SLEEP:
                if(tmr50usIsExpired(hfsm1.delay_offset,  MIDDLE_SLEEP_TIME_UNIT_50US))
                {
                    spi_cmd0_0C_lightsleep();
spi_cmd0_20_set_register_bank(0);
//while( (spi_r(0x26)&0x07)!=0x02 ) {;}
//spi_read_sta1_omst();
                    hfsm1.state = ST_PTX_LIGHT_SLEEP;
                }
        break;
    case ST_PTX_LIGHT_SLEEP:
            if( !(spi_read_fifostatus()&0x20) )  // not TX_FULL
            {
                if( !(spi_read_fifostatus()&0x20) ) { // not TX_FULL
                    movedata_to_spiTxQ___ptx(1);
                change_syncword_ptxPipe0_to_prxPipeYouwant(spiTxQ.remote_pipe_no);
								//change_amble2();
								#if TX_NOACK_PKT == 0x11
                    if     (spiTxQ.remote_pipe_no==0) {movedata_spiTxQ_to_ptx_txfifo(0x11);}//0x13: test pipe 0 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==1) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==2) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==3) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==4) {movedata_spiTxQ_to_ptx_txfifo(0x11);}//0x13: test pipe 4 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==5) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
								#else
										if     (spiTxQ.remote_pipe_no==0) {movedata_spiTxQ_to_ptx_txfifo(0x13);}//0x13: test pipe 0 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==1) {movedata_spiTxQ_to_ptx_txfifo(0x13);}
                    else if(spiTxQ.remote_pipe_no==2) {movedata_spiTxQ_to_ptx_txfifo(0x13);}
                    else if(spiTxQ.remote_pipe_no==3) {movedata_spiTxQ_to_ptx_txfifo(0x13);}
                    else if(spiTxQ.remote_pipe_no==4) {movedata_spiTxQ_to_ptx_txfifo(0x13);}//0x13: test pipe 4 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==5) {movedata_spiTxQ_to_ptx_txfifo(0x13);}
								#endif
								}
                #if 0
                if( !(spi_read_fifostatus()&0x20) ) { // not TX_FULL
                    movedata_to_spiTxQ___ptx(0);
                    if     (spiTxQ.remote_pipe_no==0) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 0 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==1) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==2) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==3) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==4) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 4 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==5) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                }
                if( !(spi_read_fifostatus()&0x20) ) { // not TX_FULL
                    movedata_to_spiTxQ___ptx(0);
                    if     (spiTxQ.remote_pipe_no==0) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 0 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==1) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==2) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==3) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==4) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 4 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==5) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                }
                if( !(spi_read_fifostatus()&0x20) ) { // not TX_FULL
                    movedata_to_spiTxQ___ptx(0);
                    if     (spiTxQ.remote_pipe_no==0) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 0 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==1) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==2) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==3) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==4) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 4 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==5) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                }
                if( !(spi_read_fifostatus()&0x20) ) { // not TX_FULL
                    movedata_to_spiTxQ___ptx(0);
                    if     (spiTxQ.remote_pipe_no==0) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 0 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==1) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==2) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==3) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==4) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 4 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==5) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                }
                if( !(spi_read_fifostatus()&0x20) ) { // not TX_FULL
                    movedata_to_spiTxQ___ptx(0);
                    if     (spiTxQ.remote_pipe_no==0) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 0 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==1) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==2) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==3) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                    else if(spiTxQ.remote_pipe_no==4) {movedata_spiTxQ_to_ptx_txfifo(0x11);} // test 0x13: pipe 4 as broadcast no_ack=1
                    else if(spiTxQ.remote_pipe_no==5) {movedata_spiTxQ_to_ptx_txfifo(0x11);}
                }				
								
								#endif
//r_irq2 = spi_read_fifostatus();
//uart_puts("r_irq2="); uart_putu8(r_irq2); uart_puts("  "); uart_putchar_n('\n');
                #if  TX_METHOD_CE == 0 
                spi_w(0x15, 0x00);                  //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE
								//uart_puts("strobe tx");
								if(debug_i < TX_PKT_CNT){
									
                spi_cmd0_0E_TX();
								debug_i++;
									//debug_i=1;
								}
                #elif TX_METHOD_CE == 1 
                spi_w(0x15, 0x01);                  //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE
                //no need to spi_cmd0_0E_TX()
                #endif
                spi_cmd0_20_set_register_bank(0);
                //while( (spi_r(0x26)&0x07)!=0x04 ) {;}       // STA1  [2:0]OMST 0:Deep Sleep Mode, 2:Light Sleep Mode, 4:TX Mode, 5: RX Mode, 6:Calibration mode
                hfsm1.state = ST_PTX_W4_TXCOMPLETE;
            }
        break;
    case ST_PTX_W4_TXCOMPLETE:
            if (GPIO_IRQ3_IS_RESET())
            {
                r_irq1 = spi_read_n_clear_irq1();
												spi_cmd0_20_set_register_bank(0);
												uart_puts("rssi= ");uart_putu8(spi_r(0x29)); //uart_putchar_n('\n');							
								//uart_putu8(r_irq1);
                if( r_irq1 & 0x10 ) {                   //[4]MAX_RT: transaction FAIL
											#if 0
											spi_cmd0_09_txfifo_flush();
											movedata_to_spiTxQ___ptx(1);
											change_syncword_ptxPipe0_to_prxPipeYouwant(spiTxQ.remote_pipe_no);
											movedata_spiTxQ_to_ptx_txfifo(0x11);
											#endif
											
											debug_MAX_RT++;
									uart_puts("RT:");uart_putu16(debug_i);uart_puts("  MAX_RT:");uart_putu16(debug_MAX_RT);uart_puts("  TX_DS:");uart_putu16(debug_TX_DS);uart_puts("  RT2:");uart_putu8(spi_r(0x14));uart_putchar_n('\n');			
//read_rt2_upon_maxrt();

                    #if   TX_METHOD_CE == 0 
                        #if 1 //debug
                        hfsm1.delay_offset = tmr50usGetCurrentTicks();
                        hfsm1.state = ST_PTX_MAXRT_DELAY_BEFORE_NEXT_TRANS;
                        #else
										if(debug_i<TX_PKT_CNT){
											//spi_cmd0_09_txfifo_flush();

                    spi_cmd0_0E_TX();
										debug_i++;}
                        #endif
                    #elif TX_METHOD_CE == 1 
                    //no need to spi_cmd0_0E_TX()
                    #endif
                }
                if( r_irq1 & 0x20 ) {                   //[5]TX_DS: transaction SUCCESSFUL
									debug_TX_DS++;
									uart_puts("RT:");uart_putu16(debug_i);uart_puts("  MAX_RT:");uart_putu16(debug_MAX_RT);uart_puts("  TX_DS:");uart_putu16(debug_TX_DS);uart_putchar_n('\n');			
//read_rt2_upon_txds();

if(spi_read_fifostatus()&0x10) { // [4]TX_EMPTY
                    spiTxQ.status = SPITXQ_TX_COMPLETE;
                    spi_w(0x15, 0x00);                  //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE
                   #if TX_MIDSLEEP == 1
                    hfsm1.state = ST_PTX_ENTRY_MIDDLE;
                   #else
                    hfsm1.delay_offset = tmr50usGetCurrentTicks();
                    hfsm1.state = ST_PTX___________;
                   #endif
}
else {
                    #if   TX_METHOD_CE == 0 
                    if(debug_i<TX_PKT_CNT){
                    spi_cmd0_0E_TX();
										debug_i++;}                   // trigger next transaction
                                                        // the txfifo is still occupied. MAX_RT does not clear txfifo
										
                    #elif TX_METHOD_CE == 1 
                    //no need to spi_cmd0_0E_TX(), 5602 automatically trigger next transaction
                    #endif
}
                }
                if( r_irq1 & 0x40 ) {                   //[6]RX_DR: roger an ack_payload
               #if TX_MIDSLEEP == 0 // midsleep would clear rxfifo content
            //    if   (   spi_read_fifostatus() & 0x02  ) { // RX_FULL    test: wait RX_FULL then read
               #endif
                while( !(spi_read_fifostatus() & 0x01) ) { // RX_EMPTY
                    spi_read_rxfifo_to_spiRxQ();
                    debug_rxpackets_from_prx_not_consecutive(spiRxQ.w_index, spiRxQ.payload+0);
                  //debug_print_rssi();
									uart_puts("zzz_pipe="); uart_putu8(spiTxQ.remote_pipe_no); uart_puts("  "); //uart_putchar_n('\n');
                    callback_spiRxQ_process_at_hfsm2();
                }
               #if TX_MIDSLEEP == 0
            //    }
               #endif
                }
            }
        break;
    case ST_PTX___________:
              //if(tmr50usIsExpired(hfsm1.delay_offset, 20000))  // debug
             // if(tmr50usIsExpired(hfsm1.delay_offset,  2000))  // debug
                if(tmr50usIsExpired(hfsm1.delay_offset,  100))  // debug
              //if(tmr50usIsExpired(hfsm1.delay_offset,   200))  // debug
                {
										//debug_i=1;
                    hfsm1.state = ST_PTX_LIGHT_SLEEP;
                }
            break;
    case ST_PTX_MAXRT_DELAY_BEFORE_NEXT_TRANS:
                if(tmr50usIsExpired(hfsm1.delay_offset, 100))  // debug
              //if(tmr50usIsExpired(hfsm1.delay_offset,  2000))  // debug
              //if(tmr50usIsExpired(hfsm1.delay_offset,   600))  // debug
                {
									/*
                    if(debug_i<1000){
                    spi_cmd0_0E_TX();
										debug_i++;}                   // trigger next transaction
                                                        // the txfifo is still occupied. MAX_RT does not clear txfifo
                    hfsm1.state = ST_PTX_W4_TXCOMPLETE;*/
								     if(debug_i<1000){}
                    //spi_cmd0_0E_TX();
										//debug_i++;}  
										spi_cmd0_09_txfifo_flush();
						
										hfsm1.state = ST_PTX_LIGHT_SLEEP;
                }
            break;
    }
}

#endif


////////////////////////////////////////
#define ST_PRX_SETTING                0x0500
#define ST_PRX_SET_ING                0x0501
#define ST_PRX_W4_RXCOMPLETE          0x0502
#define ST_PRX_TEST_CE_0_AT_ANYTIME   0x0503  // test CE=0 && switch role to ptx
#define ST_PRX_TEST_CE_0_AT_RXDR      0x0504  // test CE=0 && switch role to ptx
#ifdef _CODE_TRMODE_PRX_
/**
 *  @ingroup BC3601_Host_FSM1_public_by_Holtek_label
 */
void entry_hfsm1_prx(void)
{
    hfsm1.trmode = TRMODE_PRX ;
    hfsm1.state = ST_0;
}
/**
 *  @ingroup BC3601_Host_FSM1_public_by_Holtek_label
 */
void exit_hfsm1_prx(void)
{
    spi_cmd0_0C_lightsleep();    //exit
    hfsm1.trmode = 0;
    hfsm1.state = ST_0;
}
/**
 *  @ingroup BC3601_Host_FSM1_private_by_Holtek_label
 */
/*
static void debug_print_rssi(void)
{
    unsigned char rssi;
//bank 0
    spi_cmd0_20_set_register_bank(0);
    uart_puts("STA1=");  rssi=spi_r(0x26); uart_putu8(rssi); uart_puts("  ");  //uart_putchar_n('\n');
    uart_puts("rssi1="); rssi=spi_r(0x27); uart_putu8(rssi); uart_puts("  ");  //uart_putchar_n('\n');
    uart_puts("rssi2="); rssi=spi_r(0x28); uart_putu8(rssi); uart_puts("  ");  //uart_putchar_n('\n');
    uart_puts("rssi3="); rssi=spi_r(0x29); uart_putu8(rssi); uart_puts("  ");  //uart_putchar_n('\n');
    uart_puts("ffidx="); rssi=spi_r(0x2F); uart_putu8(rssi); uart_puts("  ");  //uart_putchar_n('\n');
}*/
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
        uart_puts("ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "); uart_putchar_n('\n');uart_putchar_n('\n');uart_putchar_n('\n');
    }
    expected_length_from_ptx = len+1;
    if(expected_length_from_ptx>32) {expected_length_from_ptx=1;}
    expected_payload_0_from_ptx = pPayload[0]+1;
}
static void prx_process_possible_rxdr_before_roleswitch_to_ptx(void)
{
    unsigned char r_irq1, r_irq2;
            if (GPIO_IRQ3_IS_RESET())
            {
                r_irq1 = spi_read_n_clear_irq1();
                if( r_irq1 & 0x40 ) {                   // [6]RX_DR
                while( !((r_irq2 = spi_read_fifostatus()) & 0x01)) { // RX_EMPTY
                        unsigned char rx_pipe_no;
                        rx_pipe_no = spi_r(0x02);        rx_pipe_no=(rx_pipe_no&0x0E)>>1; // RC2 [3:1]RX_P_NO (RO read only)
//                                                       rx_pipe_no=(    r_irq1&0x0E)>>1;
                    spi_read_rxfifo_to_spiRxQ();
                        debug_rxpackets_from_ptx_not_consecutive(spiRxQ.w_index, spiRxQ.payload+0);
                        uart_puts("___________RX_P_NO=");     uart_putu8(rx_pipe_no);     uart_puts("  ");  //uart_putchar_n('\n');
                    callback_spiRxQ_process_at_hfsm2();
                }
                }
            }
}
static void host5602_fsm1_prx(void)
{
    unsigned char r_irq1, r_irq2;
    static unsigned char  aaaaaa;
		static int debug_i;
    switch(hfsm1.state)
    {
    case ST_PRX_SETTING:
            spi_w(0x15, 0x00);                  //CE      [5:1]-,   [0]CE
            spi_w(0x03, 0x01|0x00);                //MASK CONFIG [7]-, [6]MASK_RX, [5]MASK_TX, [4]MASK_MAX_RT, [3]-, [2-, [1]-, [0]PRIM_RX=1:PRX          
            spi_cmd0_20_set_register_bank(0);
            spi_w(0x37, P5ACTIVE | P4ACTIVE | P3ACTIVE | P2ACTIVE | P1ACTIVE | P0ACTIVE); //DM4  (02_EN_RXADDR)
            spi_cmd0_89_rxfifo_flush();
            spi_clear_irq1();
            hfsm1.state = ST_PRX_SET_ING;
        break;
    case ST_PRX_SET_ING:


								macroCSN_SELECT_FPGA;
                spi_cmd0_20_set_register_bank(0);
								 if (DPL2_EN_ACK_PLD == 0x02)
                {

                    if( !(spi_read_fifostatus()&0x20) )  // not TX_FULL
                    {
                        movedata_to_spiTxQ___prx();
                        movedata_spiTxQ_to_prx_txfifo(spiTxQ.remote_pipe_no);
											
                    }
                }
									
									spi_w(0x15, 0x01);                  //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE
               // while( (spi_r(0x26)&0x07)!=0x05 ) {;}       // STA1  [2:0]OMST 0:Deep Sleep Mode, 2:Light Sleep Mode, 4:TX Mode, 5: RX Mode, 6:Calibration mode
            
						hfsm1.delay_offset = tmr50usGetCurrentTicks();
            hfsm1.state = ST_PRX_W4_RXCOMPLETE;
									
        break;
    case ST_PRX_W4_RXCOMPLETE:
             #if 0
                if( //(spi_read_fifostatus()&0x10) &&  // TX_EMPTY
                    tmr50usIsExpired(hfsm1.delay_offset,  300+ (rand()&0x3FF) ) )
                {
                    spi_w(0x0A, 0x00);                  //CE      [7]DISABLED_TXEN, [6]DISABLED_RXEN,  [5:1]-,   [0]CE
                    hfsm1.delay_offset = tmr50usGetCurrentTicks();
                    hfsm1.state = ST_PRX_TEST_CE_0_AT_ANYTIME;
                    break;
                }
                #endif
		
            if (GPIO_IRQ3_IS_RESET())
            {
							
//								delay_unit_50us(2);
                r_irq1 = spi_read_n_clear_irq1();
							  //delay_unit_50us(2);
//r_irq2=spi_read_fifostatus();
//uart_puts("r_irq2="); uart_putu8(r_irq2); uart_puts(" ");
              //if( r_irq1 & 0x10 ) {                   // [4]MAX_RT, PRX no MAX_RT
              //}
              //if( r_irq1 & 0x20 ) {                   // [5]TX_DS   ack_payload is confirmed by "PID from remote PTX is different"
              //}
                if( r_irq1 & 0x40 ) {                   // [6]RX_DR									
									 debug_i++;
                r_irq2 = spi_read_fifostatus();
								//uart_putu8(r_irq2);
              //  if( r_irq2 & 0x02 ) {                   // RX_FULL    test: wait RX_FULL then read
                 {
                        unsigned char rx_pipe_no;
                      //unsigned char rssi;
                        rx_pipe_no=(r_irq1&0x0E)>>1; // RC2 [3:1]RX_P_NO (RO read only)
												spi_read_rxfifo_to_spiRxQ();
                        debug_rxpackets_from_ptx_not_consecutive(spiRxQ.w_index, spiRxQ.payload+0);
                      //debug_print_rssi();
                      //uart_puts("fifoSTATUS=");  uart_putu8(r_irq2);     uart_puts("  ");  //uart_putchar_n('\n');
												uart_puts("packets_i= ");uart_putu16(debug_i);
                        uart_puts(" RX_P_NO=");     uart_putu8(rx_pipe_no); uart_puts("  ");  //uart_putchar_n('\n');
                    callback_spiRxQ_process_at_hfsm2();
                }// while( ((r_irq2 = spi_read_fifostatus()) & 0x01)==0x00 ); // [0]RX_EMPTY
               // }
								if (DPL2_EN_ACK_PLD == 0x02)
                {
                    if( !(spi_read_fifostatus()&0x20) )  // not TX_FULL
                    {
                        movedata_to_spiTxQ___prx();
                        movedata_spiTxQ_to_prx_txfifo(spiTxQ.remote_pipe_no);	
                    }
                }	
                }
								
            }						
        break;
						
    case ST_PRX_TEST_CE_0_AT_ANYTIME:
                prx_process_possible_rxdr_before_roleswitch_to_ptx();
                if(tmr50usIsExpired(hfsm1.delay_offset,     20))
                {
                    hfsm1.state = ST_PRX_SET_ING;
                }
        break;
    case ST_PRX_TEST_CE_0_AT_RXDR:
                prx_process_possible_rxdr_before_roleswitch_to_ptx();
                if(tmr50usIsExpired(hfsm1.delay_offset,    200))
                {
                    hfsm1.state = ST_PRX_SET_ING;
                }
        break;
    }
}
#endif


/**
 *  @ingroup BC3601_Host_FSM1_private_by_Holtek_label
 */
static void host5602_fsm1(void)
{
    macroCSN_SELECT_FPGA;

    switch(hfsm1.state)
    {
    case ST_0:
        if (hfsm1.trmode == 0) {break;}
        
        if (hfsm1.trmode == TRMODE_PTX) {
uart_puts("5602BLE  PTX   CH="); uart_putu8(RFCH_FRONTEND); uart_putchar_n('\n');
            hfsm1.state = ST_PTX_SETTING;
        }
        if (hfsm1.trmode == TRMODE_PRX) {
uart_puts("5602BLE  PRX   CH="); uart_putu8(RFCH_FRONTEND); uart_putchar_n('\n');
            hfsm1.state = ST_PRX_SETTING;
        }
            break;
    }
    
    #ifdef _CODE_TRMODE_PTX_
    host5602_fsm1_ptx();
    #endif
    #ifdef _CODE_TRMODE_PRX_
    host5602_fsm1_prx();
    #endif
}


////////////////////////////////////////
/**
 *  @ingroup BC3601_Host_FSM1_public_by_Holtek_label
 */
void host5602_process(void)
{
        #if   DATARATE == 250
				dm1_mdiv_sel = 0x80;
        dm1_mdiv_sdr = 0x20;
        #elif DATARATE == 1000
        dm1_mdiv_sel = 0x40;
				dm1_mdiv_sdr = 0x10;
				#elif DATARATE == 2000
				dm1_mdiv_sel = 0x00;
				dm1_mdiv_sdr = 0x00;
        #endif

        #if   DATARATE == 250
        dm1_d_rate = 0x00;
        #elif DATARATE == 1000
        dm1_d_rate = 0x01;
				#elif DATARATE == 2000
				dm1_d_rate = 0x02;
                                                    //0x00, DM1 [2:0]=000: 250 kbps datarate
                                                    //0x01, DM1 [2:0]=001:   1 Mbps datarate
																										//0x02, DM1 [2:0]=010:   2 Mbps datarate																																																				
        #endif
				
        dm1_aw          = DM1_AW ;          //setting @bc5602_host.h
        
        init_mcu();
        init_5602();

        while(1)
        {
            host5602_fsm1();
            host5602_fsm2();
            
        }//while(1)
    
}



