//
// packet_define_tsnd151.hpp
// TSND151の通信コマンドなどの定義ファイル
//
// Created by KikuchiTomoo on 2022/08/25 (Auto added by Emacs 27.1)
// 


#ifndef PACKET_DEFINE_TSND151
#define PACKET_DEFINE_TSND151

#include <map>

#define PACKET_DEFAULT_LEN (0)
#define PACKET_MAX_LEN (267)

#define PACKET_HEADR_LEN (1)
#define PACKET_CMD_CODE_LEN (1)
#define PACKET_MAX_PARAM_LEN (264)
#define PACKET_MIN_PARAM_LEN (1)
#define PACKET_BCC_LEN (1)

#define PACKET_HEADER (0x9A)

#define PACKET_TYPE_NONDEFINE (0)
#define PACKET_TYPE_CMD (1)
#define PACKET_TYPE_RES (2)
#define PACKET_TYPE_EVENT (3)

typedef struct packet{
  char *body;
  int len;
}packet_t;

typedef struct packet_tsnd151{
  const unsigned char header = PACKET_HEADER;
  unsigned char cmd = 0;
  unsigned char param[PACKET_MAX_PARAM_LEN] = {0};
  unsigned char bcc = 0;
  int param_len = 0;
  int bcc_len = 0;
}packet_TSND151_t;

// static const std::map<int,int> PRM_SIZE_TBL;

/* Commands */

