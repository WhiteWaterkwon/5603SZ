/*********************************************************************************************************//**
 * @file    SYNC_BCH.c
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
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include <string.h>                        //memcmp,
#include "sync_bch.h"



/* Private variables ---------------------------------------------------------------------------------------*/

/* Global functions ----------------------------------------------------------------------------------------*/

////////////////////////////////////////
//efine BCH_32_SELECT            6      // 0: BCH(32,26, 4,t1) Barker  7, support 2^(26-( 7-1))= 1,048,576 devices
                                        // 1: BCH(32,26, 4,t1) Barker 11, support 2^(26-(11-1))=    65,536 devices
                                        // 2: BCH(32,26, 4,t1) Barker 13, support 2^(26-(13-1))=    16,384 devices
                                        // 3: BCH(32,21, 6,t2) Barker  7, support 2^(21-( 7-1))=    32,768 devices
                                        // 4: BCH(32,21, 6,t2) Barker 11, support 2^(21-(11-1))=     2,048 devices
                                        // 5: BCH(32,21, 6,t2) Barker 13, support 2^(21-(13-1))=       512 devices
                                        // 6: BCH(32,16, 8,t3) Barker  7, support 2^(16-( 7-1))=     1,024 devices
                                        // 7: BCH(32,16, 8,t3) Barker 11, support 2^(16-(11-1))=        64 devices
                                        // 8: BCH(32,16, 8,t3) Barker 13, support 2^(16-(13-1))=        16 devices
                                        // 9: BCH(32,11,12,t5) Barker  7, support 2^(11-( 7-1))=        32 devices
#define BCH_32_N            32
#if   BCH_32_SELECT == 0
#define BCH_32_K            26
#define BCH_32_BARKER_LEN       7
#define LAP_32_K                    20  //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 1
#define BCH_32_K            26
#define BCH_32_BARKER_LEN       11
#define LAP_32_K                    16  //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 2
#define BCH_32_K            26
#define BCH_32_BARKER_LEN       13
#define LAP_32_K                    14  //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 3
#define BCH_32_K            21
#define BCH_32_BARKER_LEN       7
#define LAP_32_K                    15  //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 4
#define BCH_32_K            21
#define BCH_32_BARKER_LEN       11
#define LAP_32_K                    11  //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 5
#define BCH_32_K            21
#define BCH_32_BARKER_LEN       13
#define LAP_32_K                    9   //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 6
#define BCH_32_K            16
#define BCH_32_BARKER_LEN       7
#define LAP_32_K                    10  //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 7
#define BCH_32_K            16
#define BCH_32_BARKER_LEN       11
#define LAP_32_K                    6   //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 8
#define BCH_32_K            16
#define BCH_32_BARKER_LEN       13
#define LAP_32_K                    4   //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#elif BCH_32_SELECT == 9
#define BCH_32_K            11
#define BCH_32_BARKER_LEN       7
#define LAP_32_K                    5   //(BCH_32_K - BCH_32_BARKER_LEN + 1)
#endif
////////////////////////////////////////
#define BCH_16_SELECT            0      // 0: BCH(16,11,4,t1) Barker 7, support 2^(11-(7-1))=  32 devices
#define BCH_16_N            16
#if   BCH_16_SELECT == 0
#define BCH_16_K            11
#define BCH_16_BARKER_LEN        7
#define LAP_16_K                    5   //(BCH_16_K - BCH_16_BARKER_LEN + 1)
#endif
////////////////////////////////////////

static unsigned char g[8];
static unsigned char p[8];

#if   BCH_16_K == 7
static const unsigned char g_16_7[2]  = { 0xC0, 0x9C };
#elif BCH_16_K == 11
static const unsigned char g_16_11[2] = { 0x00, 0xB4 };
#endif

#if   BCH_32_K == 11
static const unsigned char g_32_11[4] = { 0x00, 0xFC, 0xAD, 0xE9 };
#elif BCH_32_K == 16
static const unsigned char g_32_16[4] = { 0x00, 0x80, 0x78, 0xC8 };
#elif BCH_32_K == 21
static const unsigned char g_32_21[4] = { 0x00, 0x00, 0xB0, 0x9B };
#elif BCH_32_K == 26
static const unsigned char g_32_26[4] = { 0x00, 0x00, 0x00, 0xDE };
#endif


