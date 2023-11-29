/*********************************************************************************************************//**
 * @file    doxydefgroup.h
 * @version $Rev:: 922          $
 * @date    $Date:: 2017-10-26 #$
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




/** @defgroup nRF2401_Host_label nRF24L01 Host
 */
/** @defgroup nRF2401_Host_FSM2_by_Customer_label nRF24L01 Host FSM2 by Customer
 *  @ingroup  nRF2401_Host_label
 */
/** @defgroup nRF2401_Host_FSM1_public_by_Holtek_label nRF24L01 Host FSM1 (public) by Holtek
 *  @ingroup  nRF2401_Host_label
 */
/** @defgroup nRF2401_Host_FSM1_private_by_Holtek_label nRF24L01 Host FSM1 (private) by Holtek
 *  @ingroup  nRF2401_Host_label
 */
/** @defgroup nRF2401_BCH_label BCH code to generate syncword
 *  @ingroup  nRF2401_Host_label
 */




/** @defgroup HT32_Series_Peripheral_label HT32 Peripheral
 */


/** @defgroup SPI_label SPI
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup SPI_Commands_label SPI strobe commands
 *  @ingroup  SPI_label
 *  @ingroup  nRF2401_Host_FSM1_private_by_Holtek_label
 */
/** @defgroup SPI_spiTxQ_label SPI buffer spiTxQ
 *  @ingroup  SPI_label
 *  @ingroup  nRF2401_Host_FSM1_public_by_Holtek_label
 */
/** @defgroup SPI_spiRxQ_label SPI buffer spiRxQ
 *  @ingroup  SPI_label
 *  @ingroup  nRF2401_Host_FSM1_public_by_Holtek_label
 */



/** @defgroup GPIO_label GPIO
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup Software_SPI_label GPIO based Software SPI
 *  @ingroup  GPIO_label SPI_label
 *  @ingroup  nRF2401_Host_FSM1_private_by_Holtek_label
 */
/** @defgroup IRQ_to_GPIO_label GPIO <-------- 24L01 IRQ
 *  @ingroup  GPIO_label
 *  @ingroup  nRF2401_Host_FSM1_private_by_Holtek_label
 */



/** @defgroup BFTM_label BFTM
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup Timer_50us_label Timer unit 50us for Delay
 *  @ingroup  BFTM_label
 *  @ingroup  nRF2401_Host_FSM1_private_by_Holtek_label
 */



/** @defgroup USART_label UART
 *  @ingroup  HT32_Series_Peripheral_label
 */
/** @defgroup UART_PutChar_label UART debug info to sscom32
 *  @ingroup  USART_label
 */


/************************************************************************************************************/



