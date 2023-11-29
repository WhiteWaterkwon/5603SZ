/*********************************************************************************************************//**
 * @file    SYNC_BCH.h
 * @version $Rev:: 101          $
 * @date    $Date:: 2017-01-16 #$
 * @brief   Calc SYNC by BCH code.
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

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/* Exported types ------------------------------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------------------------------------*/

// <<< Use Configuration Wizard in Context Menu >>>

// <o> BCH(32) Select
//    <0=> BCH(32,26, 4,t1) Barker  7, support 2^(26-( 7-1))= 1,048,576 devices
//    <1=> BCH(32,26, 4,t1) Barker 11, support 2^(26-(11-1))=    65,536 devices
//    <2=> BCH(32,26, 4,t1) Barker 13, support 2^(26-(13-1))=    16,384 devices
//    <3=> BCH(32,21, 6,t2) Barker  7, support 2^(21-( 7-1))=    32,768 devices
//    <4=> BCH(32,21, 6,t2) Barker 11, support 2^(21-(11-1))=     2,048 devices
//    <5=> BCH(32,21, 6,t2) Barker 13, support 2^(21-(13-1))=       512 devices
//    <6=> BCH(32,16, 8,t3) Barker  7, support 2^(16-( 7-1))=     1,024 devices
//    <7=> BCH(32,16, 8,t3) Barker 11, support 2^(16-(11-1))=        64 devices
//    <8=> BCH(32,16, 8,t3) Barker 13, support 2^(16-(13-1))=        16 devices
//    <9=> BCH(32,11,12,t5) Barker  7, support 2^(11-( 7-1))=        32 devices
#define BCH_32_SELECT            8

// <<< end of configuration section >>>

/* Exported macro ------------------------------------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------------------------------------*/
void calc_syncword_64(u8 lap3932, u32 lap, u8 *pSyncword);
     /* first 32 bits (after preamble) syncword and                      */
     /* final 32 bits (before trailer) syncword by the same BCH(32) code */
     /* #define BCH_32_SELECT 0: lap 0 ~ FF FFFFFFFF , BCH(32,26,t1) Barker  7, support 1,099,511,627,776 devices, max allowed THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 1: lap 0 ~ 00 FFFFFFFF , BCH(32,26,t1) Barker 11, support     4,294,967,296 devices, max allowed THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 2: lap 0 ~ 00 0FFFFFFF , BCH(32,26,t1) Barker 13, support       268,435,456 devices, max allowed THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 3: lap 0 ~ 00 3FFFFFFF , BCH(32,21,t2) Barker  7, support     1,073,741,824 devices, max allowed THOLD[1:0]=2 */
     /* #define BCH_32_SELECT 4: lap 0 ~ 00 003FFFFF , BCH(32,21,t2) Barker 11, support         4,194,304 devices, max allowed THOLD[1:0]=2 */
     /* #define BCH_32_SELECT 5: lap 0 ~ 00 0003FFFF , BCH(32,21,t2) Barker 13, support           262,144 devices, max allowed THOLD[1:0]=2 */
     /* #define BCH_32_SELECT 6: lap 0 ~ 00 000FFFFF , BCH(32,16,t3) Barker  7, support         1,048,576 devices, max allowed THOLD[1:0]=3 */
     /* #define BCH_32_SELECT 7: lap 0 ~ 00 00000FFF , BCH(32,16,t3) Barker 11, support             4,096 devices, max allowed THOLD[1:0]=3 */
     /* #define BCH_32_SELECT 8: lap 0 ~ 00 000000FF , BCH(32,16,t3) Barker 13, support               256 devices, max allowed THOLD[1:0]=3 */
     /* #define BCH_32_SELECT 9: lap 0 ~ 00 000003FF , BCH(32,11,t5) Barker  7, support             1,024 devices, max allowed THOLD[1:0]=3 */