static const unsigned char p_16[2] = { 0x34, 0xF1 };
static const unsigned char p_32[4] = { 0xAC, 0xB8, 0x4F, 0x86 };

#define BARKER_7_PAD_1                        ((u32)0x00000013)
#define BARKER_7_PAD_0                        ((u32)0x0000002C)
#define BARKER_11_PAD_1                       ((u32)0x00000123)
#define BARKER_11_PAD_0                       ((u32)0x000002DC)
#define BARKER_13_PAD_1                       ((u32)0x00000ACF)
#define BARKER_13_PAD_0                       ((u32)0x00000530)


static void array_shift_less(unsigned char bytes, unsigned char *a)
{
    unsigned char i;
    for (i = 0; i<(bytes - 1); i++) {
        a[i] >>= 1;
        if (a[i + 1] & 0x01) { a[i] |= 0x80; }
    }
    a[bytes - 1] >>= 1;
}
static void array_shift_larger(unsigned char bytes, unsigned char *a, unsigned char times)
{
    unsigned char i, tt;
    for (tt = 0; tt < times; tt++) {
    for (i = (bytes - 1); i>0; i--) {
        a[i] <<= 1;
        if (a[i - 1] & 0x80) { a[i] |= 0x01; }
    }
    a[0] <<= 1;
    }
}

/**
 *  @ingroup BC3601_BCH_label
 */
void bch32_syncword_calc(u32 lap, u8 *pIdcode)
{
    unsigned char iteration_mod;
    unsigned char aa[4];
    unsigned char gg[4];
    unsigned char tmp[4];

    #if   BCH_32_BARKER_LEN == 7
    lap &= ((((u32)1)<<LAP_32_K)-1);
    if (lap & (((u32)1)<<(LAP_32_K-1))) { lap |= (BARKER_7_PAD_1 << LAP_32_K); }
    else                                { lap |= (BARKER_7_PAD_0 << LAP_32_K); }
    #elif BCH_32_BARKER_LEN == 11
    lap &= ((((u32)1)<<LAP_32_K)-1);
    if (lap & (((u32)1)<<(LAP_32_K-1))) { lap |= (BARKER_11_PAD_1 << LAP_32_K); }
    else                                { lap |= (BARKER_11_PAD_0 << LAP_32_K); }
    #elif BCH_32_BARKER_LEN == 13
    lap &= ((((u32)1)<<LAP_32_K)-1);
    if (lap & (((u32)1)<<(LAP_32_K-1))) { lap |= (BARKER_13_PAD_1 << LAP_32_K); }
    else                                { lap |= (BARKER_13_PAD_0 << LAP_32_K); }
    #endif

    aa[0] = lap;
    aa[1] = lap >> 8;
    aa[2] = lap >> 16;
    aa[3] = lap >> 24;
    //
    array_shift_larger(4, aa, BCH_32_N - BCH_32_K);
    
#if   BCH_32_K == 11
    memcpy(g, g_32_11, 4);
#elif BCH_32_K == 16
    memcpy(g, g_32_16, 4);
#elif BCH_32_K == 21
	memcpy(g, g_32_21, 4);
#elif BCH_32_K == 26
	memcpy(g, g_32_26, 4);
#endif
    memcpy(gg, g, 4);
    memcpy(p, p_32, 4);
    //
#if   BCH_32_K == 11
    tmp[0] = 0x00;
    tmp[1] = 0x00;
    tmp[2] =(aa[2] ^ p[2])&0xE0;
    tmp[3] = aa[3] ^ p[3];
#elif BCH_32_K == 16
    tmp[0] = 0x00;
    tmp[1] = 0x00;
    tmp[2] = aa[2] ^ p[2];
    tmp[3] = aa[3] ^ p[3];
#elif BCH_32_K == 21
    tmp[0] = 0x00;
	tmp[1] =(aa[1] ^ p[1])&0xF8;
	tmp[2] = aa[2] ^ p[2];
    tmp[3] = aa[3] ^ p[3];
#elif BCH_32_K == 26
	tmp[0] =(aa[0] ^ p[0])&0xC0;
	tmp[1] = aa[1] ^ p[1];
	tmp[2] = aa[2] ^ p[2];
    tmp[3] = aa[3] ^ p[3];
#endif
    //
    for(iteration_mod=0;iteration_mod<BCH_32_K;iteration_mod++)
    {
        unsigned char ii,mask;
        ii=iteration_mod>>3;
        switch(iteration_mod&0x07) {
            case 0x00: mask=0x80; break;
            case 0x01: mask=0x40; break;
            case 0x02: mask=0x20; break;
            case 0x03: mask=0x10; break;
            case 0x04: mask=0x08; break;
            case 0x05: mask=0x04; break;
            case 0x06: mask=0x02; break;
            case 0x07: mask=0x01; break;
        }
        if(tmp[3-ii]&mask) {
            tmp[0] ^= gg[0];
            tmp[1] ^= gg[1];
            tmp[2] ^= gg[2];
            tmp[3] ^= gg[3];
        }
        array_shift_less(4, gg);
    }
    //
#if   BCH_32_K == 11
    tmp[0] ^= p[0];
    tmp[1] ^= p[1];
    tmp[2] = ((tmp[2] ^ p[2]) & 0x1F) | (aa[2] & 0xE0);
    tmp[3] = aa[3];
#elif BCH_32_K == 16
    tmp[0] ^= p[0];
    tmp[1] ^= p[1];
    tmp[2] = aa[2];
    tmp[3] = aa[3];
#elif BCH_32_K == 21
    tmp[0] ^= p[0];
	tmp[1] = ((tmp[1] ^ p[1]) & 0x07) | (aa[1] & 0xF8);
	tmp[2] = aa[2];
    tmp[3] = aa[3];
#elif BCH_32_K == 26
	tmp[0] = ((tmp[0] ^ p[0]) & 0x3F) | (aa[0] & 0xC0);
	tmp[1] = aa[1];
	tmp[2] = aa[2];
    tmp[3] = aa[3];
#endif
    //
    pIdcode[0]=tmp[3];
    pIdcode[1]=tmp[2];
    pIdcode[2]=tmp[1];
    pIdcode[3]=tmp[0];
}
/**
 *  @ingroup BC3601_BCH_label
 */
