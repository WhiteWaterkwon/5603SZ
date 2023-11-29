/*********************************************************************************************************//**
 * @file    doxydefgroup.h
 * @version $Rev:: 922          $
 * @date    $Date:: 2016-11-02 #$
 * @brief   Documentation tool Doxygen modules/groups definition file.
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




/** @defgroup BC3601_Host_label BC3601 Host
 */
/** @defgroup BC3601_Host_FSM2_by_Customer_label BC3601 Host FSM2 by Customer
 *  @ingroup  BC3601_Host_label
 */
/** @defgroup BC3601_Host_FSM1_public_by_Holtek_label BC3601 Host FSM1 (public) by Holtek
 *  @ingroup  BC3601_Host_label
 */
/** @defgroup BC3601_Host_FSM1_private_by_Holtek_label BC3601 Host FSM1 (private) by Holtek
 *  @ingroup  BC3601_Host_label
 */
/** @defgroup BC3601_BCH_label BCH code to generate syncword
 *  @ingroup  BC3601_Host_label
 */




/** @defgroup HT32_Series_Peripheral_label HT32 Peripheral
 */


/** @defgroup SPI_label SPI
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup SPI_Commands_label SPI strobe commands
 *  @ingroup  SPI_label
 *  @ingroup  BC3601_Host_FSM1_private_by_Holtek_label
 */
/** @defgroup SPI_spiTxQ_label SPI buffer spiTxQ
 *  @ingroup  SPI_label
 *  @ingroup  BC3601_Host_FSM1_public_by_Holtek_label
 */
/** @defgroup SPI_spiRxQ_label SPI buffer spiRxQ
 *  @ingroup  SPI_label
 *  @ingroup  BC3601_Host_FSM1_public_by_Holtek_label
 */



/** @defgroup GPIO_label GPIO
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup Software_SPI_label GPIO based Software SPI
 *  @ingroup  GPIO_label SPI_label
 *  @ingroup  BC3601_Host_FSM1_private_by_Holtek_label
 */
/** @defgroup IRQ3_to_GPIO_label GPIO <-------- 3601 IRQ3
 *  @ingroup  GPIO_label
 *  @ingroup  BC3601_Host_FSM1_private_by_Holtek_label
 */



/** @defgroup BFTM_label BFTM
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup Timer_50us_label Timer unit 50us for Delay
 *  @ingroup  BFTM_label
 *  @ingroup  BC3601_Host_FSM1_private_by_Holtek_label
 */



/** @defgroup USART_label UART
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup UART_PutChar_label UART debug info to sscom32
 *  @ingroup  USART_label
 */


/************************************************************************************************************/