/**
 *  @mainpage nRF24L01 V5 Host demo program by MCU HT32F1656
 ***********
 ### Platform ######################
    @par
 *  @image html platform2.png
 
    @par
    <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
    <caption id="tbl_platform"> </caption>
    <tr style="background-color:steelblue;color:white;">
        <th> Parts
        <th> by
        <th> note
       </tr>
    <tr><td> FSM2 @ MCU ht32f1656
        <td> Customer
        <td><ul><li>nrf2401_host2.c
            </ul>
    <tr><td> FSM1 @ MCU ht32f1656
        <td> Holtek
        <td><ul><li>nrf2401_host.c
                <li>nrf2401_host.h with <b>Configuration Wizard</b>\n
                    Select <b>Mode</b> and other settings.\n 
                    One device runs one Mode at a time.
            </ul>
    <tr><td> nRF24L01 V5 IC
        <td> Holtek
        <td><ul><li>
            </ul>
    </table>

 ### FSM2 Modes ######################
 @par
    <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
    <caption id="tbl_modes"> </caption>
    <tr style="background-color:steelblue;color:white;">
        <th> FSM2 Modes
        <th> note
       </tr>
    <tr><td> \ref name_page_fsm2_txsimple      "FSM2 -- TX Simple fifo (0~64 bytes)"
        <td><ul><li>TX site do not know whether the remote RX site roger the packet or not.
            </ul>
    <tr><td> \ref name_page_fsm2_rxsimple      "FSM2 -- RX Simple fifo (0~64 bytes)"
        <td><ul><li>FSM1 will schedule 24L01 to RX mode again, if 24L01 result IRQ3[1]RX complete with no error.\n
                    FSM1 thus give "continous RX" alike service to FSM2.
            </ul>
    <tr><td> \ref name_page_fsm2_txextend      "FSM2 -- TX Extend fifo (0~255 bytes)"
        <td><ul><li>TX site do not know whether the remote RX site roger the packet or not.
            </ul>
    <tr><td> \ref name_page_fsm2_rxextend      "FSM2 -- RX Extend fifo (0~255 bytes)"
        <td><ul><li>FSM1 will schedule 24L01 to RX mode again, if 24L01 result IRQ3[1]RX complete with no error.\n
                    FSM1 thus give "continous RX" alike service to FSM2.
            </ul>
    <tr><td> FSM2 -- Loopback extend fifo, Tester site
        <td>
    <tr><td> FSM2 -- Loopback extend fifo, DUT site
        <td>
    <tr><td> \ref name_page_fsm2_txinfinite    "FSM2 -- TX Infinite fifo (256~ bytes)"
        <td><ul><li>TX site do not know whether the remote RX site roger the packet or not.
            </ul>
    <tr><td> \ref name_page_fsm2_rxinfinite    "FSM2 -- RX Infinite fifo (256~ bytes)"
        <td><ul><li>FSM1 will schedule 24L01 to RX mode again, if 24L01 result IRQ3[1]RX complete with no error.\n
                    FSM1 thus give "continous RX" alike service to FSM2.
            </ul>
    <tr><td> \ref name_page_fsm2_wot           "FSM2 -- periodical WOT"
        <td><ul><li>No result TX_complete would be reported to FSM2.\n 
                    FSM2 can check spiTxQ.wot.count_wakeup > xx to decide when to exit.
            </ul>
    <tr><td> \ref name_page_fsm2_wor           "FSM2 -- periodical WOR"
        <td><ul><li> \ref secname_wot_wor_alignment "Time Aligned to WOT"
            </ul>
    <tr><td> \ref name_page_fsm2_autoresend    "FSM2 -- AutoResend (fsm1 repeat if tx_failure)"
        <td><ul><li>If 24L01 nonperiodical AutoResend report IRQ3[7]ARK_TX_failure to MCU, then 24L01 really exit and return to LightSleep.
                <li><span style="color:fuchsia;">
                    No result TX_failure would be reported to FSM2.\n 
                    \ref secname_msc210_ars_scenario3 "FSM1 will schedule 24L01 to AutoResend again, if 24L01 result IRQ3[7]ARK TX failure."\n
                    FSM1 thus give the same (as WOT+AutoResend) "only TX_complete, no TX_failure" service to FSM2.
                    </span>
                <li>24L01 no TX transmission, if no data packet to send.
            </ul>
    <tr><td> \ref name_page_fsm2_autoack       "FSM2 -- AutoAck"
        <td><ul><li>Although nonperiodical, 24L01 AutoAck always be activated and thus keep receiving packet.
            </ul>
    <tr><td> \ref name_page_fsm2_wotautoresend "FSM2 -- periodical WOT+AutoResend"
        <td><ul><li>If 24L01 periodical WOT+AutoResend report IRQ3[7]ARK_TX_failure to MCU, then 24L01 actually still keep doing WOT+AutoResend.\n
                    There is no FAILURE for 24L01 periodical WOT+AutoResend, because WOT+AutoResend always have a next period.\n
                    IRQ3[7]ARK_TX_failure reporting is just for some reason.
                <li>FSM1 ignore IRQ3[7]ARK_TX_failure from 24L01.
                    FSM1 wait IRQ3[1]TX_complete only.
                <li>No result TX_failure would be reported to FSM2.\n 
                    "periodical" makes 24L01 always have a next chance to resend.\n
                <li>\ref secname_wotars_dummypacket "dummy packets:"\n
                    24L01 must send packet at duty-on time of every periods.\n
                    What if no new packet to send?\n
                    24L01 have to TX dummy packet with payload length 0 and changed TXPID, even if no data packet to send.\n
                    FSM1 handle this by writing dummy packet with length 0 (TXDLEN=0 and changed TXPID), if FSM2 no new data packet to send.
            </ul>
    <tr><td> \ref name_page_fsm2_worautoack    "FSM2 -- periodical WOR+AutoAck"
        <td><ul><li> \ref secname_woraak_alignment "Time Aligned to WOT+AutoResend"
            </ul>
    <tr><td> \ref name_page_fsm2_master        "FSM2 -- periodical bidirectional Master"
        <td><ul><li>No result TX_failure would be reported to FSM2.\n 
                    "periodical" makes 24L01 always have a next chance to resend.
                <li>24L01 still have to periodically TX dummy packet with payload length 0, even if no data packet to send.\n
                    FSM1 handle this by writing dummy packet with length 0 (TXDLEN=0 and changed TXPID), if FSM2 no new data packet to send.
            </ul>
    <tr><td> \ref name_page_fsm2_slave         "FSM2 -- periodical bidirectional Slave"
        <td><ul><li>No result TX_failure would be reported to FSM2.\n 
                    "periodical" makes 24L01 always have a next chance to resend.
                <li>24L01 still have to periodically TX dummy packet with payload length 0, even if no data packet to send.\n
                    FSM1 handle this by writing dummy packet with length 0 (TXDLEN=0 and changed TXPID), if FSM2 no new data packet to send.
            </ul>
    </table>

 ### Others ######################
 @par
    <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
    <caption id="tbl_others"> </caption>
    <tr style="background-color:steelblue;color:white;">
        <th> Topics
        <th> note
       </tr>
    <tr><td> \ref name_page_init "Initial"
        <td><ul><li>
            </ul>
    <tr><td> \ref name_page_channel_freq "Channel Frequency"
        <td><ul><li>
            </ul>
    <tr><td> \ref name_page_BCH "Syncword generated by BCH code"
        <td><ul><li>
            </ul>
    <tr><td> \ref name_page_header_addr "Header Address"
        <td><ul><li>
            </ul>
    <tr><td> \ref name_page_uniark "Unidirectional ARK (ARK: AutoResend & AutoAck)"
        <td><ul><li>
            </ul>
    <tr><td> \ref name_page_biark "Bidirectional ARK"
        <td><ul><li>
            </ul>
    <tr><td> \ref name_page_ppt_atr_align "ATR time alignment"
        <td><ul><li>
            </ul>
    </table>
 *
 *  @author Kidd Chen
 *
 *  @date 2017/Oct/2
 *
 */



