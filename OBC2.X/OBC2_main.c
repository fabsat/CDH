/* 
 * File:   OBC2_main.c
 * Author: Tetsuya
 *
 * Created on 2016/02/12, 19:31
 */
#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "pic_clock.h"
#include "uart_serial.h"
#include "spi.h"
#include "i2c.h"
#include "tempADT.h"
#include "OBC2_app.h"
#include "spi_master.h"
//#include "spi_slave.h"

//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

/*OBC1になる関数*/
void OBC1(void);

int main(void){

    LED0TRIS = 0;
    OBC1_RESET_init();
    if(OBC1_STATUS) OBC1_reset();       // OBC1の状態によってOBC1をリセットする
    I2C_init();
//    spi_slave_init(SPI_ISR_ENABLE);
    uart_init();
    
    while(1)
    {
        LED0 = 1;
        __delay_ms(100);
        
        while(!OBC1_ACK_TRIS)
        {
            int count = 0;
            if(count < 100){
                OBC1_reset();       // OBC1の状態によってOBC1をリセットする
                count++;
            }else{
                OBC1();
            }
        }
        
        //OBC2();
        
               
        LED0 = 0;
        __delay_ms(100);
    }
    
    return 0;
}

void OBC1(void)
{
    MCLR_init();            // MCLR_reset 初期設定
    sysprot_init();         // system_protocol 初期設定
    spi_master_start();
    uart_init();
    
    while(1)
    {
        LED0 = 1;
        __delay_ms(1000);

        /*COMのステータス確認*/
        COM_status();

        /*POWのステータス確認*/
        POW_status();
        
        /*OBC1作業関数*/
        command(COM_status);

        LED0 = 0;
        __delay_ms(1000);   
    }
}