#define CMD_F_GET_NAME              (0x10)  // 機器情報取得
#define CMD_F_SET_TIME              (0x11)  // 時刻設定
#define CMD_F_GET_TIME              (0x12)  // 時刻取得
#define CMD_F_SET_START             (0x13)  // 計測開始/計測予約
#define CMD_F_GET_IS_START          (0x14)  // 計測予約確認 
#define CMD_T_SET_STOP              (0x15)  // 計測停止/計測予約クリア
#define CMD_F_SET_PREF_ACCE         (0x16)  // 加速/角速度計測設定
#define CMD_F_GET_PREF_ACCE         (0x17)  // 加速/角速度計測設定取得 
#define CMD_F_SET_PREF_GEO          (0x18)  // 地磁気計測設定 
#define CMD_F_GET_PREF_GEO          (0x19)  // 地磁気計測設定取得
#define CMD_F_SET_PREF_ATOM         (0x1A)  // 気圧計測設定
#define CMD_F_GET_PREF_ATOM         (0x1B)  // 気圧計測設定取得
#define CMD_F_SET_PREF_BATTERY      (0x1C)  // バッテリ電圧計測設定
#define CMD_F_GET_PREF_BATTERY      (0x1D)  // バッテリ電圧計測設定取得 
#define CMD_F_SET_PREF_EXTERNAL_OUT (0x1E)  // 外部拡張端子計測&エッジデータ出力設定 
#define CMD_F_GET_PREF_EXTERNAL_OUT (0x1F)  // 外部拡張端子計測&エッジデータ出力設定取得 
#define CMD_F_SET_PREF_I2C          (0x20)  // 外部拡張 I2C 通信設定
#define CMD_F_GET_PREF_I2C          (0x21)  // 外部拡張 I2C 通信設定取得 
#define CMD_F_SET_RANGE_ACCE        (0x22)  // 加速度センサ計測レンジ設定
#define CMD_F_GET_RANGE_ACCE        (0x23)  // 加速度センサ計測レンジ設定取得
#define CMD_F_SET_CORRECT_ACCE      (0x24)  // 加速度センサ補正設定
#define CMD_F_SET_RANGE_VELO        (0x25)  // 角速度センサ計測レンジ設定
#define CMD_F_GET_RANGE_VELO        (0x26)  // 角速度センサ計測レンジ設定取得
#define CMD_F_SET_CORRECT_VELO      (0x27)  // 角速度センサ補正設定 
#define CMD_F_CALIBRATION_GEO       (0x28)  // 地磁気センサキャリブレーション
#define CMD_F_SET_PREF_I2C_DEV      (0x29)  // 外部拡張 I2C 通信デバイス設定
#define CMD_F_GET_PREF_I2C_DEV      (0x2A)  // 外部拡張 I2C 通信デバイス設定取得
#define CMD_F_TEST_I2C              (0x2B)  // 外部拡張 I2C 通信テスト
#define CMD_F_SET_PREF_OPT_BTN      (0x2C)  // オプションボタン操作モード設定
#define CMD_F_GET_PREF_OPT_BTN      (0x2D)  // オプションボタン操作モード設定取得
#define CMD_F_SET_RREF_OVW_DATA     (0x2E)  // 計測記録上書き設定
#define CMD_F_GET_PREF_OVW_DATA     (0x2F)  // 計測記録上書き設定取得
#define CMD_T_SET_PREF_EXTERNAL     (0x30)  // 外部拡張端子設定
#define CMD_T_GET_PREF_EXTERNAL     (0x31)  // 外部拡張端子設定取得
#define CMD_F_SET_PREF_BZZ_VOL      (0x32)  // ブザー音量設定
#define CMD_F_GET_PREF_BZZ_VOL      (0x33)  // ブザー音量設定取得
#define CMD_T_PLAY_BZZ              (0x34)  // ブザー鳴動
#define CMD_F_CLAER_DATA            (0x35)  // 計測データ記録クリア
#define CMD_F_GET_DATA_ENTRY_NUM    (0x36)  // 計測データ記録エントリ件数取得
#define CMD_F_GET_DATA_ENTRY        (0x37)  // 計測データ記録エントリ取得
#define CMD_F_GET_DATA_ENTRY_DETAIL (0x38)  // 計測データ記録エントリ詳細取得
#define CMD_F_GET_MEMORY_DATA       (0x39)  // 計測データ記録メモリ読み出し
#define CMD_F_GET_MEMORY_REMAIN     (0x3A)  // 計測データ記録メモリ残容量取得
#define CMD_F_GET_BATTRY_STATUS     (0x3B)  // バッテリ状態取得
#define CMD_T_GET_STATUS            (0x3C)  // 動作状態取得
#define CMD_F_GET_OFFSET_ACCE       (0x3D)  // 加速度センサオフセット値取得
#define CMD_F_GET_OFFSET_VELO       (0x3E)  // 角速度センサオフセット値取得
#define CMD_F_CLAER_ALL_PREF        (0x3F)  // 設定値初期化 
#define CMD_F_SET_AUTO_PWROFF_TIME  (0x50)  // オートパワーオフ時間設定
#define CMD_F_GET_AUTO_PWROFF_TIME  (0x51)  // オートパワーオフ時間設定取得
#define CMD_F_SET_IS_BLUETOOTH      (0x52)  // オフライン計測 Bluetooth 接続受付設定
#define CMD_F_GET_IS_BLUETOOTH      (0x53)  // オフライン計測 Buletooth 接続受付設定取得
#define CMD_F_BREAK_GET_MEMORY_DATA (0x54)  // 計測データ記録メモリ読み出し中断
#define CMD_F_SET_PREF_QUATERNION   (0x55)  // クオータニオン計測設定
#define CMD_F_GET_PREF_QUATERNION   (0x56)  // クオータニオン計測設定取得
#define CMD_F_SET_PREF_I2C_DEV_2    (0x57)  // 外部拡張 I2C 通信デバイス設定 2
#define CMD_F_GET_PREF_I2C_DEV_2    (0x58)  // 外部拡張 I2C 通信デバイス設定取得 2
#define CMD_F_SET_PREF_EXTRL_16AD   (0x59)  // 拡張 16bitAD 計測設定
#define CMD_F_GET_RREF_EXTRL_16AD   (0x5A)  // 拡張 16bitAD 計測設定取得
#define CMD_T_SET_PREF_EXTRL_DALV   (0x5B)  // 外部拡張端子 DA 出力レベル設定
#define CMD_F_GET_DATA_ENTRY_2      (0x5C)  // 計測データ記録エントリ取得 2 
#define CMD_F_GET_CAN_DATA_SAVE     (0x5D)  // 計測記録可否チェック