/**
 *  @page name_page_fsm2_txsimple FSM2 -- TX Simple fifo
 *  - nrf2401_host2.c\n   host24L01_fsm2_txSimple()
 ***********
 *  <hr>
 *  @section secname_msc21_txsimple MSC
 *  @image html msc_02_simple.png
 ***********
 *  <hr>
 *  @section secname_msc210_txsimple MSC: fsm2 - fsm1 - 24L01
 *  @image html msc_02_simple_210012.png
 ***********
 *  <hr>
 *  @section secname_uml2_txsimple UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_txSimple();
 *  }
 *  repeat
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *  while (spiTxQ.status == TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_rxsimple FSM2 -- RX Simple fifo
 *  - nrf2401_host2.c\n     host24L01_fsm2_rxSimple()
 ***********
 *  <hr>
 *  @section secname_uml2_rxsimple UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_rxSimple();
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
 *  @page name_page_fsm2_txextend FSM2 -- TX Extended fifo
 *  - nrf2401_host2.c\n     host24L01_fsm2_txExtended()
 ***********
 *  <hr>
 *  @section secname_msc21_txextend MSC
 *  @image html msc_04_extend.png
 ***********
 *  <hr>
 *  @section secname_msc210_txextend MSC: fsm2 - fsm1 - 24L01
 *  @image html msc_04_extend_210012.png
 ***********
 *  <hr>
 *  @section secname_uml2_txextend UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_txExtended();
 *  }
 *  repeat
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *  while (spiTxQ.status == TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_rxextend FSM2 -- RX Extended fifo
 *  - nrf2401_host2.c\n     host24L01_fsm2_rxExtended()
 ***********
 *  <hr>
 *  @section secname_uml2_rxextend UML
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
 *  @page name_page_fsm2_loopback_extend FSM2 -- Loopback extended fifo
 *  @section secname_loopbackExtend_tester Loopback extended fifo --- TESTER site
 *  - nrf2401_host2.c\n     host24L01_fsm2_loopbackExtended_tester()
 *  @section secname_loopbackExtend_dut    Loopback extended fifo --- DUT site
 *  - nrf2401_host2.c\n     host24L01_fsm2_loopbackExtended_dut()
 */


/**
 *  @page name_page_fsm2_txinfinite FSM2 -- TX Infinite fifo
 *  - nrf2401_host2.c\n     host24L01_fsm2_txInfinite()
 ***********
 *  <hr>
 *  @section secname_msc21_txinfinite MSC
 *  @image html msc_06_infinite.png
 ***********
 *  <hr>
 *  @section secname_msc210_txinfinite MSC: fsm2 - fsm1 - 24L01
 *  @image html msc_06_infinite_210012.png
 ***********
 *  <hr>
 *  @section secname_uml2_txinfinite UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_txInfinite();
 *  }
 *  repeat
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *  while (spiTxQ.status == TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_rxinfinite FSM2 -- RX Infinite fifo
 *  - nrf2401_host2.c\n     host24L01_fsm2_rxInfinite()
 ***********
 *  <hr>
 *  @section secname_uml2_rxinfinite UML
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
 *  @page name_page_fsm2_wot FSM2 -- WOT
 *  - nrf2401_host2.c\n     host24L01_fsm2_wot()
 ***********
 *  <hr>
 *  @section secname_msc21_wot MSC
 *  @image html msc_08_wot_wor.png
 ***********
 *  <hr>
 *  @section secname_uml2_wot UML
 *  @startuml 
 *  start
 *  repeat
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_wot();
 *    note right
 *     parameter Period must be equal to the Remote WOR's Period
 *     parameter Period in unit milli second
 *         12 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *         21 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *         48 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *        225 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *       1100 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
 *    end note
 *  }
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
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
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *    note right
 *     Without <b>Exit</b>, 
 *     consecutive movedata_to_spiTxQ() is okay.
 *    end note
 *  while (spiTxQ.wot.count_wakeup >= 100 ?) is (not yet)
 *  endwhile (times\nenough)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */

/**
 *  @page name_page_fsm2_wor FSM2 -- WOR
 *  - nrf2401_host2.c\n     host24L01_fsm2_wor()
 ***********
 *  <hr>
 *  @section secname_uml2_wor UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_wor();
 *    note right
 *     parameter Period must be equal to the Remote WOT's Period
 *     parameter Period in unit milli second
 *         12 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *         21 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *         48 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *        225 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *       1100 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
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
 *******
 *  <hr>
 *  @section secname_wor_alignment Time Aligned to WOT
 *      - @ref secname_wot_wor_alignment "WOR time aligned to WOT"
 */


