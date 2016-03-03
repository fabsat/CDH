/******************************************************************************************
 * system_protocol.c
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * MCU間のデータ送受信プロトコルのソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2016/02/13 || Hirofumi Hamada
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





/******************************************************************************************
 *                                パケット関連の各種定義                                    *
 *****************************************************************************************/
/* Payloadの最大サイズ */
#define MAX_PAYLOAD_SIZE  30
#define MAX_PAYLOAD_INDEX (MAX_PAYLOAD_SIZE - 1) 


/* パケットフォーマット構造体 */
typedef struct
{
    uint8_t data_type;                 // Payloadデータのタイプを示す
    uint8_t payload[MAX_PAYLOAD_SIZE]; // Payload本体
    uint8_t data_end_command;          // データ終了コマンド
} packet_format_t;




/* パケットの初期化処理用マクロ */
#define PACKET_INIT                                                \
{                                                                  \
    0x00,                                                          \
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, \
    0x00                                                           \
}


/******************************************************************************************
 *                                static関数のプロトタイプ宣言                              *
 *****************************************************************************************/
static void uint8_data_set(uint8_t *p_data, uint8_t data_len);
static void send_payload(destination_t destination, uint8_t *p_payload);




/******************************************************************************************
 *                                     グローバル変数                                      *
 *****************************************************************************************/
/* このグローバル変数のパケットに各種データを格納していく データが送信されたら初期化される */
static packet_format_t packet;

/* データの格納できる先頭インデックスを示す データが送信されたら初期化される */
static uint8_t index_pos;



/******************************************************************************************
 *                                     ライブラリ関数                                      *
 *****************************************************************************************/

/*=====================================================
 * @brief
 *     Payloadに送信データを格納していく
 * @param
 *     p_data      :uint8_t, uint16_t, doubleの
 *                  配列へのポインタ
 *
 *     data_len    :p_dataは何データあるか
 *                  ただし以下のデータ数制限がある
 *                  uint8_tは15データ
 *                  uint16_tは10データ
 *                  doubleは6データ
 *
 *     byte_of_data:格納したいデータ型のバイト数
 *                  uint8_tなら1
 *                  uintt16_tなら2
 *                  doubleなら4
 * @return
 *     Flee space of payload:ペイロードの残り空き容量
 *     0xff:渡されたデータ全ては格納できない
 *          またはbyte_of_dataが適切な値でない
 * @note
 *     最大ペイロードサイズまで格納できる
 *===================================================*/
uint8_t sent_data_set(void *p_data, uint8_t data_len, uint8_t byte_of_data)
{    
    /* 渡されたデータを格納する空き容量があるか確認 なければ0xffを返す */
    if((MAX_PAYLOAD_SIZE - index_pos) - (data_len * (byte_of_data + 1)) < 0)
    {
        return 0xff;
    }
        
    /* 各型のデータ格納へ分岐 */
    switch(byte_of_data)
    {
	case 1:
            uint8_data_set((uint8_t *)p_data, data_len);
            break;
        case 2:
            uint16_data_set((uint16_t *)p_data, data_len);
            break;
        case 4:
            double_data_set((double *)p_data, data_len);
            break;
        default:
            return 0xff;
    }

    /* 残りのPayload空き容量(Byte)を返す */
    return (uint8_t)(MAX_PAYLOAD_SIZE - index_pos);
}


/*=====================================================
 * @brief
 *     PayloadへCW用データを格納する
 * @param
 *     p_cw_data:CWデータへのポインタ
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void cw_data_set(cw_t *p_cw_data)
{
    uint8_t i;
    
    /* 電源データの格納 */
    uint8_data_set(&(p_cw_data->pow_data), 4);

    /* 温度データの格納 */
    uint8_data_set(&(p_cw_data->temp_data), 4);

    /* OBC2のステータス格納 */
    uint8_data_set(&(p_cw_data->obc2), 1);

    /* COMMCUのステータス格納 */
    uint8_data_set(&(p_cw_data->commcu), 1);

    /* POWMCUのステータス格納 */
    uint8_data_set(&(p_cw_data->powmcu), 1);
}