/* Response */

#define RES_CMD                     (0x8F) // コマンドレスポンス
#define RES_NAME                    (0x90) // 機器情報取得応答
#define RES_TIME                    (0x92) // 時刻取得応答
#define RES_SET_START               (0x93) // 計測時刻応答
#define RES_PREF_ACCE_VELO          (0x97) // 加速/角速度計測設定応答
#define RES_PREF_GEO                (0x99) // 地磁気計測設定応答
#define RES_PREF_ATOM               (0x9B) // 気圧計測設定応答
#define RES_PREF_BATTARY            (0x9D) // バッテリ電圧計測設定応答
#define RES_PREF_EXTERNAL_OUT       (0x9F) // 外部拡張端子計測&エッジデータ出力設定応答
#define RES_PREF_I2C                (0xA1) // 外部拡張 I2C 通信設定応答
#define RES_PREF_RANGE_ACCE         (0xA3) // 加速度センサ計測レンジ設定応答
#define RES_PREF_RANGE_VELO         (0xA6) // 角速度センサ計測レンジ設定応答
#define RES_PREF_I2C_DEV            (0xAA) // 外部拡張 I2C 通信デバイス設定応答
#define RES_TEST_I2C                (0xAB) // 外部拡張 I2C 通信テスト応答
#define RES_PREF_OPT_BTN            (0xAD) // オプションボタン操作モード設定応答
#define RES_PREF_OVW_DATA           (0xAF) // 計測記録上書き設定応答
#define RES_PREF_EXTERNAL           (0xB1) // 外部拡張端子設定応答
#define RES_PREF_BZZ_VOL            (0xB3) // ブザー音量設定応答
#define RES_DATA_ENTRY_NUM          (0xB6) // 計測データ記録エントリ件数応答
#define RES_DATA_ENTRY              (0xB7) // 計測データ記録エントリ応答
#define RES_DATA_ENTRY_DETAIL       (0xB8) // 計測データ記録エントリ詳細応答
#define RES_FINISH_READING_MEMORY   (0xB9) // 計測データ記録メモリ読み出し完了応答
#define RES_REMAIN_MEMORY_SIZE      (0xBA) // 計測データ記録メモリ残容量応答
#define RES_BATTRY_STATUS           (0xBB) // バッテリ状態応答
#define RES_STATUS                  (0xBC) // 動作状態応答
#define RES_OFFSET_ACCE             (0xBD) // 加速度センサオフセット値応答
#define RES_OFFSET_VELO             (0xBE) // 角速度センサオフセット値応答
#define RES_PREF_AUTO_PWROFF_TIME   (0xD1) // オートパワーオフ時間設定取得応答
#define RES_IS_BLUETOOTH_STATUS     (0xD3) // オフライン計測 Bluetooth 接続受付設定取得応答
#define RES_PREF_QUATERNION         (0xD6) // クオータニオン計測設定応答
#define RES_PREF_I2C_DEV_2          (0xD8) // 外部拡張 I2C 通信デバイス設定応答 2
#define RES_PREF_EXTRL_16AD         (0xDA) // 拡張 16bitAD 計測設定応答
#define RES_PREF_DATA_ENTRY_2       (0xDC) // 計測データ記録エントリ応答 2
#define RES_CAN_DATA_SAVE           (0xDD) // 計測記録可否チェック応答