/**
 *  @page name_page_fsm2_autoresend FSM2 -- AutoResend (fsm1 repeat if tx_failure)
 *  - nrf2401_host2.c\n     host24L01_fsm2_autoresend()
 *  - \ref name_page_uniark "Unidirectional ARK (ARK: AutoResend & AutoAck)"
 ***********
 *  <hr>
 *  @section secname_msc21_autoresend      MSC TX_COMPLETE
 *  @image html msc_10_autoresend.png
 ***********
 *  @section secname_msc21_autoresend_fail MSC long time no TX_COMPLETE
 *  @image html msc_10_autoresend_fail.png
 ***********
 *  <hr>
 *  @section secname_msc210_ars_scenario1   MSC(fsm2-fsm1-24L01) ARKNM=0~15, TX_COMPLETE success regular send, no need resend
 *  @image html msc10_ars_arknm0_txcomplete_210012.png
 ***********
 *  <hr>
 *  @section secname_msc210_ars_scenario2   MSC(fsm2-fsm1-24L01) ARKNM=3~15, TX_COMPLETE fail regular send, need resend
 *  @image html msc10_ars_arknm3_15_txcompleteWithRetransmit_210012.png
 ***********
 *  <hr>
 *  @section secname_msc210_ars_scenario3   MSC(fsm2-fsm1-24L01) ARKNM=0, 24L01 ARK_TX_FAILURE ---> FSM1 schedule 24L01 to AutoResend do a JOB again
 *  @image html msc10_ars_arknm0_txcompleteWithReentry_210012.png
 ***********
 *  <hr>
 *  @section secname_msc210_ars_scenario4   MSC(fsm2-fsm1-24L01) ARKNM=1, 24L01 ARK_TX_FAILURE ---> FSM1 schedule 24L01 to AutoResend do a JOB again
 *  @image html msc10_ars_arknm1_txcompleteWithReentry_210012.png
 ***********
 *  <hr>
 *  @section secname_uml2_autoresend UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_autoresend();
 *    note right
 *     parameter ARKNM
 *    end note
 *  }
 *  repeat
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *  while (spiTxQ.status == TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 */


/**
 *  @page name_page_fsm2_autoack FSM2 -- AutoAck
 *  - nrf2401_host2.c\n     host24L01_fsm2_autoack()
 *  - \ref name_page_uniark "Unidirectional ARK (ARK: AutoResend & AutoAck)"
 ***********
 *  <hr>
 *  @section secname_uml2_autoack UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_autoack();
 *    note right
 *     ARKNM unused @ nonperiodical AutoAck
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
 *  @page name_page_fsm2_wotautoresend FSM2 -- WOT+AutoResend
 *  - nrf2401_host2.c\n     host24L01_fsm2_wotAutoresend()
 *  - \ref name_page_uniark "Unidirectional ARK (ARK: AutoResend & AutoAck)"
 ***********
 *  <hr>
 *  @section secname_msc21_wotautoresend MSC
 *  @image html msc_12_wotautoresend.png
 ***********
 *  @section secname_uml2_wotautoresend UML
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
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *  while (spiTxQ.status == TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end
 *  @enduml
 ***********
 *  <hr>
 *  @section secname_wotars_dummypacket Dummy packet
 *  @par
    24L01 must send packet at duty-on time of every periods.\n
    What if MCU no new packet to send?\n
    MCU must take care.
 *  @par
    MCU FSM1 add dummy packets with
        + payload length 0
        + TXPID changed
        .
    if FSM2 no new data packet to send
 */

/**
 *  @page name_page_fsm2_worautoack FSM2 -- WOR+AutoAck
 *  - nrf2401_host2.c\n     host24L01_fsm2_worAutoack()
 *  - \ref name_page_uniark "Unidirectional ARK (ARK: AutoResend & AutoAck)"
 ***********
 *  <hr>
 *  @section secname_uml2_worautoack UML
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
 *******
 *  <hr>
 *  @section secname_ref_woraak_aligned Time Aligned to WOT+AutoResend
 *      + @ref secname_woraak_alignment "WOR+AutoAck Time Aligned to WOT+AutoResend"
 */

/**
 *  @page name_page_fsm2_master FSM2 -- Master
 *  - nrf2401_host2.c\n     host24L01_fsm2_master()
 *  - \ref name_page_uniark "Unidirectional ARK (ARK: AutoResend & AutoAck)"
 *  - \ref name_page_biark "Bidirectional ARK"
 ***********
 *  <hr>
 *  @section secname_msc21_master MSC
 *  @image html msc_14_master_slave.png
 ***********
 *  @section secname_uml2_master UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_master();
 *    note right
 *     parameter Period must be equal to the Remote Slave's Period
 *     parameter Period in unit milli second
 *        27 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *        46 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *       105 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *       496 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *      2436 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
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
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *  while (spiTxQ.status == TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end fork
 *  end
 *  @enduml
 *
 ***********
 *  @section secname_fsm1_master Master FSM1
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
 *  :spi_cmd0_09_txfifo_address_pointer_reset();
 *  if (spiTxQ.result.w4_tx_complete==1\nPrevious packet not yet ACKed) then (re-send\nprevious)
 *    :Old the same\nTX FIFO\nTXPID\nTXDLEN;
 *  elseif (NEW packet\nto send ?) then (send\nNEW\npacket)
 *  :movedata_spiTxQ_to_txfifo()
 *  TXDLEN=
 *  PID= changed ;
 *  else (nothing to send\nsend NEW packet with TXDLEN=0)
 *  :TXDLEN=0
 *  TXPID= changed ;
 *    note right
 *    TXDLEN=0 : dummy packet
 *    TXPID changed : new packet, though it is dummy
 *    <U+21D2> IRQ3[1] RX_complete at remote Slave
 *    <U+21D2> Slave MCU fsm1 then initiate AutoResend
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
 *  : spi_read_rxfifo_to_spiRxQ() >
 *  : callback_spiRxQ_process_at_hfsm2() ;
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
 *
 ***********
 *  <hr>
 *  @section name_page_bidir_master Bidirectional ARK
 *  - \ref name_page_biark "Bidirectional ARK"
 */