/*=====================================================
 * @brief
 *     指定したサブシステムにデータを送信する(Master用)
 * @param
 *     destination     :送信の相手先
 *     data_type       :payloadに格納したデータのタイプ
 *     data_end_command:データはまだ継続するかどうか
 * @return
 *     void:
 * @note
 *     この関数実行後にsetしたデータ内容は初期化される
 *===================================================*/
void send_data_master(destination_t destination, data_type_t data_type, data_end_command_t data_end_command)
{
    packet.data_type        = (uint8_t)data_type;
    packet.data_end_command = (uint8_t)data_end_command;

    packet_send_master(destination, &packet);
}


void receive_data_master(destination_t destination, )



/******************************************************************************************
 *                                    static関数                                          *
 *****************************************************************************************/

/*-----------------------------------------------------
 * @brief
 *     uint8_t型のデータをPayloadに格納
 * @param
 *     p_data  :uint8_t型データへのポインタ
 *     data_len:データ個数
 * @return
 *     void:
 * @note
 *     sent_data_set()で空き容量は必ず足りている前提
 *---------------------------------------------------*/
static void uint8_data_set(uint8_t *p_data, uint8_t data_len)
{
    while(data_len)
    {
        packet.payload[index_pos++] = 1;
        packet.payload[index_pos++] = *p_data++;
        data_len--;
    }
}


/*-----------------------------------------------------
 * @brief
 *     uint16_t型のデータをpayloadに格納
 * @param
 *     p_data  :データへのポインタ
 *     data_len:データ数
 * @return
 *     void:
 * @note
 *     sent_data_set()で空き容量は必ず足りている前提
 *---------------------------------------------------*/
static void uint16_data_set(uint16_t *p_data, uint8_t data_len)
{
    uint8_t buf[3];
    uint8_t i;

    while(data_len)                               // data_lenが0になるまで
    {
        uint16_to_byte_array(p_data++, buf);      // payload用のフォーマットに変換する
        
        for(i = 0; i < 3; i++)
        {
            packet.payload[index_pos++] = buf[i]; // payloadに格納する
        }
        data_len--;
    }
}


/*-----------------------------------------------------
 * @brief
 *     double型のデータをpayloadに格納
 * @param
 *     p_data  :データへのポインタ
 *     data_len:データ数
 * @return
 *     void:
 * @note
 *     sent_data_set()で空き容量は必ず足りている前提
 *---------------------------------------------------*/
static void double_data_set(double *p_data, uint8_t data_len)
{
    uint8_t buf[5];
    uint8_t i;

    while(data_len)                               // data_lenが0になるまで
    {
        double_to_byte_array(p_data++, buf);      // payload用フォーマットに変換する

        for(i = 0; i < 5; i++)
        {
            packet.payload[index_pos++] = buf[i]; // payloadに格納する
        }
        data_len--;
    }
}


/*-----------------------------------------------------
 * @brief
 *     uint16_t型のデータを1Byteのデータ配列に変換する
 * @param
 *     p_data:変換したいデータ
 *     p_data_array:変換したいデータを格納する配列(3Byte)
 * @return
 *     void:
 * @note
 *     p_data_arrayには必ず3Byte以上を持たせる
 *---------------------------------------------------*/
static void uint16_to_byte_array(uint16_t *p_data, uint8_t *p_data_array)
{
    const uint16_t bit_mask = 0b0000000011111111;

    /* Payloadフォーマット用に先頭配列に4Byteを格納 */
    p_data_array[0] = 2;
    p_data_array[1] = (uint8_t)(*p_data >> 8);
    p_data_array[2] = (uint8_t)(*p_data & bit_mask);
}


/*-----------------------------------------------------
 * @brief
 *     double型のデータを1byteのデータ配列に変換する
 * @param
 *     p_data      :変換したいデータ
 *     p_data_array:変換したデータを格納する配列(5Byte)
 * @return
 *     void:
 * @note
 *     データ配列は必ず5Byte以上の要素数を持つこと
 *---------------------------------------------------*/
