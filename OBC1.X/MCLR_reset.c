/************************************************
 * MCLR_reset.c
 * Tetsuya Kaku
 * =============================================
 * MCLR_reset関数ソースファイル
 * =============================================
 * ・ver1.00 一版 2016/03/03 Tetsuya Kaku
 * リセットする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/
#include <xc.h>
#include <pic16f877a.h>
#include "pic_clock.h"
#include "MCLR_reset.h"


void MCLR_init(void){

    char *nin;
    nin = 0x09;
    
    OBC2_MCLR = 0;
    COM_MCLR = 0;
    POW_MCLR = 0;
    *nin = 0b00000111;
    
}

void MCLR_reset(unsigned char name){
    
    char *nin;
    nin = 0x09;
    
    *nin = 0x00;
    __delay_ms(10);
    *nin = name;

}