/**
 *  @page name_page_fsm2_slave FSM2 -- Slave
 *  - nrf2401_host2.c\n     host24L01_fsm2_slave()
 *  - \ref name_page_uniark "Unidirectional ARK (ARK: AutoResend & AutoAck)"
 *  - \ref name_page_biark "Bidirectional ARK"
 ***********
 *  <hr>
 *  @section secname_uml2_slave UML
 *  @startuml 
 *  start
 *  partition "Entry" {
 *  #Cyan:entry_hfsm1_slave();
 *    note right
 *     parameter Period must be equal to the Remote Master's Period
 *     parameter Period in unit milli second
 *        27 ms < Period < 32000 ms, for DR250 & SYNC 8 & FEC on & Manchester on
 *        46 ms < Period < 32000 ms, for DR125 & SYNC 8 & FEC on & Manchester on
 *       105 ms < Period < 32000 ms, for DR50  & SYNC 8 & FEC on & Manchester on
 *       496 ms < Period < 32000 ms, for DR10  & SYNC 8 & FEC on & Manchester on
 *      2436 ms < Period < 32000 ms, for DR2   & SYNC 8 & FEC on & Manchester on
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
 *  while(new data to send ?) is (no)
 *  endwhile(to TX)
 *  partition "New data to send" {
 *  #Cyan:movedata_to_spiTxQ();
 *  while (spiTxQ.status == TX_COMPLETE ?) is (not yet)
 *  endwhile (TX\ncompleted)
 *  }
 *  repeat while ()
 *  end fork
 *  end
 *  @enduml
 *
 ***********
 *  @section secname_fsm1_slave Slave FSM1
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
 *  :spi_read_rxfifo_to_spiRxQ()
 *  callback_spiRxQ_process_at_hfsm2();
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
 *  :spi_read_rxfifo_to_spiRxQ()
 *  callback_spiRxQ_process_at_hfsm2();
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
 *  :spi_cmd0_09_txfifo_address_pointer_reset();
 *  if (spiTxQ.result.w4_tx_complete==1\nPrevious packet not yet ACKed) then (re-send\nprevious)
 *    :Old the same\nTX FIFO\nTXPID\nTXDLEN;
 *  elseif (NEW packet\nto send ?) then (send\nNEW\npacket)
 *  :movedata_spiTxQ_to_txfifo()
 *  TXDLEN=
 *  TXPID= changed;
 *  else (nothing to send\nsend NEW packet with TXDLEN=0)
 *  :TXDLEN=0
 *  TXPID= changed ;
 *    note right
 *    TXDLEN=0 : dummy packet
 *    TXPID changed : new packet, though it is dummy
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
 *  @page name_page_init Initial
    - nrf2401_host.c\n     init_24L01_fifomode()
 ***********
 *  <hr>
 *  ### initial 24L01 fifomode #################
 *  @image html msc_init.png
 */

/**
 *  @page name_page_channel_freq Channel Frequency
 *  - nrf2401_host.c\n     nRF2401_frequency_configure()
 ***********
 *  <hr>
 *  ### Channel Frequency #################
 */