/**
 *  @mainpage BC3601 V4 Host demo program by MCU HT32F1656
 *  - Platform
 *    * Host: MCU HT32F1656.
 *    * BC3601 V4 FPGA.
 *    * RF Front end: BC3601 V3 IC.
 *  - Host <b>FSM2</b> @ bc3601_host2.c by Customer
 *  - Host <b>FSM1</b> @ bc3601_host.c  by Holtek
 *    * Configuration Wizard @ bc3601_host.h
 *      + Select <b>Mode</b> and other settings. One device runs one Mode at a time.
 *  - Modes:
 *    -# FSM2 -- TX Simple fifo (0~64 bytes) (supported by TX Extended fifo)
 *    -# FSM2 -- RX Simple fifo (0~64 bytes) (supported by RX Extended fifo)
 *    -# \ref name_page_fsm2_txExtended    "FSM2 -- TX Extended fifo (0~255 bytes)"
 *    -# \ref name_page_fsm2_rxExtended    "FSM2 -- RX Extended fifo (0~255 bytes)"
 *    -# \ref name_page_fsm2_txInfinite    "FSM2 -- TX Infinite fifo (256~  bytes)"
 *    -# \ref name_page_fsm2_rxInfinite    "FSM2 -- RX Infinite fifo (256~  bytes)"
 *    -# \ref name_page_fsm2_wot           "FSM2 -- WOT"
 *           + WOT without TX_complete. FSM2 check fsm1 spiTxQ.wot.count_wakeup > xx to decide when to exit.
 *    -# \ref name_page_fsm2_wor           "FSM2 -- WOR"
 *    -# \ref name_page_fsm2_autoresend    "FSM2 -- one time   Unidirectional ARK -- AutoResend"
 *           + If fsm1 TX fails, fsm1 retransmit. Thus no TX_fail for fsm2.
 *    -# \ref name_page_fsm2_autoack       "FSM2 -- one time   Unidirectional ARK -- AutoAck"
 *    -# \ref name_page_fsm2_wotAutoresend "FSM2 -- periodical Unidirectional ARK -- WOT+AutoResend"
 *    -# \ref name_page_fsm2_worAutoack    "FSM2 -- periodical Unidirectional ARK -- WOR+AutoAck"
 *    -# \ref name_page_fsm2_master        "FSM2 -- periodical Bidirectional ARK -- Master"
 *           + add bt.c, \#define \_CODE_BT_, can test together with IVT BlueSoleil @ HCI UART 115200 baudrate
 *    -# \ref name_page_fsm2_slave         "FSM2 -- periodical Bidirectional ARK -- Slave"
 *           + add bt.c, \#define \_CODE_BT_, can test together with IVT BlueSoleil @ HCI UART 115200 baudrate
 *  - \ref name_page_BCH "BCH code to generate syncword"
 *
 *  @author Kidd Chen
 *
 *  @date 2017/Jan/23
 *
 */


/**
 *  @page name_page_ppt_atr_align ATR alignment
 *  @section secname_atr_alignment ATR alignment
 *  @image html slide2.png
 *  @image html slide3.png
 *  @image html slide4.png
 *  @image html slide5.png
 *  @image html slide6.png
 *  @section secname_wot_wor_alignment WOT --- WOR alignment
 *  @image html slide7.png
 *  @section secname_wotautoresend_worautoack_alignment WOT+AutoResend --- WOR+AutoAck alignment
 *  @image html slide8.png
 *  @image html slide9.png
 *  @image html slide10.png
 *  @image html slide11.png
 *  @image html slide12.png
 *  @section secname_2way_communications Two Ways Communications By WOT+AutoResend --- WOR+AutoAck
 *  @image html slide13.png
 *  @image html slide14.png
 *  @image html slide15.png
 *  @image html slide16.png
 */

/**
 *  @page name_page_bidir_ark Bidirectional ARK
 *  Bidirectional ARK
 *  @image html msc_2way_01.png
 */


