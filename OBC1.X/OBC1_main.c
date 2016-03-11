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
#include "MCLR_reset.h"
#include "uart_serial.h"
#include "AD_value.h"
#include "plati_temp.h"
#include "spi_master.h"
#include "system_protocol.h"
#include "OBC1_app.h"


//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

cw_t cw = CW_DATA_INIT;

int main(void){
    
    LED0TRIS = 0;
    LED0 = 0;
    LED1TRIS = 0;
    LED1 = 0;
    
    MCLR_init();            // MCLR_reset 初期設定
    sysprot_init();         // system_protocol 初期設定
    ad_con_init();            // AD変換の初期設定
    spi_master_start();
    uart_init();
    __delay_ms(1000);
    
    while(1)
    {
        LED0 = 1;
        __delay_ms(1000);

        /*COMのステータス確認*/
        uint8_t COM_stat = COM_status();

        /*POWのステータス確認*/
        POW_status();
        
        /*OBC1作業関数*/
        command(COM_stat);

        LED0 = 0;
        __delay_ms(1000);   
    }
    
    return 0;
}
