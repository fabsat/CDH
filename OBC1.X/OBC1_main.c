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

//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

typedef struct
{
    unsigned char preint;
    unsigned char data_type;
    unsigned char payload_index_end;
    unsigned char payload;
    unsigned char data_end;
} packet_t;

typedef struct
{
    unsigned char power1[4];
    unsigned char power2[4];
    unsigned char temp[4];
    unsigned char OBC2:1;
    unsigned char COMMUC:1;
    unsigned char POWMCU:1;
} cw_t;

#define command 0x03
#define data    0x02
#define cw      0x01

#define Light   0x04
#define temp    0x05




int main(void) {
    
    TRISD7 = 0;
    RD7 = 0;
    __delay_ms(500);
    MCLR_init();
    uart_init();
    
    
    
    ///ad_con_init();            // AD変換の初期設定
    //spi_master_init();
    //__delay_ms(3000);
    
    /*
    union{
            double d;
            char c[1];
    }u;
    */   

    while(1){
        RD7 = 1;
        MCLR_reset(OBC2_RESET);
        __delay_ms(1000);
    //    u.d=0.0;
    //    u.d=get_temp(); 
    //    printf("%.2f\r\n", u.d);
        //spi_master_send(u.c);
    //    spi_master_send('T');
        RD7 = 0;
        __delay_ms(1000);
    }
    
    
    return 0;
}