/**
 *  @page name_page_fsm1_master Bidirectional ARK -- Master FSM1
 *
 *  @startuml 
 *  start
 *  repeat
 *  partition "WOT+AutoResend" {
 *  partition "Initial WOT+AutoResend" {
 *  :FIFO1=0x00\nFIFO2=0x00;
 *  :ATR1=0x09 for DR250\n  [0]ATR_EN=1\n  [2:1]=00:ATR WOT\n  [3]ATRCTM=1:continuous mode\n  [4]ATRTU=0:250us,1:1ms\n  [7:6]ATRCLK_DIV= ;
 *  :ATR2=0x00\nATR3=0x03, (ATRCYC=0x0300 for DR250);
 *  :ATR4=0xFF, (ATRRXAP=0xFF);
 *  :ATR5=0xFF\nATR6=0xFF, (ATRRXEP=0xFFFF);
 *  :ATR7=0x07
 *  ATR8=;
 *  :IO1=0x2E\nIRQ2=0xC1;
 *  :spi_cmd0_09_txfifo_flush();
 *  if (spiTxQ.result.w4_tx_complete==1\nPrevious packet not yet ACKed) then (re-send\nprevious)
 *    :Old the same\nTX FIFO\nPID\nTXDLEN;
 *  elseif (NEW packet\nto send ?) then (send\nNEW\npacket)
 *  :movedata_spiTxQ_to_txfifo()
 *  TXDLEN=
 *  PID=;
 *  else (nothing to send\nsend NEW packet with TXDLEN=0)
 *  :TXDLEN=0
 *  PID ++ ;
 *    note right
 *    TXDLEN=0 : dummy packet
 *    PID++ : new packet 
 *    <U+21D2> IRQ3[1] RX_complete at remote Slave
 *    <U+21D2> Slave MCU then initiate AutoResend
 *    end note
 *  endif
 *  :spi_cmd0_0B_idle();
 *  }
 *  partition "Wait for IRQ3[0] TX Complete" {
 *  while (IRQ3[0] TX complete ?) is (not yet)
 *  endwhile (TX completed)
 *  : spi_cmd0_0C_lightsleep() to exit wot+autoresend ;
 *  }
 *  }
 *
 *  partition "AutoACK" {
 *  partition "Initial AutoACK" {
 *  : autoack_setting(1) ;
 *  : autoack_set_rx() ;
 *  }
 *  partition "Wait for IRQ3[1] RX Complete" {
 *  while (IRQ3[1] RX complete ?) is (not yet)
 *  endwhile (RX completed)
 *  : spi_cmdd_BF_rxfifo_read() >
 *  : movedata_spiRxQ_to_UR_TxBuf() ;
 *  : clear_spiRxQ() ;
 *  }
 *  partition "Delay for ACK transmission to remote is finished" {
 *  : hfsm1.delay_offset = tmr50usGetCurrentTicks();
 *  while (Timer expired ?) is (not yet)
 *  endwhile (expired)
 *  : spi_cmd0_0C_lightsleep() to exit autoack ;
 *  }
 *  }
 *  repeat while (to repeat at next ATRCYC period)
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm1_slave Bidirectional ARK -- Slave FSM1
 *
 *  @startuml 
 *  start
 *  partition "ATR Alignment stage" {
 *  partition "Initial WOR+AutoAck" {
 *  :FIFO2=0x00;
 *  :ATR1=0x1B for DR250\n  [0]ATR_EN=1\n  [2:1]=01:ATR WOR\n  [3]ATRCTM=1:continuous mode\n  [4]ATRTU=0:250us,1:1ms\n  [7:6]ATRCLK_DIV= ;
 *  :ATR2=0x00\nATR3=0x03, (ATRCYC=0x0300 for DR250);
 *  :ATR4=0xFF, (ATRRXAP=0xFF);
 *  :ATR5=0xFF\nATR6=0xFF, (ATRRXEP=0xFFFF);
 *  :ATR7=0x07
 *  ATR8=;
 *  :IO1=0x2E\nIRQ1=0x01\nIRQ2=0x12;
 *  :spi_cmd0_0B_idle();
 *  }
 *  partition "Wait for IRQ3[1] RX Complete" {
 *  while (IRQ3[1] RX complete ?) is (not yet)
 *  endwhile (RX completed)
 *  :update_atrct_upon_atr_rxcomplete()
 *  update_atrrxep_after_alignment();
 *  :spi_cmdd_BF_rxfifo_read()
 *  movedata_spiRxQ_to_UR_TxBuf();
 *  }
 *  partition "Delay for ACK transmission to remote is finished" {
 *  : hfsm1.delay_offset = tmr50usGetCurrentTicks();
 *  while (Timer expired ?) is (not yet)
 *  endwhile (expired)
 *  :spi_cmd0_0C_lightsleep() to exit wor+autoack (alignment stage);
 *  }
 *  }
 *
 *  repeat
 *  partition "WOR+AutoAck" {
 *  partition "Initial WOR+AutoAck" {
 *  :FIFO2=0x00;
 *  :ATR1=0x1B for DR250\n  [0]ATR_EN=1\n  [2:1]=01:ATR WOR\n  [3]ATRCTM=1:continuous mode\n  [4]ATRTU=0:250us,1:1ms\n  [7:6]ATRCLK_DIV= ;
 *  :ATR7=0x07;
 *  :IO1=0x2E\nIRQ1=0x01\nIRQ2=0x12;
 *  :spi_cmd0_0B_idle();
 *  }
 *  partition "Wait for IRQ3[1] RX Complete" {
 *  while (IRQ3[1] RX complete ?) is (not yet)
 *  endwhile (RX completed)
 *  :update_atrct_upon_atr_rxcomplete();
 *  :spi_cmdd_BF_rxfifo_read()
 *  movedata_spiRxQ_to_UR_TxBuf();
 *  }
 *  partition "Delay for ACK transmission to remote is finished" {
 *  : hfsm1.delay_offset = tmr50usGetCurrentTicks();
 *  while (Timer expired ?) is (not yet)
 *  endwhile (expired)
 *  :spi_cmd0_0C_lightsleep() to exit wor+autoack;
 *  }
 *  }
 *
 *  partition "AutoResend" {
 *  partition "Initial AutoResend" {
 *  :autoresend_setting(1)
 *  movedata_UR_RxBuf_to_spiTxQ();
 *  :spi_cmd0_09_txfifo_flush();
 *  if (spiTxQ.result.w4_tx_complete==1\nPrevious packet not yet ACKed) then (re-send\nprevious)
 *    :Old the same\nTX FIFO\nPID\nTXDLEN;
 *  elseif (NEW packet\nto send ?) then (send\nNEW\npacket)
 *  :movedata_spiTxQ_to_txfifo()
 *  TXDLEN=
 *  PID=;
 *  else (nothing to send\nsend NEW packet with TXDLEN=0)
 *  :TXDLEN=0
 *  PID no changed ;
 *    note right
 *    TXDLEN=0 : dummy packet
 *    PID no changed
 *    end note
 *  endif
 *  :spi_cmd0_0E_TX();
 *  }
 *  partition "Wait for IRQ3[0] TX Complete" {
 *  while (IRQ3[0] TX complete ?) is (not yet)
 *  endwhile (TX completed)
 *  :spiTxQ.result.w4_tx_complete=0 ;
 *  }
 *  }
 *  repeat while (to repeat at next ATRCYC period)
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_txExtended FSM2 -- TX Extended FIFO @ bc3601_host2.c host3601_fsm2_txExtended()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_txExtended();
 *  }
 *  repeat
 *  while(new packet to TX ?) is (no)
 *  endwhile(to TX)
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___txExtendfifo();
 *  while (spiTxQ.status == SPITXQ_TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_rxExtended FSM2 -- RX Extended FIFO @ bc3601_host2.c host3601_fsm2_rxExtended()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_rxExtended();
 *  }
 *  partition "RX a packet" {
 *    note left
 *       RX a packet
 *       is by call-back function 
 *       callback_spiRxQ_process_at_hfsm2()
 *    end note
 *  }
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_txInfinite FSM2 -- TX Infinite FIFO @ bc3601_host2.c host3601_fsm2_txInfinite()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_txInfinite();
 *  }
 *  repeat
 *  while(new packet to TX ?) is (no)
 *  endwhile(to TX)
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___txInfinitefifo();
 *  while (spiTxQ.status == SPITXQ_TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_rxInfinite FSM2 -- RX Infinite FIFO @ bc3601_host2.c host3601_fsm2_rxInfinite()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_rxInfinite();
 *  }
 *  partition "RX a packet" {
 *    note left
 *       RX a packet
 *       is by call-back function 
 *       callback_spiRxQ_process_at_hfsm2()
 *    end note
 *  }
 *  end
 *  @enduml
 */


