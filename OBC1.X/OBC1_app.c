/******************************************************************************************
 * OBC1_app.c
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * OBC1プロトコルのソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2016/03/06 || Tetsuya Kaku
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE
 * XC8
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#include <xc.h>
#include "pic_clock.h"
#include "system_protocol.h"
#include "spi_master.h"
#include "OBC1_app.h"
#include "MCLR_reset.h"


static void LED_data_set(void);
static void I2Ctemp_data_set(void);


void COM_status(void)
{
    uint8_t COM_status = 0;
    spi_master_send(COM, 0x01);
    COM_status = spi_master_receive(COM);           // COMのステータス受け取り
    if(COM_status == 0x00 || COM_status == 0xFF)    // COMの生存判定
    {
        MCLR_reset(COM_RESET);                      // COMをリセット
    }
}

void POW_status(void)
{
    spi_master_send(POW, 0x01);
    if(POW_READY==0) MCLR_reset(POW_RESET);         //POWの生存判定＆POWをリセット
}

void command(uint8_t data)
{
    switch(data)
    {
        case GET_CW_DATA:
            get_cw_data();
            cw_data_set(&cw);
            spi_master_send(COM, 0x11);
            send_data_master(COM, CW, DATA_END);
            break;
            
        case LED_BLINK:
            LED1 = 1;
            __delay_ms(100);
            LED1 = 0;
            __delay_ms(100);
            LED_data_set();
            spi_master_send(COM, 0x11);
            send_data_master(COM, LED_BLINK, DATA_END);
            break;

        case GET_I2C_TEMP:
            spi_master_send(OBC2, 0x01);
            I2Ctemp_data_set();
            spi_master_send(COM, 0x11);
            send_data_master(COM, DATA, DATA_END);
            break;
    }
}


static void LED_data_set(void)
{
    sent_data_set(0x01, 1, 1);
}

static void I2Ctemp_data_set(void)
{
    receive_data_master(OBC2);
}
