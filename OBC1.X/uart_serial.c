/************************************************
 * uart_serial.c
 * Tetsuya Kaku
 * =============================================
 * uart通信(シリアル)定義ソースファイル
 * =============================================
 * ・ver1.00 初版 2015/12/09 Tetsuya Kaku
 * uartでPCとシリアル通信をする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/
#include <xc.h>
#include "uart_serial.h"
#include "pic_clock.h"


/*=====================================================
 * @brief
 *     uart通信初期設定関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     型番によってRXピンを確認する必要がある
 *===================================================*/
void uart_init(void)
{
    TRISCbit.TRISC7 = 1;         // RX=RC7 is serial data input
    SPBRG = SPBRG_DATA;
    TXSTA = (TX9_RX9_DATA | BRGH_DATA | 0x20);
    RCSTA = (TX9_RX9_DATA | 0x90);
}


/*=====================================================
 * @brief
 *     1Byteデータ送信
 * @param
 *     byte:送信データ
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void putch(uint8_t byte)
{
    while(!TXIF)
    {
        ;
    }
    TXREG = byte;
}


/*=====================================================
 * @brief
 *     文字列送信
 * @param
 *     str:文字列へのポインタ
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void put_string(uint8_t *str)
{
    while(*str != '\0')
    {
        putch(*str++);
    }
    putch('\0');
}


/*=====================================================
 * @brief
 *     1Byte受信
 * @param
 *     void:
 * @return
 *     RCREG:受信データ
 * @note
 *     none
 *===================================================*/
uint8_t getch(void)
{
    while(!RCIF)
    {
        ;
        
    }
 
    return RCREG;
}