void bch16_syncword_calc(u32 lap, u8 *pIdcode)
{
    unsigned char iteration_mod;
    unsigned char aa[2];
    unsigned char gg[2];
    unsigned char tmp[2];

    #if   BCH_16_BARKER_LEN == 7
    lap &= ((((u32)1)<<LAP_16_K)-1);
    if (lap & (((u32)1)<<(LAP_16_K-1))) { lap |= (BARKER_7_PAD_1 << LAP_16_K); }
    else                                { lap |= (BARKER_7_PAD_0 << LAP_16_K); }
    #endif

    aa[0] = lap;
    aa[1] = lap >> 8;
    //
    array_shift_larger(2, aa, BCH_16_N - BCH_16_K);
    
    #if   BCH_16_K == 7
    memcpy(g, g_16_7, 2);
    #elif BCH_16_K == 11
    memcpy(g, g_16_11, 2);
    #endif
    memcpy(gg, g, 2);
    memcpy(p, p_16, 2);
    //
    #if   BCH_16_K == 7
    tmp[0] = 0x00;
    tmp[1] =(aa[1] ^ p[1]) & 0xFE;
    #elif BCH_16_K == 11
    tmp[0] =(aa[0] ^ p[0]) & 0xE0;
    tmp[1] = aa[1] ^ p[1];
    #endif
    for (iteration_mod = 0; iteration_mod < BCH_16_K; iteration_mod++)
    {
        unsigned char ii, mask;
        ii = iteration_mod >> 3;
        switch (iteration_mod & 0x07) {
        case 0x00: mask = 0x80; break;
        case 0x01: mask = 0x40; break;
        case 0x02: mask = 0x20; break;
        case 0x03: mask = 0x10; break;
        case 0x04: mask = 0x08; break;
        case 0x05: mask = 0x04; break;
        case 0x06: mask = 0x02; break;
        case 0x07: mask = 0x01; break;
        }
        if (tmp[1 - ii] & mask) {
            tmp[0] ^= gg[0];
            tmp[1] ^= gg[1];
        }
        array_shift_less(2, gg);
    }
    //
    #if   BCH_16_K == 7
    tmp[0] =   tmp[0] ^ p[0];
    tmp[1] = ((tmp[1] ^ p[1]) & 0x01) | (aa[1] & 0xFE);
    #elif BCH_16_K == 11
    tmp[0] = ((tmp[0] ^ p[0]) & 0x1F) | (aa[0] & 0xE0);
    tmp[1] =   aa[1];
    #endif
    //
    pIdcode[0]=tmp[1];
    pIdcode[1]=tmp[0];
}

