//
// tsnd151.hpp
// tsnd151のコマンドインタフェース周りをまとめたクラス
// nonbrockingとbrockingが混ざっているので注意
//
// Created by KikuchiTomoo on 2022/08/25 (Auto added by Emacs 27.1)
//

#ifndef TSND151_HPP__
#define TSND151_HPP__

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <map>
#include "serial.hpp"
#include "deque.hpp"
#include "deque_struct.hpp"
#include "packet_define_tsnd151.hpp"


#define TSND151_BAUDRATE (115200)

#define NOTIFY_NOT_RECVED 0x00

static std::map<const unsigned char,const int> TSND151_EVENT_SIZE_TABLE = {
  {NOTIFY_ACCE_AND_VELO_DATA, NOTIFY_ACCE_AND_VELO_DATA_SIZE},
  {NOTIFY_ATOM_DATA, NOTIFY_ATOM_DATA_SIZE},
  {NOTIFY_BATTARY_VOLTAGE_DATA, NOTIFY_BATTARY_VOLTAGE_DATA_SIZE},
  {NOTIFY_ERROR_MEASURE, NOTIFY_ERROR_MEASURE_SIZE},
  {NOTIFY_GEO_DATA, NOTIFY_GEO_DATA_SIZE},
  {NOTIFY_EXTERNAL_AD16_DATA, NOTIFY_EXTERNAL_AD16_DATA_SIZE},
  {NOTIFY_EXTERNAL_DETECT_EDGE, NOTIFY_EXTERNAL_DETECT_EDGE_SIZE},
  {NOTIFY_EXTERNAL_I2C_DATA, NOTIFY_EXTERNAL_I2C_DATA_SIZE},
  {NOTIFY_EXTERNAL_I2C_DATA_2, NOTIFY_EXTERNAL_I2C_DATA_2_SIZE},
  {NOTIFY_EXTERNAL_PIN_DATA, NOTIFY_EXTERNAL_PIN_DATA_SIZE},
  {NOTIFY_STARTED_MEASURE, NOTIFY_STARTED_MEASUR_SIZE},
  {NOTIFY_FINISHED_MEASURE, NOTIFY_FINISHED_MEASURE_SIZE},
  {NOTIFY_QUATERNION_DATA, NOTIFY_QUATERNION_DATA_SIZE},
  {NOTIFY_NOT_RECVED, -1}
};

#define TSND151_RECV_WAIT  0
#define TSND151_RECV_HEAD  1
#define TSND151_RECV_CMD   2
#define TSND151_RECV_PARSE 3
#define TSND151_RECV_RET   4


class TSND151{
private:

  pthread_mutex_t m_ = PTHREAD_MUTEX_INITIALIZER;

  const char pin_name_[12][256] = {
    "\x1b[31mNC	\x1b[39m",
    "\x1b[36mInput\x1b[39m",
    "\x1b[36mNegative Edge\x1b[39m",
    "\x1b[36mPositive Edge\x1b[39m",
    "\x1b[36mNegative/Positive Edge\x1b[39m",
    "\x1b[36mNegative Edge (Cancel Chattering)\x1b[39m",
    "\x1b[36mPositive Edge (Cancel Chattering)\x1b[39m",
    "\x1b[36mNegative/Positive Edge (Cancel Chattering)\x1b[39m",
    "\x1b[33mOutput (Low)\x1b[39m",
    "\x1b[33mOutput (Hight)\x1b[39m",
    "\x1b[35mAD (Input)\x1b[39m",
    "\x1b[35mDA (Output)\x1b[39m",
  };
  
  SerialUSB *s;
  RingDeque<char> *buf_;
  RingDeque<char> *recv_;
  RingDequeStruct<TSND151_Event_t> *events_;
  
  bool is_started = false;  
  packet_t p_;

  char __calc_bcc(packet_t buf);
  void __set_send_buf(packet_t buf);

  void __send();
  void __recv();

  int recv_status_ = TSND151_RECV_WAIT;
  unsigned char recv_cmd_ = 0x00;
  int recv_pram_size_ = 0;

  bool __recv_loop(unsigned char cmd, int prm_size);
  bool __set_start(int smode, TSND151_time_t st, int emode, TSND151_time_t et, bool is_show);
  bool __check();
  void __parse_recv_data(unsigned char cmd);
  void __parse_recv_data_time(TSND151_Event_t *event);
  TSND151_time_t will_start_time;
  TSND151_time_t will_end_time;

  
public:
  TSND151(const char *port_name);
  ~TSND151();

  /**
   * 機器の情報を取得する (ブロッキング)
   * @param info[out] 機器情報取得応答
   * @param is_show[in] 情報の表示の可否
   */
  void get_b_info(TSND151_info_t *info, bool is_show = true);

  bool set_b_time(TSND151_time_t time, bool is_show = true); 
  void get_b_time(TSND151_time_t *time, bool is_show = true);

  /**
   * 計測の予約/開始をする
   * @param begin[in/out] 開始時間
   *
   */
  bool set_b_start_absolute(TSND151_time_t begin, TSND151_time_t end, bool is_show = true);
  bool set_b_start_relative(TSND151_time_t begin, TSND151_time_t end, bool is_show = true);
  bool set_b_start_now(TSND151_time_t end, bool is_show = true);

  bool set_b_start_freerun(bool is_show = true);

  bool set_b_stop(bool is_show = true);

  bool set_b_external_pins(TSND151_external_pin_t  pin, bool is_show = true);
  void get_b_external_pins(TSND151_external_pin_t *pin, bool is_show = true);

  bool set_b_external_16bitAD(TSND151_external_ad_t  setting, bool is_show = true);
  void get_b_external_16bitAD(TSND151_external_ad_t *setting, bool is_show = true);

  /* 以下 Event受け取り系 */
  bool recv_enevt_process();  
  int  get_recv_event_num();
  TSND151_Event_t pop_recv_event();

};

#endif
