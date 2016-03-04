/******************************************************************************************
 * spi_master.c
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * PIC16F877A用SPI通信(Master)ソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2015/12/27 || Hirofumi Hamada
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(V3.10/Ubuntu)
 * XC8 (V1.35/Linux)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#include <xc.h>
#include "spi_master.h"
#include "pic_setting.h"
#include "system_protocol.h"


/*=====================================================
 * @brief
 *     SPI Masterスタート関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     Pin43:SDO(RC5)
 *     Pin42:SDI(RC4/SDA)
 *     Pin37:SCK(RC3/SCL)
 *===================================================*/
void spi_master_start(void)
{
    /* SPI pin I/O Configuration ( RC4 is SDI -> INPUT  Controlled by module??) */
    TRISCbits.TRISC3 = 0;    // RC3 is SCK -> OUTPUT
    TRISCbits.TRISC5 = 0;    // RC5 is SDO -> OUTPUT
    TRISCbits.TRISC4 = 1;    // RC4 is SDI -> INPUT
    
    /* SS pin configure OUTPUT */
    SS_OBC2_TRIS   = 0;      // RC1 is SS  -> OUTPUT
    //SS_COMMCU_TRIS = 0;
    //SS_POWMCU_TRIS = 0;

    /* Allow Programming of SPI configuring */
    SSPCONbits.SSPEN = 0;

    /*  SPI Mode Setup */
    SSPSTATbits.SMP = 0;     // Input data sampled at middle of data output time
    SSPCONbits.CKP  = 0;     // Idle state for clock is a low level
    SSPSTATbits.CKE = 0;     // Transmit occurs on transition from idle to active clock state
    
    /* SPI Master mode, clock = FOSC/4(Maximum speed) */
    SSPCONbits.SSPM3 = 0;          
    SSPCONbits.SSPM2 = 0;          
    SSPCONbits.SSPM1 = 0;          
    SSPCONbits.SSPM0 = 0;

    /* SS_PIN set HIGH */
    SS_OBC2   = 1;
    //SS_COMMCU = 1;
    //SS_POWMCU = 1;
   
    /* End SPI programming and Start serial port */
    SSPCONbits.SSPEN = 1;
}


/*=====================================================
 * @brief
 *     SPI Masterデータ受信関数(1Byte)
 * @param
 *     destination          :通信の相手先を選択
 *     p_store_received_data:受信データを受け取るポインタ
 * @return
 *     SYS_SUCCESS:受信成功
 *     SYS_TIMEOUT:timeout終了
 * @note
 *     1[s]で受信完了しなければTIMEOUTとなる
 *===================================================*/
sys_result_t spi_master_receive(destination_t destination,
                                uint8_t *p_store_received_data)
{
    uint8_t dummy;
    uint16_t timeout_counter = 1000;

    /* Read data to dummy */
    dummy = SSPBUF;

    /* Slave Select -> Low */
    switch(destination)
    {
        case OBC2:
            SS_OBC2   = 0;
            break;
        case COMMCU:
            //SS_COMMCU = 0;
            break;
        case POWMCU:
            //SS_POWMCU = 0;
            break;
    }

    /* Set dummy data to SSPBUF and SPI START */
    SSPBUF = 0x00;

    /* Wait for receiving finish */
    while(SSPSTATbits.BF == 0)
    {
        /* TIMEOUT (exceeds 1.0[s]) */
        if(timeout_counter == 0)
        {
            return SYS_TIMEOUT;
        }

        /* reset counter decrement */
        __delay_ms(1);
        timeout_counter--;
    }

    /* Slave Select -> High */
    switch(destination)
    {
        case OBC2:
            SS_OBC2   = 1;
            break;
        case COMMCU:
            //SS_COMMCU = 1;
            break;
        case POWMCU:
            //SS_POWMCU = 1;
            break;
    }

    /* received data is stored */
    *p_store_received_data = SSPBUF;

    /* Return SYS_SUCCESS */
    return SYS_SUCCESS;
}


/*=====================================================
 * @brief
 *     SPI Masterデータ送信関数(1Byte)
 * @param
 *     destination:通信の相手先を選択
 *     p_data     :送信データへのポインタ
 * @return
 *     SYS_SUCCESS:送信成功
 *     SYS_TIMEOUT:timeout終了
 * @note
 *     1[s]で送信完了しなければTIMEOUTとなる
 *===================================================*/
sys_result_t spi_master_send(destination_t destination,
                             uint8_t *p_data)
{
    uint8_t dummy;
    uint16_t timeout_counter = 1000;

    /* Read data to dummy */
    dummy = SSPBUF;
    
    /* Slave Select -> Low */
    switch(destination)
    {
        case OBC2:
            SS_OBC2   = 0;
            break;
        case COMMCU:
            //SS_COMMCU = 0;
            break;
        case POWMCU:
            //SS_POWMCU = 0;
            break;
    }

    /* Set dummy data to SSPBUF ,SPI START */
    SSPBUF = *p_data;

    /* Wait for receiving finish */
    while(SSPSTATbits.BF == 0)
    {
        /* TIMEOUT (exceeds 1.0[s]) */
        if(timeout_counter == 0)
        {
            return SYS_TIMEOUT;
        }

        /* reset counter decrement */
        __delay_ms(1);
        timeout_counter--;
    }

    /* Slave Select -> High */
    switch(destination)
    {
        case OBC2:
            SS_OBC2   = 1;
            break;
        case COMMCU:
            //SS_COMMCU = 1;
            break;
        case POWMCU:
            //SS_POWMCU = 1;
            break;
    }
    
    /* Read data to dummy */
    dummy = SSPBUF;

    /* Return SYS_SUCCESS */
    return SYS_SUCCESS;   
}


/*=====================================================
 * @brief
 *     SPI stop関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void spi_master_stop(void)
{
    /* SS pin -> LOW */
    SS_OBC2   = 0;
    //SS_COMMCU = 0;
    //SS_POWMCU = 0;

    /* SPI(MSSP) disable */
    SSPCONbits.SSPEN = 0;
}