void calc_syncword_16(u32 lap, u8 *pSyncword)
{
    bch16_syncword_calc(lap, pSyncword+0);
  //pSyncword[2]=0x00;
  //pSyncword[3]=0x00;
  //pSyncword[4]=0x00;
  //pSyncword[5]=0x00;
  //pSyncword[6]=0x00;
  //pSyncword[7]=0x00;
}
void calc_syncword_32(u32 lap, u8 *pSyncword)
{
    bch32_syncword_calc(lap, pSyncword+0);
  //pSyncword[4]=0x00;
  //pSyncword[5]=0x00;
  //pSyncword[6]=0x00;
  //pSyncword[7]=0x00;
}
void calc_syncword_48(u32 lap, u8 *pSyncword)
{
    volatile u32 remapped_lap_lo; //to calc first 32 bits syncword, to be consistent with HW
    volatile u32 remapped_lap_hi; //to calc final 16 bits syncword
    
    //remap: to make t3-->(t3+t1) for consecutive laps
    remapped_lap_lo  = lap & ((((u32)1)<<LAP_32_K)-1);
    remapped_lap_hi  = (lap >> LAP_32_K) & ((((u32)1)<<LAP_16_K)-1);
    remapped_lap_hi  = (remapped_lap_hi + remapped_lap_lo) & ((((u32)1)<<LAP_16_K)-1) ;

    bch32_syncword_calc(remapped_lap_lo, pSyncword+0); //to calc first 32 bits syncword, to be consistent with HW
    bch16_syncword_calc(remapped_lap_hi, pSyncword+4); //to calc final 16 bits syncword
  //pSyncword[6]=0x00;
  //pSyncword[7]=0x00;
}
void calc_syncword_64(u8 lap3932, u32 lap, u8 *pSyncword)
{
    //u8 lap3932: for maximum lap[39:0], #define LAP_32_K 20, BCH(32,26,4,t1) Barker 7
    volatile u32 remapped_lap_lo; //to calc first 32 bits syncword, to be consistent with HW
    volatile u32 remapped_lap_hi; //to calc final 32 bits syncword
    
    //remap: to make t3-->(t3+t3) for consecutive laps
    remapped_lap_lo  = lap & ((((u32)1)<<LAP_32_K)-1);
  #if LAP_32_K > 16 //16: assume u32 lap
    remapped_lap_hi  = lap3932;
    remapped_lap_hi  = ((remapped_lap_hi<<(32-LAP_32_K)) + (lap>>LAP_32_K)) & ((((u32)1)<<LAP_32_K)-1);
    //                                     32:assume u32 lap
  #else
    remapped_lap_hi  = (lap >> LAP_32_K) & ((((u32)1)<<LAP_32_K)-1);
  #endif
    remapped_lap_hi  = (remapped_lap_hi + remapped_lap_lo) & ((((u32)1)<<LAP_32_K)-1) ;
  
    bch32_syncword_calc(remapped_lap_lo, pSyncword+0); //to calc first 32 bits syncword, to be consistent with HW
    bch32_syncword_calc(remapped_lap_hi, pSyncword+4); //to calc final 32 bits syncword
}