static void double_to_byte_array(double *p_data, uint8_t *p_data_array)
{
    uint8_t i;
    union  // 変換用のunionを定義
    {
        double data;
        uint8_t array[4];      
    } converter;

    /* double型のデータを受け取る */
    converter.data = *p_data;

    /* Payloadフォーマット用に先頭配列に4Byteを格納 */
    p_data_array[0] = 4;
    
    /* 引数で与えられた配列にデータをコピー */
    for(i = 0; i < 4; i++)
    {
        p_data_array[i + 1] = converter.array[i];
    }
}


/*=====================================================
 * @brief
 *     他MCUにパケットデータを送信する(SPI Master用)
 * @param
 *     destination:送信の相手先
 *     p_packet   :パケットへのポインタ
 * @return
 *     SYS_SUCCESS:パケット送信成功
 *     SYS_TIMEOUT:タイムアウト終了
 * @note
 *     none
 *===================================================*/
static sys_result_t packet_send_master(destination_t destination, packet_format_t *p_packet)
{
    uint16_t timeout_counter;

    /* プリアンブルを送信 */
    spi_master_send(destination, &p_packet->preamble);

    /* data_typeの受信可能待ち */
#if destination == OBC2
    WAIT_OR_TIMEOUT(OBC2_NOTIFICATION);
#elif destination == COMMCU
    WAIT_OR_TIMEOUT(COMMCU_NOTIFICATION);
#elif destination == POWMCU
    WAIT_OR_TIMEOUT(POWMCU_NOTIFICATION);
#endif
    
    /* data_typeを送信 */
    spi_master_send(destination, &p_packet->data_type);

    /* payload_index_endの受信可能待ち */
#if destination == OBC2
    WAIT_OR_TIMEOUT(OBC2_NOTIFICATION);
#elif destination == COMMCU
    WAIT_OR_TIMEOUT(COMMCU_NOTIFICATION);
#elif destination == POWMCU
    WAIT_OR_TIMEOUT(POWMCU_NOTIFICATION);
#endif
    
    /* payload_index_endの送信 */
    spi_master_send(destination, &p_packet->payload_index_end);
    
    /* payloadの受信可能待ち */
#if destination == OBC2
    WAIT_OR_TIMEOUT(OBC2_NOTIFICATION);
#elif destination == COMMCU
    WAIT_OR_TIMEOUT(COMMCU_NOTIFICATION);
#elif destination == POWMCU
    WAIT_OR_TIMEOUT(POWMCU_NOTIFICATION);
#endif
    
    /* ペイロードを送信 */
    send_payload(destination, p_packet->payload);

    /* data_end_commandを送信 */
    spi_master_send(destination, &p_packet->data_end_command);


    return SYS_SUCCESS;
}


/*-----------------------------------------------------
 * @brief
 *     payloadのデータ送信
 * @param
 *     destination:送信の相手先
 *     p_payload  :payloadへのポインタ
 * @return
 *     void:
 * @note
 *     送信するpayloadのバイトデータが0x00になるまで
 *     又はMAX_PAYLOAD_SIZEを送るまでSPIでデータ送信する。
 *---------------------------------------------------*/
static void send_payload(destination_t destination, uint8_t *p_payload)
{
    uint8_t i;
    uint8_t data_len;
    uint8_t index_pos = 0;                             // Counter variable for MAX_PAYLOAD_SIZE

    /* Send Payload */
    while(*p_payload != 0x00 || index_pos < MAX_PAYLOAD_SIZE)
    {
        data_len = *p_payload;                         // Get data length of transmitted data
        spi_master_send(destination, p_payload++);     // Transmit data length
        index_pos++;                                
        for(i = 0; i < data_len; i++)                  // Transmit data in data length times
        {
            spi_master_send(destination, p_payload++);
            index_pos++;
        }
    }
}