/* Events */
#define NOTIFY_ACCE_AND_VELO_DATA   (0x80) // 加速度角速度計測データ通知
#define NOTIFY_GEO_DATA             (0x81) // 地磁気計測データ通知
#define NOTIFY_ATOM_DATA            (0x82) // 気圧計測データ通知
#define NOTIFY_BATTARY_VOLTAGE_DATA (0x83) // バッテリ電圧データ通知 
#define NOTIFY_EXTERNAL_PIN_DATA    (0x84) // 外部拡張端子データ通知
#define NOTIFY_EXTERNAL_DETECT_EDGE (0x85) // 外部拡張端子エッジ検出通知
#define NOTIFY_EXTERNAL_I2C_DATA    (0x86) // 外部拡張 I2C 受信データ通知
#define NOTIFY_ERROR_MEASURE        (0x87) // 計測エラー通知
#define NOTIFY_STARTED_MEASURE      (0x88) // 計測開始通知
#define NOTIFY_FINISHED_MEASURE     (0x89) // 計測終了通知
#define NOTIFY_QUATERNION_DATA      (0x8A) // クオータニオン計測データ通知
#define NOTIFY_EXTERNAL_I2C_DATA_2  (0x8B) // 外部拡張 I2C 受信データ通知 2
#define NOTIFY_EXTERNAL_AD16_DATA   (0x8C) // 拡張 16bitAD 計測データ通知

#define NOTIFY_ACCE_AND_VELO_DATA_SIZE   (22)
#define NOTIFY_GEO_DATA_SIZE             (13)
#define NOTIFY_ATOM_DATA_SIZE            (9)
#define NOTIFY_BATTARY_VOLTAGE_DATA_SIZE (7)
#define NOTIFY_EXTERNAL_PIN_DATA_SIZE    (9)
#define NOTIFY_EXTERNAL_DETECT_EDGE_SIZE (6)
#define NOTIFY_EXTERNAL_I2C_DATA_SIZE    (13)
#define NOTIFY_ERROR_MEASURE_SIZE        (5)
#define NOTIFY_STARTED_MEASUR_SIZE       (1)
#define NOTIFY_FINISHED_MEASURE_SIZE     (1)
#define NOTIFY_QUATERNION_DATA_SIZE      (30)
#define NOTIFY_EXTERNAL_I2C_DATA_2_SIZE  (22)
#define NOTIFY_EXTERNAL_AD16_DATA_SIZE   (12)

#define RES_NAME_PRM_SIZE                (30)
#define RES_NAME_PRM_SERIAL_NUM_SIZE     (10)
#define RES_NAME_PRM_BLUETOOTH_ADDR_SIZE (6)
#define RES_NAME_PRM_VERSION_SIZE        (4)
#define RES_NAME_PRM_NAME_SIZE           (10)

#define RES_SET_TIME_PRM_SIZE            (1)

#define RES_GET_TIME_PRM_SIZE            (8)
#define RES_TIME_PRM_YEAR_SIZE           (1)
#define RES_TIME_PRM_MON_SIZE            (1)
#define RES_TIME_PRM_DAY_SIZE            (1)
#define RES_TIME_PRM_HOUR_SIZE           (1)
#define RES_TIME_PRM_MINSIZE             (1)
#define RES_TIME_PRM_SEC_SIZE            (1)
#define RES_TIME_PRM_MSEC_SIZE           (2)
#define RES_SET_START_PRM_SIZE           (13)
#define RES_CMD_PRM_SIZE                 (1)
#define RES_PREF_EXTERNAL_PRM_SIZE       (4)
#define RES_PREF_EXTERNAL_16AD_PRM_SIZE  (7)

typedef struct {
  char SerialNumber[11]; // 文字列“AP12345678”
  char BluetoothAddr[6]; // 00:11:22:33:44:55
  char Version[4];       // 0x00000000～0xFFFFFFFF
  char ModelName[10];    // 0x00(\0)まで有効
}TSND151_info_t;

typedef struct {
  int year;
  char mon;
  char day;
  char hour;
  char min;
  char sec;
  uint16_t msec;
}TSND151_time_t;

typedef struct{
  char pin1;
  char pin2;
  char pin3;
  char pin4;
}TSND151_external_pin_t;

typedef unsigned char TSND151_AD_GAINS_t;