/*
 * calc_syncword_XX() are Systematic Encoding: the input data is embedded in the encoded output.
 * syncXX_to_lap()    are not Decoding, but just to truncate syncword and get only the LAP part.
 */
u32 sync16_to_lap(u8 *pSyncword)
{
    volatile u32 lap;
    lap = pSyncword[0];
    lap = pSyncword[1] + (lap<<8);
    lap >>= (BCH_16_N - BCH_16_K);
    lap &= ((((u32)1)<<LAP_16_K)-1);
    return lap;
}
u32 sync32_to_lap(u8 *pSyncword)
{
    volatile u32 lap;
    lap = pSyncword[0];
    lap = pSyncword[1] + (lap<<8);
    lap = pSyncword[2] + (lap<<8);
    lap = pSyncword[3] + (lap<<8);
    lap >>= (BCH_32_N - BCH_32_K);
    lap &= ((((u32)1)<<LAP_32_K)-1);
    return lap;
}
u32 sync48_to_lap(u8 *pSyncword)
{
    volatile u32 remapped_lap_lo; //calc first 32 bits syncword, to be consistent with HW
    volatile u32 remapped_lap_hi; //calc final 16 bits syncword
    remapped_lap_lo = pSyncword[0];
    remapped_lap_lo = pSyncword[1] + (remapped_lap_lo<<8);
    remapped_lap_lo = pSyncword[2] + (remapped_lap_lo<<8);
    remapped_lap_lo = pSyncword[3] + (remapped_lap_lo<<8);
    remapped_lap_lo>>= (BCH_32_N - BCH_32_K);
    remapped_lap_lo &= ((((u32)1)<<LAP_32_K)-1);
    //
    remapped_lap_hi = pSyncword[4];
    remapped_lap_hi = pSyncword[5] + (remapped_lap_hi<<8);
    remapped_lap_hi >>= (BCH_16_N - BCH_16_K);
    remapped_lap_hi &= ((((u32)1)<<LAP_16_K)-1);
    //inverse remap
    remapped_lap_hi += ( ((u32)1)<<LAP_16_K   ) - (remapped_lap_lo & ((((u32)1)<<LAP_16_K)-1)) ;
    remapped_lap_hi &= ((((u32)1)<<LAP_16_K)-1);
    //
    return ((u32)(remapped_lap_hi << LAP_32_K) + remapped_lap_lo);
}
lap40_for_sync64_t sync64_to_lap(u8 *pSyncword)
{
    volatile lap40_for_sync64_t lap40;
  //volatile u32 remapped_lap_lo; //calc first 32 bits syncword, to be consistent with HW
    volatile u32 remapped_lap_hi; //calc final 16 bits syncword
    lap40.lap = pSyncword[0];
    lap40.lap = pSyncword[1] + (lap40.lap<<8);
    lap40.lap = pSyncword[2] + (lap40.lap<<8);
    lap40.lap = pSyncword[3] + (lap40.lap<<8);
    lap40.lap>>= (BCH_32_N - BCH_32_K);
    lap40.lap &= ((((u32)1)<<LAP_32_K)-1);
    //
    remapped_lap_hi = pSyncword[4];
    remapped_lap_hi = pSyncword[5] + (remapped_lap_hi<<8);
    remapped_lap_hi = pSyncword[6] + (remapped_lap_hi<<8);
    remapped_lap_hi = pSyncword[7] + (remapped_lap_hi<<8);
    remapped_lap_hi >>= (BCH_32_N - BCH_32_K);
    remapped_lap_hi &= ((((u32)1)<<LAP_32_K)-1);
    //inverse remap
    remapped_lap_hi += ( ((u32)1)<<LAP_32_K   ) - (lap40.lap & ((((u32)1)<<LAP_32_K)-1)) ;
    remapped_lap_hi &= ((((u32)1)<<LAP_32_K)-1);
  #if LAP_32_K > 16 //16: assume u32 lap
    lap40.lap3932 = remapped_lap_hi >> (32-LAP_32_K);
  #else
    lap40.lap3932 = 0x00;
  #endif
    lap40.lap = ((u32)(remapped_lap_hi << LAP_32_K)) + lap40.lap ;
    return lap40;
}


