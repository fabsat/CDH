/* 
 * File:   OBC1_main.c
 * Author: Tetsuya
 *
 * Created on 2016/02/12, 19:22
 */
#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "pic_clock.h"
#include "uart_serial.h"
#include "AD_value.h"
#include "plati_temp.h"
#include "spi_master.h"
#include "MCLR_reset.h"
#include "system_protocol.h"


//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF




int main(void) {
    
    TRISD7 = 0;
    RD7 = 0;

    MCLR_init();            // MCLR_reset 初期設定
    sysprot_init();         // system_protocol 初期設定
    uart_init();
    ad_con_init();            // AD変換の初期設定

    //spi_master_init();

    /*CWのデータセット*///ビルドエラー発生
/*    packet_format_t.data_type = 1;
    spi_master_start();
    spi_master_receive(POW, cw_t.power1);
    spi_master_receive(POW, cw_t.power2);
    spi_master_receive(POW, cw_t.power3);
    spi_master_receive(POW, cw_t.power4);
    spi_master_receive(POW, cw_t.power5);
    cw_t.temp = get_temp();
    cw_t.obc2 = 1;
    cw_t.powmcu = 1;
*/
    
    /*生存判定*/
    if(COM_READY==0) MCLR_reset(COM_RESET);
    if(POW_READY==0) MCLR_reset(POW_RESET);
    
            
    while(1){
        RD7 = 1;
//        MCLR_reset(POW_RESET);            //　これをしたらBBM全体がリセットされる
        __delay_ms(1000);
   
        RD7 = 0;
        __delay_ms(1000);
    }
    
    
    return 0;
}