/**
 *  @page name_page_BCH Syncword generated by BCH code
 *  - sync_bch.c
 ***********
 *  <hr>
 *  ### SYNCWORD 8 ###############################
    @par
      + calc_syncword_64(u8 addr3932, u32 addr, u8 *syncword) to generate syncword <b>8</b> bytes
      + first 32 bits (after preamble) syncword and\n
        final 32 bits (before trailer) syncword are generated by the same BCH(32) code
      .
      <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
      <caption id="tbl_sync8"> </caption>
      <tr style="background-color:steelblue;color:white;">
          <th colspan="2"> syncword[31:0]
          <th colspan="2"> syncword[63:32]
          <th rowspan="2"> supported devices
          <th rowspan="2"> max allowed\n THOLD[1:0]
         </tr>
      <tr style="background-color:steelblue;color:white;">
          <th> BCH(32)
          <th> Barker
          <th> BCH(32)
          <th> Barker
         </tr>
      <tr><td rowspan="3"> BCH(32,26,t1) <td>  7  <td rowspan="3"> BCH(32,26,t1) <td>  7  <td align=right> 1,099,511,627,776  <td rowspan="3"> 1
      <tr>                               <td> 11                                 <td> 11  <td align=right>     4,294,967,296
      <tr>                               <td> 13                                 <td> 13  <td align=right>       268,435,456
      <tr><td rowspan="3"> BCH(32,21,t2) <td>  7  <td rowspan="3"> BCH(32,21,t2) <td>  7  <td align=right>     1,073,741,824  <td rowspan="3"> 2
      <tr>                               <td> 11                                 <td> 11  <td align=right>         4,194,304
      <tr>                               <td> 13                                 <td> 13  <td align=right>           262,144
      <tr><td rowspan="3"> BCH(32,16,t3) <td>  7  <td rowspan="3"> BCH(32,16,t3) <td>  7  <td align=right>         1,048,576  <td rowspan="3"> 3
      <tr>                               <td> 11                                 <td> 11  <td align=right>             4,096
      <tr>                               <td> 13                                 <td> 13  <td align=right>               256
      <tr><td>             BCH(32,11,t5) <td>  7  <td>             BCH(32,11,t5) <td>  7  <td align=right>             1,024  <td> 3
      </table>

 *  ### SYNCWORD 6 ###############################
    @par
      + calc_syncword_48(u32 addr, u8 *syncword) to generate syncword <b>6</b> bytes
      + first 32 bits (after preamble) syncword is generated by BCH(32) code
      + final 16 bits (before trailer) syncword is generated by BCH(16,11,t1) Barker 7 code
      .
      <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
      <caption id="tbl_sync6"> </caption>
      <tr style="background-color:steelblue;color:white;">
          <th colspan="2"> syncword[31:0]
          <th colspan="2"> syncword[47:32]
          <th rowspan="2"> supported devices
          <th rowspan="2"> max allowed\n THOLD[1:0]
         </tr>
      <tr style="background-color:steelblue;color:white;">
          <th> BCH(32)
          <th> Barker
          <th> BCH(16)
          <th> Barker
         </tr>
      <tr><td rowspan="3"> BCH(32,26,t1) <td>  7  <td rowspan="10"> BCH(16,11,t1) <td rowspan="10"> 7 <td align=right> 33,554,432  <td rowspan="3"> 1
      <tr>                               <td> 11                                                      <td align=right>  2,097,152
      <tr>                               <td> 13                                                      <td align=right>    524,288
      <tr><td rowspan="3"> BCH(32,21,t2) <td>  7                                                      <td align=right>  1,048,576  <td rowspan="3"> 1\n limit by BCH(16,11,t1)
      <tr>                               <td> 11                                                      <td align=right>     65,536
      <tr>                               <td> 13                                                      <td align=right>     16,384
      <tr><td rowspan="3"> BCH(32,16,t3) <td>  7                                                      <td align=right>     32,768  <td rowspan="3"> 1\n limit by BCH(16,11,t1)
      <tr>                               <td> 11                                                      <td align=right>      2,048
      <tr>                               <td> 13                                                      <td align=right>        512
      <tr><td>             BCH(32,11,t5) <td>  7                                                      <td align=right>      1,024  <td> 1\n limit by BCH(16,11,t1)
      </table>

 *  ### SYNCWORD 4 ###############################
    @par
      + calc_syncword_32(u32 addr, u8 *syncword) to generate syncword <b>4</b> bytes
      .
      <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
      <caption id="tbl_sync4"> </caption>
      <tr style="background-color:steelblue;color:white;">
          <th colspan="2"> syncword[31:0]
          <th rowspan="2"> supported devices
          <th rowspan="2"> max allowed\n THOLD[1:0]
         </tr>
      <tr style="background-color:steelblue;color:white;">
          <th> BCH(32)
          <th> Barker
         </tr>
      <tr><td rowspan="3"> BCH(32,26,t1) <td>  7  <td align=right> 1,048,576  <td rowspan="3"> 1, but better to set 0
      <tr>                               <td> 11  <td align=right>    65,536
      <tr>                               <td> 13  <td align=right>    16,384
      <tr><td rowspan="3"> BCH(32,21,t2) <td>  7  <td align=right>    32,768  <td rowspan="3"> 2, but better to set 1
      <tr>                               <td> 11  <td align=right>     2,048
      <tr>                               <td> 13  <td align=right>       512
      <tr><td rowspan="3"> BCH(32,16,t3) <td>  7  <td align=right>     1,024  <td rowspan="3"> 3
      <tr>                               <td> 11  <td align=right>        64
      <tr>                               <td> 13  <td align=right>        16
      <tr><td>             BCH(32,11,t5) <td>  7  <td align=right>        32  <td> 3
      </table>

 *  ### SYNCWORD 2 ###############################
    @par
      + calc_syncword_16(u32 addr, u8 *syncword) to generate syncword <b>2</b> bytes
      .
      <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
      <caption id="tbl_sync2"> </caption>
      <tr style="background-color:steelblue;color:white;">
          <th colspan="2"> syncword[15:0]
          <th rowspan="2"> supported devices
          <th rowspan="2"> max allowed\n THOLD[1:0]
         </tr>
      <tr style="background-color:steelblue;color:white;">
          <th> BCH(16)
          <th> Barker
         </tr>
      <tr><td>             BCH(16,11,t1) <td>  7  <td align=right>        32  <td> 1, but better to set 0
      </table>
 *
 */

/**
 *  @page name_page_header_addr Header Address
    @par
    <table style="background-color:#ffffff;border-collapse:collapse;" border="1" cellpadding="10">
    <caption id="multi_row">Header Address</caption>
    <tr style="background-color:steelblue;color:white;">
        <th> PLHAC_EN\n PKT3[2]
        <th> PLHLEN\n   PKT3[1]\n 0: 1 byte\n 1: 2 bytes
        <th> PLH_EN\n   PKT3[0]\n Enable
        <th> bytes
        <th> header[7:6] \n MCU write PKT2[7:6] ---> TXPID\n MCU read PKT7[7:6] <--- RXPID
        <th> header[5:0] \n PKT8[5:0]\n PLHA[5:0]
        <th> header[15:8]\n PKT9[7:0]\n PLHEA[7:0]
       </tr>
    <tr><td>                  X  <td>                X  <td> 0  <td>Disabled,\n No header field <td>-    <td>-                                  <td>-            
    <tr><td bgcolor="lime">   0  <td>                0  <td> 1  <td>               1 byte       <td>PID  <td bgcolor="lime">software use flags  <td>-            
    <tr><td bgcolor="yellow"> 1  <td>                0  <td> 1  <td>               1 byte       <td>PID  <td bgcolor="yellow">Address           <td>-
    <tr><td bgcolor="lime">   0  <td bgcolor="gold"> 1  <td> 1  <td bgcolor="gold">2 bytes      <td>PID  <td bgcolor="lime">software use flags  <td bgcolor="gold">Address
    <tr><td bgcolor="yellow"> 1  <td bgcolor="gold"> 1  <td> 1  <td bgcolor="gold">2 bytes      <td>PID  <td bgcolor="yellow">Address           <td bgcolor="gold">Address
    </table>
 *
 */


