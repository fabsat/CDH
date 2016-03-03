/************************************************
 * pic_clock.h
 * Tetsuya Kaku
 * =============================================
 * delay用のクロック定義ヘッダファイル
 * =============================================
 * ・ver1.00 初版 2015/12/09 Tetsuya Kaku
 * 20MHzで定義
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#ifndef MCLR_RESET_H
#define	MCLR_RESET_H
#include <xc.h>
#include "pic_clock.h"

#define OBC2_MCLR     TRISEbits.TRISE2     //MCLRに使用するピンをRE2
#define OBC2_RESET    0x04
#define COM_MCLR      TRISEbits.TRISE1     //MCLRに使用するピンをRE1
#define COM_RESET     0x02
#define POW_MCLR      TRISEbits.TRISE0     //MCLRに使用するピンをRE0
#define POW_RESET     0x01

void MCLR_init(void);
void MCLR_reset(unsigned char name);

#endif	/* MCLR_RESET_H */