/**
 *  @page name_page_fsm2_wot FSM2 -- WOT @ bc3601_host2.c host3601_fsm2_wot()
 *  @startuml 
 *  start
 *  repeat
 *  while(new packet to TX ?) is (no)
 *  endwhile(to TX)
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_wot();
 *    note right
 *     parameter Period must be equal to the Remote WOR's Period
 *     parameter Period in unit milli second
 *         8.2 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *        16.4 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *        41.0 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *       205.0 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *      1024.0 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
 *    end note
 *  }
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___wot();
 *  while (spiTxQ.wot.count_wakeup >= 100 ?) is (not yet)
 *  endwhile (times\nenough)
 *    note right
 *     WOT <b>NO TX_complete</b>.
 *     FSM2 can use the measurement
 *       spiTxQ.wot.count_wakeup to decide when to exit.
 *    end note
 *  }
 *  partition "Exit" {
 *  #Cyan:exit_hfsm1_wot();
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 *
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_wot();
 *  }
 *  repeat
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___wot();
 *    note right
 *     Without <b>Exit</b>, 
 *     consecutive movedata_to_spiTxQ___wot() is okay.
 *    end note
 *  while (spiTxQ.wot.count_wakeup >= 100 ?) is (not yet)
 *  endwhile (times\nenough)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_wor FSM2 -- WOR @ bc3601_host2.c host3601_fsm2_wor()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_wor();
 *    note right
 *     parameter Period must be equal to the Remote WOT's Period
 *     parameter Period in unit milli second
 *         8.2 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *        16.4 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *        41.0 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *       205.0 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *      1024.0 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
 *    end note
 *  }
 *  partition "RX a packet" {
 *    note left
 *       RX a packet
 *       is by call-back function 
 *       callback_spiRxQ_process_at_hfsm2()
 *    end note
 *  }
 *  end
 *  @enduml
 */


