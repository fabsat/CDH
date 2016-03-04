/******************************************************************************************
 * spi_master.h
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * SPI通信(マスター側)ヘッダファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2015/12/27 || Hirofumi Hamada
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#ifndef _SPI_MASTER_H
#define	_SPI_MASTER_H

#include <xc.h>
#include "pic_setting.h"
#include "system_protocol.h"

/* Slave Select Pin */
#define SS_OBC2        PORTCbits.RC1
#define SS_COMMCU      R
#define SS_POWMCU      R

/* SS pin TRIS */
#define SS_OBC2_TRIS   TRISCbits.TRISC1
#define SS_COMMCU_TRIS TRIS
#define SS_POWMCU_TRIS TRIS




/* Prototype of Function */
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
void spi_master_start(void);


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
                                uint8_t *p_store_received_data);


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
                             uint8_t *p_data);


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
void spi_master_stop(void);


#endif	/* _SPI_MASTER_H */