/**
 *  @page name_page_uniark Unidirectional ARK
    - ARK: AutoResend & AutoAck
    - Unidirectional: data packet only
 	  + from AutoResend to AutoAck site, or
 	  + from WOT+AutoResend to WOR+AutoAck site
 	. 
 ***********
 *  <hr>
 *  @section secname_ars_compare Comparison of AutoResend and WOT+AutoResend
    @par
    <table style="background-color:#f4f4f4;border-collapse:collapse;" border="1" cellpadding="10">
    <caption id="tbl_resend_compare">Comparison of AutoResend and WOT+AutoResend</caption>
    <tr style="background-color:steelblue;color:white;width:50%;">
        <th rowspan="2" bgcolor="#ffffff">
        <th colspan="2" bgcolor="#008080"> 24L01
        <th colspan="2" bgcolor="#008080"> MCU FSM1
        <th colspan="2" bgcolor="#008080"> MCU FSM2
       </tr>
    <tr>
        <th bgcolor="#00c0c0"> (one time)\n AutoResend
        <th bgcolor="#00a0a0"> periodical\n WOT+AutoResend
        <th bgcolor="#00c0c0"> (one time)\n AutoResend
        <th bgcolor="#00a0a0"> periodical\n WOT+AutoResend
        <th bgcolor="#00c0c0"> (one time)\n AutoResend
        <th bgcolor="#00a0a0"> periodical\n WOT+AutoResend
       </tr>
    <tr>
        <td bgcolor="#c0c0c0"> TX data packet
        <td><ul><li> data packet:\n from AutoResend site to AutoAck site
                <li> have to turn on fields\n
                     (1) Header[7:0] for PID\n
                     (2) Payload Length\n
                     (3) CRC\n
            </ul>
        <td><ul><li> (same)
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> RX ack packet
        <td><ul><li> ack/acknowledge packet:\n from AutoAck site to AutoResend site
                <li> have to turn on fields\n
                     (1) Header[7:0] for PID\n
                     (2) Payload Length\n
                     (3) CRC\n
                <li> Payload <b>Length 0</b>
                <li> Payload Length 0 choosen because 24L01 no specific field for the purpose Acknowledge
                <li> TXPID of ack packet is useless
            </ul>
        <td><ul><li> (same)
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> (remote) auto ack
        <td><ul><li> remote AutoAck site 24L01 <b>auto</b> TX an <b>ack</b> packet, if RX a data packet correctly with CRC check pass.
                <li> ack means "hello there, i have already received your data packet correctly"
            </ul>
        <td><ul><li> (same)
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> auto resend
        <td><ul><li> AutoResend site 24L01 <b>auto resend</b> TX data packet <b>again</b>, if do NOT RX ack packet correctly with CRC check pass.
            </ul>
        <td><ul><li> (same)
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> ARKNM
        <td><ul><li> allowed maximum chances of resend
            </ul>
        <td><ul><li> (same)
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> JOB
        <td><ul><li> JOB: (1+ARKNM) chances to TX data packet to and then check whether correctly RX an ack packet from the remote AutoAck site.
                <li> If correctly RX an ack packet, then JOB success and no need to do the remaining chances of REsend
                <li> If all (1+ARKNM) chances NOT RX ack packet, then JOB fails
                <li> if(JOB success): report IRQ3[0] TX Complete\n
                     else (JOB fail): report IRQ3[7] ARK TX Failure
            </ul>
        <td><ul><li> One JOB in the duty-on time of a period
                <li> If JOB fails, 24L01 auto do again the JOB in the next period.
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> Activated\n (duty on time in a period)
        <td><ul><li> only if with new data packet to send
            </ul>
        <td><ul><li> Periodical
                <li> Must send packet at duty-on time of every periods
                <li> What if no new packet to send?\n MCU must take care
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> Deactivated\n (duty off time in a period)
        <td><ul><li> if no new data packet to send
                <li> return to LightSleep mode
            </ul>
        <td><ul><li> IDLE mode (32khz low power)
            </ul>
        <td>
        <td>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> dummy packet
        <td>
        <td><ul><li> Must send packet at duty-on time of every periods
                <li> What if no new packet to send?\n MCU must take care
            </ul>
        <td>
        <td bgcolor="yellow"><ul><li> Add <b>dummy</b> packets with\n (1) payload length 0\n (2) TXPID changed\n if FSM2 no new data packet to send
            </ul>
        <td>
        <td><ul><li> Not involved
            </ul>
    <tr>
        <td bgcolor="#c0c0c0"> ATR time alignment
        <td><ul><li> no
            </ul>
        <td><ul><li> need\n (WOR+AutoAck site)
            </ul>
        <td>
        <td bgcolor="yellow"><ul><li> remote WOR+AutoAck site FSM1 handles ATR time alignment
            </ul>
        <td>
        <td><ul><li> remote WOR+AutoAck site FSM2\n Not involved in ATR time alignment
            </ul>
    <tr>
        <td bgcolor="#c0c0c0"> IRQ3[0] TX Complete
        <td><ul><li> JOB success
                <li> return to LightSleep
            </ul>
        <td><ul><li> JOB success
                <li> stlll working WOT+AutoResend
            </ul>
        <td><ul><li> report spiTxQ.status = TX_COMPLETE
            </ul>
        <td><ul><li> report spiTxQ.status = TX_COMPLETE
            </ul>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> IRQ3[7] ARK TX Failure
        <td><ul><li> JOB fails
                <li> return to LightSleep
           </ul>
        <td><ul><li> JOB fails in this period
                <li> stlll working WOT+AutoResend\n ==> always have a next period to do again the job
           </ul>
        <td bgcolor="lightyellow"><ul><li> \ref secname_msc210_ars_scenario3 "Should schedule 24L01 to do a JOB again"
                <li> "do a JOB again": must keep data packet all the same\n
                     (1) TXPID\n
                     (2) Payload\n
                     (3) Payload Length TXDLEN\n
                     (4) PKT8[5:0] flags
                <li> FSM1 therefore support the same service "No result TX_failure would be reported to FSM2"
                <li> Same service, except the Activated/Deactivated behaviour
           </ul>
        <td bgcolor="yellow"><ul><li> should ignore IRQ3[7], because 24L01 stlll working WOT+AutoResend
                <li> No result TX_failure would be reported to FSM2
           </ul>
        <td>
        <td>
    <tr>
        <td bgcolor="#c0c0c0"> FSM1 report to FSM2
        <td>
        <td>
        <td bgcolor="lightyellow"><ul><li> (same as WOT+AutoResend)
           </ul>
        <td><ul><li> TX_COMPLETE the only one result defined
                <li> FAILURE undefined.
           </ul>
        <td bgcolor="lightyellow"><ul><li> (same as WOT+AutoResend)
           </ul>
        <td><ul><li> If long time no report from FSM1, then FSM2 decide when to <b>flush</b> the not-yet-acked data packet.
           </ul>
    <tr>
        <td bgcolor="#c0c0c0"> flush
        <td>
        <td>
        <td>
        <td>
        <td><ul><li> (same as WOT+AutoResend)
           </ul>
        <td><ul><li> <b>flush</b> before TX COMPLETE make remote site RX integrity problem because of TXPID.
                <li> movedata_to_spiTxQ() always makes FSM1 use a new changed TXPID
                <li> movedata_to_spiTxQ() with old data and new TXPID: integrity problem
           </ul>
    <tr>
        <td bgcolor="#c0c0c0"> TXPID
        <td><ul><li> setting by MCU
            </ul>
        <td><ul><li> setting by MCU
            </ul>
        <td><ul><li> FSM1 write PKT2[7:6] ---> TXPID
            </ul>
        <td><ul><li> FSM1 write PKT2[7:6] ---> TXPID
            </ul>
        <td><ul><li> (same as WOT+AutoResend)
            </ul>
        <td><ul><li> movedata_to_spiTxQ() always makes FSM1 use a new changed TXPID
            </ul>
    <tr>
        <td bgcolor="#c0c0c0"> RXPID
        <td>
        <td>
        <td><ul><li> FSM1 read PKT7[7:6] <--- RXPID
            </ul>
        <td><ul><li> FSM1 read PKT7[7:6] <--- RXPID
            </ul>
        <td>
        <td>
    </table>

 ***********
 *  <hr>
 *  @section secname_ars_arknm0_txcomplete_1001 MSC: ARKNM=0 TX complete
 *  @image html msc10_ars_arknm0_txcomplete_1001.png
 ***********
 *  @section secname_ars_arknm0_txfail_1_1001 MSC: ARKNM=0 TX failure (case 1)
 *  @image html msc10_ars_arknm0_txfail_1_1001.png
 ***********
 *  @section secname_ars_arknm0_txfail_2_1001 MSC: ARKNM=0 TX failure (case 2)
 *  @image html msc10_ars_arknm0_txfail_2_1001.png
 */