/**
 *  @page name_page_fsm2_autoresend FSM2 -- Autoresend @ bc3601_host2.c host3601_fsm2_autoresend()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_autoresend();
 *    note right
 *     parameter ARKNM
 *    end note
 *  }
 *  repeat
 *  while(new packet to TX ?) is (no)
 *  endwhile(to TX)
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___autoresend();
 *  while (spiTxQ.status == SPITXQ_TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_autoack FSM2 -- Autoack @ bc3601_host2.c host3601_fsm2_autoack()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_autoack();
 *    note right
 *     parameter ARKNM
 *    end note
 *  }
 *  partition "RX a packet" {
 *    note left
 *       RX a packet
 *       is by call-back function 
 *       callback_spiRxQ_process_at_hfsm2()
 *    end note
 *  }
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_wotAutoresend FSM2 -- WOT+Autoresend @ bc3601_host2.c host3601_fsm2_wotAutoresend()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_wotAutoresend();
 *    note right
 *     parameter Period must be equal to the Remote worAutoack's Period
 *     parameter Period in unit milli second
 *     parameter ARKNM
 *    end note
 *  }
 *  repeat
 *  while(new packet to TX ?) is (no)
 *  endwhile(to TX)
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___autoresend();
 *  while (spiTxQ.status == SPITXQ_TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_worAutoack FSM2 -- WOR+Autoack @ bc3601_host2.c host3601_fsm2_worAutoack()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_worAutoack();
 *    note right
 *     parameter Period must be equal to the Remote wotAutoresend's Period
 *     parameter Period in unit milli second
 *     parameter ARKNM
 *    end note
 *  }
 *  partition "RX a packet" {
 *    note left
 *       RX a packet
 *       is by call-back function 
 *       callback_spiRxQ_process_at_hfsm2()
 *    end note
 *  }
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_master FSM2 -- Bidirectional ARK -- Master @ bc3601_host2.c host3601_fsm2_master()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_master();
 *    note right
 *     parameter Period must be equal to the Remote Slave's Period
 *     parameter Period in unit milli second
 *        24 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *        43 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *       106 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *       493 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *      2500 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
 *    end note
 *  }
 *
 *  fork 
 *  partition "RX a packet" {
 *    note left
 *       RX a packet
 *       is by call-back function 
 *       callback_spiRxQ_process_at_hfsm2()
 *    end note
 *  }
 *  fork again
 *
 *  repeat
 *  while(new packet to TX ?) is (no)
 *  endwhile(to TX)
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___autoresend();
 *  while (spiTxQ.status == SPITXQ_TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end fork
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_slave FSM2 -- Bidirectional ARK -- Slave @ bc3601_host2.c host3601_fsm2_slave()
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_slave();
 *    note right
 *     parameter Period must be equal to the Remote Master's Period
 *     parameter Period in unit milli second
 *        24 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *        43 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *       106 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *       493 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *      2500 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
 *    end note
 *  }
 *
 *  fork 
 *  partition "RX a packet" {
 *    note left
 *       RX a packet
 *       is by call-back function 
 *       callback_spiRxQ_process_at_hfsm2()
 *    end note
 *  }
 *  fork again
 *
 *  repeat
 *  while(new packet to TX ?) is (no)
 *  endwhile(to TX)
 *  partition "TX a packet" {
 *  #Cyan:movedata_to_spiTxQ___autoresend();
 *  while (spiTxQ.status == SPITXQ_TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end fork
 *  end
 *  @enduml
 */

