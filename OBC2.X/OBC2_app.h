/* 
 * File:   OBC2_app.h
 * Author: Tetsuya
 *
 * Created on 2016/03/06, 20:24
 */

#ifndef OBC2_APP_H
#define	OBC2_APP_H

#include <xc.h>
#include "pic_clock.h"

#define LED0TRIS         TRISDbits.TRISD7
#define LED0             PORTDbits.RD7

#define OBC1_MCLR        TRISEbits.TRISE0
#define OBC1_STATUS      PORTEbits.RE0
#define OBC1_ACK_TRIS    TRISDbits.TRISD5
#define OBC1_ACK         PORTDbits.RD5
#define PORTE_REG_ADR    0x09                 // PORTEのレジスタアドレス
#define PORTE_POEWR_ON   0x01                 // RE0をON


/*=====================================================
 * @brief
 *     MCLRリセットの初期化
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void OBC1_RESET_init(void);

/*=====================================================
 * @brief
 *     MCLRリセットを実行
 * @param
 *     resetted_mcu:リセットされるMCUを選択
 *                  ヘッダファイルのマクロを使用
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void OBC1_reset(void);


#endif	/* OBC2_APP_H */