void calc_syncword_48(u32 lap, u8 *pSyncword);
     /* first 32 bits (after preamble) syncword by BCH(32) code                */
     /* final 16 bits (before trailer) syncword by BCH(16,11,t1) Barker 7 code */
     /* #define BCH_32_SELECT 0: lap 0 ~    01FFFFFF , BCH(32,26,t1) Barker  7, support        33,554,432 devices, max allowed THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 1: lap 0 ~    001FFFFF , BCH(32,26,t1) Barker 11, support         2,097,152 devices, max allowed THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 2: lap 0 ~    0007FFFF , BCH(32,26,t1) Barker 13, support           524,288 devices, max allowed THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 3: lap 0 ~    000FFFFF , BCH(32,21,t2) Barker  7, support         1,048,576 devices, max allowed THOLD[1:0]=1 limited by BCH(16,11,t1) */
     /* #define BCH_32_SELECT 4: lap 0 ~    0000FFFF , BCH(32,21,t2) Barker 11, support            65,536 devices, max allowed THOLD[1:0]=1 limited by BCH(16,11,t1) */
     /* #define BCH_32_SELECT 5: lap 0 ~    00003FFF , BCH(32,21,t2) Barker 13, support            16,384 devices, max allowed THOLD[1:0]=1 limited by BCH(16,11,t1) */
     /* #define BCH_32_SELECT 6: lap 0 ~    00007FFF , BCH(32,16,t3) Barker  7, support            32,768 devices, max allowed THOLD[1:0]=1 limited by BCH(16,11,t1) */
     /* #define BCH_32_SELECT 7: lap 0 ~    000007FF , BCH(32,16,t3) Barker 11, support             2,048 devices, max allowed THOLD[1:0]=1 limited by BCH(16,11,t1) */
     /* #define BCH_32_SELECT 8: lap 0 ~    000001FF , BCH(32,16,t3) Barker 13, support               512 devices, max allowed THOLD[1:0]=1 limited by BCH(16,11,t1) */
     /* #define BCH_32_SELECT 9: lap 0 ~    000003FF , BCH(32,11,t5) Barker  7, support             1,024 devices, max allowed THOLD[1:0]=1 limited by BCH(16,11,t1) */

void calc_syncword_32(u32 lap, u8 *pSyncword);
     /* #define BCH_32_SELECT 0: lap 0 ~    000FFFFF , BCH(32,26,t1) Barker  7, support         1,048,576 devices, max allowed THOLD[1:0]=1 but experiment shows better to allow THOLD[1:0]=0 */
     /* #define BCH_32_SELECT 1: lap 0 ~    0000FFFF , BCH(32,26,t1) Barker 11, support            65,536 devices, max allowed THOLD[1:0]=1 but experiment shows better to allow THOLD[1:0]=0 */
     /* #define BCH_32_SELECT 2: lap 0 ~    00003FFF , BCH(32,26,t1) Barker 13, support            16,384 devices, max allowed THOLD[1:0]=1 but experiment shows better to allow THOLD[1:0]=0 */
     /* #define BCH_32_SELECT 3: lap 0 ~    00007FFF , BCH(32,21,t2) Barker  7, support            32,768 devices, max allowed THOLD[1:0]=2 but experiment shows better to allow THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 4: lap 0 ~    000007FF , BCH(32,21,t2) Barker 11, support             2,048 devices, max allowed THOLD[1:0]=2 but experiment shows better to allow THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 5: lap 0 ~    000001FF , BCH(32,21,t2) Barker 13, support               512 devices, max allowed THOLD[1:0]=2 but experiment shows better to allow THOLD[1:0]=1 */
     /* #define BCH_32_SELECT 6: lap 0 ~    000003FF , BCH(32,16,t3) Barker  7, support             1,024 devices, max allowed THOLD[1:0]=3 */
     /* #define BCH_32_SELECT 7: lap 0 ~    0000003F , BCH(32,16,t3) Barker 11, support                64 devices, max allowed THOLD[1:0]=3 */
     /* #define BCH_32_SELECT 8: lap 0 ~    0000000F , BCH(32,16,t3) Barker 13, support                16 devices, max allowed THOLD[1:0]=3 */
     /* #define BCH_32_SELECT 9: lap 0 ~    0000001F , BCH(32,11,t5) Barker  7, support                32 devices, max allowed THOLD[1:0]=3 */

void calc_syncword_16(u32 lap, u8 *pSyncword);
     /* #define BCH_16_SELECT 0: lap 0 ~    0000001F , BCH(16,11,t1) Barker  7, support                32 devices, max allowed THOLD[1:0]=1 but experiment shows better to allow THOLD[1:0]=0 */


/*
 * calc_syncword_XX() are Systematic Encoding: the input data is embedded in the encoded output.
 * syncXX_to_lap()    are not Decoding, but just to truncate syncword and get only the LAP part.
 */
u32 sync16_to_lap(u8 *pSyncword);
u32 sync32_to_lap(u8 *pSyncword);
u32 sync48_to_lap(u8 *pSyncword);
typedef struct {
    u8  lap3932;  //lap[39:32]
    u32 lap;      //lap[31:0]
} lap40_for_sync64_t ;
lap40_for_sync64_t sync64_to_lap(u8 *pSyncword);