/**
 *  @page name_page_BCH BCH code to generate syncword
 *  -# bch64_syncword_calc(u32 lap, u8 *pIdcode) to generate syncword <b>8</b> bytes
 *  \verbatim
 *     - BCH(64,30) Barker  7, support 2^(30- 6)= 16,777,216 devices (Bluetooth use BCH_64_SELECT == 1)
 *     - BCH(64,30) Barker 11, support 2^(30-10)=  1,048,576 devices
 *     - BCH(64,30) Barker 13, support 2^(30-12)=    262,144 devices
 *     - BCH(64,24) Barker  7, support 2^(24- 6)=    262,144 devices
 *     - BCH(64,24) Barker 11, support 2^(24-10)=     16,384 devices
 *     - BCH(64,24) Barker 13, support 2^(24-12)=      4,096 devices
 *     - BCH(64,18) Barker  7, support 2^(18- 6)=      4,096 devices
 *     - BCH(64,18) Barker 11, support 2^(18-10)=        256 devices
 *     - BCH(64,18) Barker 13, support 2^(18-12)=         64 devices
 *  \endverbatim
 *  -# bch48_syncword_calc(u32 lap, u8 *pIdcode) to generate syncword <b>6</b> bytes
 *  \verbatim
 *     - BCH(48,24) Barker  7, support 2^(24- 6)=    262,144 devices
 *     - BCH(48,24) Barker 11, support 2^(24-10)=     16,384 devices
 *     - BCH(48,24) Barker 13, support 2^(24-12)=      4,096 devices
 *  \endverbatim
 *  -# bch32_syncword_calc(u32 lap, u8 *pIdcode) to generate syncword <b>4</b> bytes
 *  \verbatim
 *     - BCH(32,16) Barker  7, support 2^(16- 6)=      1,024 devices
 *     - BCH(32,16) Barker 11, support 2^(16-10)=         64 devices
 *     - BCH(32,11) Barker  4, support 2^(11- 3)=        256 devices
 *     - BCH(32,11) Barker  5, support 2^(11- 4)=        128 devices
 *     - BCH(32,11) Barker  7, support 2^(11- 6)=         32 devices
 *  \endverbatim
 *  -# bch16_syncword_calc(u32 lap, u8 *pIdcode) to generate syncword <b>2</b> bytes
 *  \verbatim
 *     - BCH(16,11) Barker  5, support 2^(11- 4)=        128 devices
 *     - BCH(16,11) Barker  7, support 2^(11- 6)=         32 devices
 *     - BCH(16, 7) Barker  4, support 2^( 7- 3)=         16 devices
 *  \endverbatim
 */