/**
 *  @page name_page_biark Bidirectional ARK
 ***********
    - ARK: AutoResend & AutoAck
    - Bidirectional: data packet
 	  + from Master(WOT+AutoResend) to Slave(WOR+AutoAck) site, at duty-on time of a period
 	  + from Slave(AutoResend) to Master(AutoAck) site, at duty-off time adjacent after duty-on
 	. 
 *  @image html msc_2way_01.png
 ***********
 *  <hr>
 *  ### Considerations ##################
 *  @image html slide13.png
 *  @image html slide14.png
 *  @image html slide15.png
 *  @image html slide16.png
 */


/**
 *  @page name_page_ppt_atr_align ATR time alignment
 *  @section secname_atr_alignment ATR time alignment
 *  @image html slide2.png
 *  @image html slide3.png
 *  @image html slide4.png
 *  @image html slide5.png
 *  @image html slide6.png
 *******
 *  <hr>
 *  @section secname_wot_wor_alignment WOR Time Aligned to WOT
 *  @image html slide7.png
 *******
 *  <hr>
 *  @section secname_woraak_alignment WOR+AutoAck Time Aligned to WOT+AutoResend
 *  @image html slide8.png
 *  @image html slide9.png
 *  @image html slide10.png
 *  @image html slide11.png
 *  @image html slide12.png
 */