#define TSND151_AD_OFF     (0)
#define TSND151_AD_GAIN_1  (1)
#define TSND151_AD_GAIN_2  (2)
#define TSND151_AD_GAIN_3  (3)
#define TSND151_AD_GAIN_4  (4)
#define TSND151_AD_GAIN_6  (6)
#define TSND151_AD_GAIN_8  (8)
#define TSND151_AD_GAIN_12 (12)

typedef struct{
  unsigned char time_interval;
  unsigned char send_host_avarage;
  unsigned char save_mem_average;
  unsigned char use_pin_gain_ch1;
  unsigned char use_pin_gain_ch2;
  unsigned char use_pin_gain_ch3;
  unsigned char use_pin_gain_ch4;  
}TSND151_external_ad_t;

#define TSND151_EXTRN_NC           (0)
#define TSND151_EXTRN_IN           (1)
#define TSND151_EXTRN_NEG_EDGE     (2)
#define TSND151_EXTRN_POS_EDGE     (3)
#define TSND151_EXTRN_BOTH_EDGE    (4)
#define TSND151_EXTRN_NEG_EDGE_C   (5)
#define TSND151_EXTRN_POS_EDGE_C   (6)
#define TSND151_EXTRN_BOTH_EDGE_C  (7)
#define TSND151_EXTRN_OUT_LOW      (8)
#define TSND151_EXTRN_OUT_HIGH     (9)

#define TSND151_EXTRN_AD           (10)
#define TSND151_EXTRN_DA           (11)

typedef struct{
  int32_t time;
  int32_t x;
  int32_t y;
  int32_t z;
}Velocity_data_t;

typedef struct{
  int32_t time;
  int32_t x;
  int32_t y;
  int32_t z;
}Acceleration_data_t;

typedef struct{
  int32_t time;
  int32_t x;
  int32_t y;
  int32_t z;
}Geo_data_t;

typedef struct{
  int32_t time;
  int32_t atom;
  int32_t temp;
}Atom_data_t;

typedef struct{
  int32_t time;
  int16_t voltage;
  int8_t  remain_per;
}Battery_data_t;

typedef struct{
  int32_t time;
  bool pin1_high;
  bool pin2_high;
  bool pin3_high;
  bool pin4_high;
  int16_t ch3_ad;
  int16_t ch4_ad;
}External_pin_data_t;

typedef struct{
  int32_t time;
  bool pin1_detect;
  bool pin2_detect;
  bool pin3_detect;
  bool pin4_detect;
  bool option_button_detect;
}Edge_data_t;

typedef struct{
  int32_t time;
  bool is_success;
  unsigned char data;
}External_I2C_data_t;

typedef struct{
  int32_t time;
  unsigned char code;
}Error_data_t;

typedef bool Started_data_t;
typedef unsigned char Finished_data_t;

typedef struct{
  int32_t time;
  int16_t qx;
  int16_t qy;
  int16_t qz;
  Acceleration_data_t acce;
  Velocity_data_t     velo;
}Quaternion_data_t;

typedef struct{
  int32_t time;
  unsigned char device_id;
  unsigned char recv_status;
  uint16_t recv_data;
}External_I2C_2_data_t;

typedef struct{
  int32_t time;
  int16_t ch1;
  int16_t ch2;
  int16_t ch3;
  int16_t ch4;  
}External_16bitAD_data_t;
  
typedef struct{
  unsigned char           event;
  Velocity_data_t         velo;
  Acceleration_data_t     acce;
  Geo_data_t              geo;
  Atom_data_t             atom;
  Battery_data_t          battery;
  Quaternion_data_t       quaternion;
  External_16bitAD_data_t ex_ad;
  External_pin_data_t     ex_pin;
  External_I2C_data_t     ex_i2c;
  External_I2C_2_data_t   ex_i2c_2;
  Error_data_t            error;
  Started_data_t          started;
  Finished_data_t         ended;
  // bool is_valied; /// TODO: bccを計算して値の有効チェックを行う
}TSND151_Event_t;

#endif
