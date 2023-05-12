//
// tsnd151.cpp
// tsnd151のコマンドインタフェース周りをまとめたクラス
// nonbrockとbrockが混ざっているので注意
//
// Created by KikuchiTomoo on 2022/08/25 (Auto added by Emacs 27.1)
//

#include "tsnd151.hpp"

TSND151::TSND151(const char *port_name){
  s = new SerialUSB(4096);
  buf_ = new RingDeque<char>(2048);
  recv_ = new RingDeque<char>(2048);
  events_ = new RingDequeStruct<TSND151_Event_t>(4096);
  
  /*
   * Baud rate : 115200 bps
   * Data bit  : 8 bit
   * Parity    : None
   * Stop bit  : 1 bit
   * Flow seg  : None
   */
  
  s->set_port(port_name);
  s->begin(TSND151_BAUDRATE, SERIAL_8N1);
  s->set_timeout(0, 5000);
  
  p_.body = (char*) malloc(sizeof(char) * PACKET_MAX_LEN);
  p_.len  = 0;
}

void TSND151::__set_send_buf(packet_t buf){
  s->reset_send_buf(); 
  for(int i=0; i<buf.len; i++){
    s->push_send_data(buf.body[i]);
  }
}

char TSND151::__calc_bcc(packet_t buf){
  char bcc = buf.body[0];
  for(int i=1; i<buf.len; i++){
    bcc ^= buf.body[i];
  }
  return bcc;
}

TSND151::~TSND151(){
  delete s;
  delete buf_;
  free(p_.body);
}

void TSND151::get_b_info(TSND151_info_t *info, bool is_show){
  if(is_started) return;

  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_F_GET_NAME;
  p_.body[p_.len++] = 0x00;
  p_.body[p_.len++] = __calc_bcc(p_);
  __set_send_buf(p_);
  s->send(false); // brocking

  bool isr = __recv_loop(RES_NAME, RES_NAME_PRM_SIZE);
  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return;
  }
  
  for(int i=0; i<RES_NAME_PRM_SERIAL_NUM_SIZE; i++){
    info->SerialNumber[i] = buf_->pop_left();
  }

  info->SerialNumber[10] = '\0'; // end
  
  for(int i=0; i<RES_NAME_PRM_BLUETOOTH_ADDR_SIZE; i++){
    info->BluetoothAddr[i] = buf_->pop_left();
  }

  for(int i=0; i<RES_NAME_PRM_VERSION_SIZE; i++){
    info->Version[i] = buf_->pop_left();
  }

  for(int i=0; i<RES_NAME_PRM_NAME_SIZE; i++){
    info->ModelName[i] = buf_->pop_left();
  }

  if(is_show){    
    printf("Serial    Number  : \x1b[4m%s\x1b[0m\n", info->SerialNumber);
    printf("Bluetooth Address : \x1b[4m");

    for(int i=0; i<RES_NAME_PRM_BLUETOOTH_ADDR_SIZE; i++){
      printf("%02X", info->BluetoothAddr[i] & 0x000000FF);
      if(i != RES_NAME_PRM_BLUETOOTH_ADDR_SIZE-1) printf(":");
    }
    printf("\x1b[0m\n");

    printf("Farmware  Version : \x1b[4m");
    for(int i=0; i<RES_NAME_PRM_VERSION_SIZE; i++){
      printf("%02X", info->Version[i]);      
    }
    printf("\x1b[0m\n");

    
    printf("Model     Name    : \x1b[4m%s\x1b[0m\n", info->ModelName);    
  }      
}

bool TSND151::set_b_time(TSND151_time_t time, bool is_show){
  if(is_started) return false;
  if(time.year < 2000){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot set time 2000 > year \x1b[39m\n");
    return false;
  }
  
  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_F_SET_TIME;
  p_.body[p_.len++] = (unsigned char) (time.year - 2000) & 0x000000FF;
  p_.body[p_.len++] = time.mon;
  p_.body[p_.len++] = time.day;
  p_.body[p_.len++] = time.hour;
  p_.body[p_.len++] = time.min;
  p_.body[p_.len++] = time.sec;
  p_.body[p_.len++] = time.msec & 0x000000FF;
  p_.body[p_.len++] = (time.msec >> 8) & 0x000000FF;  
  p_.body[p_.len++] = __calc_bcc(p_);
  __set_send_buf(p_);
  s->send(false); // brocking

  bool isr = __recv_loop(RES_CMD, RES_SET_TIME_PRM_SIZE);
  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return false;
  }

  char data0 = buf_->pop_left();
  bool is_success = ((data0 & 0x000000FF) == 0);
  if(is_show){    
    printf("Command Response : \x1b[4m%s (%02X)\x1b[0m\x1b[39m\n", is_success ? "\x1b[32mSuccess" : "\x1b[31mFailed", data0 & 0x000000FF);
  }

  return is_success;
}


void TSND151::get_b_time(TSND151_time_t *time, bool is_show){
  if(is_started){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Measure status enable. please stop. \x1b[39m\n");
    return;
  }
  
  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_F_GET_TIME;
  p_.body[p_.len++] = 0x00;
  p_.body[p_.len++] = __calc_bcc(p_);
  __set_send_buf(p_);
  s->send(false); // brocking

  bool isr = __recv_loop(RES_TIME, RES_GET_TIME_PRM_SIZE);

  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return;
  }
  
  time->year  = (int) buf_->pop_left() + 2000;
  time->mon   =  buf_->pop_left();
  time->day   =  buf_->pop_left();
  time->hour  =  buf_->pop_left();
  time->min   =  buf_->pop_left();
  time->sec   =  buf_->pop_left();
  time->msec |= (buf_->pop_left() << 0) & 0x000000FF;
  time->msec  = (buf_->pop_left() << 8) & 0x000000FF;

  
  if(is_show){
    printf(" Year   : \x1b[4m%d\x1b[0m\n", (int)time->year);
    printf(" Month  : \x1b[4m%d\x1b[0m\n", (int)time->mon);
    printf(" Day    : \x1b[4m%d\x1b[0m\n", (int)time->day);
    printf(" Hour   : \x1b[4m%d\x1b[0m\n", (int)time->hour);
    printf(" Min    : \x1b[4m%d\x1b[0m\n", (int)time->min);
    printf(" Sec    : \x1b[4m%d\x1b[0m\n", (int)time->sec);
    printf(" mSec   : \x1b[4m%d\x1b[0m\n", (int)time->msec);    
  }
}

bool TSND151::__check(){
  if(is_started){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Measure status enable. please stop. \x1b[39m\n");
    return false;
  }
  return true;
}

bool TSND151::__set_start(int smode, TSND151_time_t st, int emode, TSND151_time_t et, bool is_show){
  if(!__check()) return false;
  if(st.year < 2000 || et.year < 2000) return false;
  
  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_F_SET_START;
  p_.body[p_.len++] = (unsigned char) smode & 0x000000FF;
  p_.body[p_.len++] = (unsigned char) (st.year - 2000);
  p_.body[p_.len++] = st.mon;
  p_.body[p_.len++] = st.day;
  p_.body[p_.len++] = st.hour;
  p_.body[p_.len++] = st.min;
  p_.body[p_.len++] = st.sec;
  p_.body[p_.len++] = (unsigned char) emode & 0x000000FF;
  p_.body[p_.len++] = (unsigned char) (et.year - 2000);
  p_.body[p_.len++] = et.mon;
  p_.body[p_.len++] = et.day;
  p_.body[p_.len++] = et.hour;
  p_.body[p_.len++] = et.min;
  p_.body[p_.len++] = et.sec;
  p_.body[p_.len++] = __calc_bcc(p_);

  __set_send_buf(p_);
  s->send(false); // brocking

  bool isr = __recv_loop(RES_SET_START, RES_SET_START_PRM_SIZE);
  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return false;
  }

  int is_set = (int) buf_->pop_left() & 0x000000FF;
  will_start_time.year = (int) buf_->pop_left() + 2000;
  will_start_time.mon  =  buf_->pop_left();
  will_start_time.day  =  buf_->pop_left();
  will_start_time.hour =  buf_->pop_left();
  will_start_time.min  =  buf_->pop_left();
  will_start_time.sec  =  buf_->pop_left();

  will_end_time.year = (int) buf_->pop_left() + 2000;
  will_end_time.mon  =  buf_->pop_left();
  will_end_time.day  =  buf_->pop_left();
  will_end_time.hour =  buf_->pop_left();
  will_end_time.min  =  buf_->pop_left();
  will_end_time.sec  =  buf_->pop_left();

  
  if(is_show){
    printf("Start time:\n");
    printf(" Year   : \x1b[4m%d\x1b[0m\n", (int)will_start_time.year);
    printf(" Month  : \x1b[4m%d\x1b[0m\n", (int)will_start_time.mon);
    printf(" Day    : \x1b[4m%d\x1b[0m\n", (int)will_start_time.day);
    printf(" Hour   : \x1b[4m%d\x1b[0m\n", (int)will_start_time.hour);
    printf(" Min    : \x1b[4m%d\x1b[0m\n", (int)will_start_time.min);
    printf(" Sec    : \x1b[4m%d\x1b[0m\n", (int)will_start_time.sec);
    printf(" mSec   : \x1b[4m%d\x1b[0m\n", (int)will_start_time.msec);
    printf("End time:\n");
    printf(" Year   : \x1b[4m%d\x1b[0m\n", (int)will_end_time.year);
    printf(" Month  : \x1b[4m%d\x1b[0m\n", (int)will_end_time.mon);
    printf(" Day    : \x1b[4m%d\x1b[0m\n", (int)will_end_time.day);
    printf(" Hour   : \x1b[4m%d\x1b[0m\n", (int)will_end_time.hour);
    printf(" Min    : \x1b[4m%d\x1b[0m\n", (int)will_end_time.min);
    printf(" Sec    : \x1b[4m%d\x1b[0m\n", (int)will_end_time.sec);
    printf(" mSec   : \x1b[4m%d\x1b[0m\n", (int)will_end_time.msec);

    printf("Command Response : \x1b[4m%s (%02X)\x1b[0m\x1b[39m\n", is_set ? "\x1b[32mSet" : "\x1b[31mNo set", is_set & 0x000000FF);

  }

  return is_set == 0 ? false : true;  
}

bool TSND151::set_b_start_absolute(TSND151_time_t begin, TSND151_time_t end, bool is_show){
  return __set_start(1, begin, 1, end, is_show);
}

bool TSND151::set_b_start_relative(TSND151_time_t begin, TSND151_time_t end, bool is_show){
  return __set_start(0, begin, 0, end, is_show);
}

bool TSND151::set_b_start_now(TSND151_time_t end, bool is_show){
  TSND151_time_t t;

  t.year = 2001; // Dont care
  t.mon  = 1;
  t.day  = 1;
  t.hour = 0;
  t.min  = 0;
  t.sec  = 0;
  
  return __set_start(0, t, 0, end, is_show);
}

bool TSND151::set_b_start_freerun(bool is_show){
  TSND151_time_t s,e;
  s.year = 2001;
  s.mon  = 1;
  s.day  = 1;
  s.hour = 0;
  s.min  = 0;
  s.sec  = 0;
  e.year = 2001;
  e.mon  = 1;
  e.day  = 1;
  e.hour = 0;
  e.min  = 0;
  e.sec  = 0;
  return __set_start(0, s, 0, e, is_show);  
}

bool TSND151::set_b_stop(bool is_show){

  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_T_SET_STOP;
  p_.body[p_.len++] = 0x00;
  p_.body[p_.len++] = __calc_bcc(p_);

  __set_send_buf(p_);
  s->send(false);

  bool isr = __recv_loop(RES_CMD, RES_CMD_PRM_SIZE);
  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return false;
  }

  char data0 = buf_->pop_left();
  bool is_success = ((data0 & 0x000000FF) == 0);
  
  if(is_show){
     printf("Command Response : \x1b[4m%s (%02X)\x1b[0m\x1b[39m\n", is_success ? "\x1b[32mSuccess" : "\x1b[31mFailed", data0 & 0x000000FF);
  }

  return is_success;
}

bool TSND151::set_b_external_pins(TSND151_external_pin_t pin, bool is_show){
  if(!__check()) return false;

  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_T_SET_PREF_EXTERNAL;
  p_.body[p_.len++] = pin.pin1;
  p_.body[p_.len++] = pin.pin2;
  p_.body[p_.len++] = pin.pin3;
  p_.body[p_.len++] = pin.pin4;
  p_.body[p_.len++] = __calc_bcc(p_);

  __set_send_buf(p_);
  s->send(false);

  bool isr = __recv_loop(RES_CMD, RES_CMD_PRM_SIZE);
  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return false;
  }
  
  char data0 = buf_->pop_left();
  bool is_success = ((data0 & 0x000000FF) == 0);
  
  if(is_show){
     printf("Command Response : \x1b[4m%s (%02X)\x1b[0m\x1b[39m\n", is_success ? "\x1b[32mSuccess" : "\x1b[31mFailed", data0 & 0x000000FF);
  }

  return is_success;
  
}

void TSND151::get_b_external_pins(TSND151_external_pin_t *pin, bool is_show){
  if(!__check()) return;

  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_T_GET_PREF_EXTERNAL;
  p_.body[p_.len++] = 0x00;
  p_.body[p_.len++] = __calc_bcc(p_);

  __set_send_buf(p_);
  s->send(false);

  bool isr = __recv_loop(RES_PREF_EXTERNAL, RES_PREF_EXTERNAL_PRM_SIZE);
  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return;
  }

  pin->pin1 = (char) buf_->pop_left() & 0x000000FF;
  pin->pin2 = (char) buf_->pop_left() & 0x000000FF;
  pin->pin3 = (char) buf_->pop_left() & 0x000000FF;
  pin->pin4 = (char) buf_->pop_left() & 0x000000FF;
  
  if(is_show){
    printf("External Pin Assign: \n");
    printf(" Pin 1 : %s\n", pin_name_[pin->pin1]);
    printf(" Pin 2 : %s\n", pin_name_[pin->pin2]);
    printf(" Pin 3 : %s\n", pin_name_[pin->pin3]);
    printf(" Pin 4 : %s\n", pin_name_[pin->pin4]);
  }
}

bool TSND151::set_b_external_16bitAD(TSND151_external_ad_t setting, bool is_show){
  if(!__check()) return false;

  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_F_SET_PREF_EXTRL_16AD;
  p_.body[p_.len++] = setting.time_interval;
  p_.body[p_.len++] = setting.send_host_avarage;
  p_.body[p_.len++] = setting.save_mem_average;
  p_.body[p_.len++] = setting.use_pin_gain_ch1;
  p_.body[p_.len++] = setting.use_pin_gain_ch2;
  p_.body[p_.len++] = setting.use_pin_gain_ch3;
  p_.body[p_.len++] = setting.use_pin_gain_ch4;
  p_.body[p_.len++] = __calc_bcc(p_);

  __set_send_buf(p_);
  s->send(false);

 bool isr =  __recv_loop(RES_CMD, RES_CMD_PRM_SIZE);
 if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return false;
  }

  char data0 = buf_->pop_left();
  bool is_success = ((data0 & 0x000000FF) == 0);
  
  if(is_show){
     printf("Command Response : \x1b[4m%s (%02X)\x1b[0m\x1b[39m\n", is_success ? "\x1b[32mSuccess" : "\x1b[31mFailed", data0 & 0x000000FF);
  }

  return is_success;  
}

void TSND151::get_b_external_16bitAD(TSND151_external_ad_t *setting, bool is_show){
  if(!__check()) return;

  p_.len = 0;
  p_.body[p_.len++] = PACKET_HEADER;
  p_.body[p_.len++] = CMD_F_GET_RREF_EXTRL_16AD;
  p_.body[p_.len++] = 0x00;
  p_.body[p_.len++] = __calc_bcc(p_);

  __set_send_buf(p_);
  s->send(false);

  bool isr = __recv_loop(RES_PREF_EXTRL_16AD, RES_PREF_EXTERNAL_16AD_PRM_SIZE);
  if(!isr){
    fprintf(stderr, "[ERR] TSND151 : \x1b[31m Cannot unlock mutex.\x1b[39m\n");
    return;
  }

  setting->time_interval = buf_->pop_left() & 0x000000FF;
  setting->send_host_avarage = buf_->pop_left() & 0x000000FF;
  setting->save_mem_average = buf_->pop_left() & 0x000000FF;
  setting->use_pin_gain_ch1 = buf_->pop_left() & 0x000000FF;
  setting->use_pin_gain_ch2 = buf_->pop_left() & 0x000000FF;
  setting->use_pin_gain_ch3 = buf_->pop_left() & 0x000000FF;
  setting->use_pin_gain_ch4 = buf_->pop_left() & 0x000000FF;

  if(is_show){
    printf("External 16Bit Analog to Digital Setting\n");
    printf(" Time Interval    : \x1b[36m%d\x1b[39m\n", setting->time_interval);
    printf(" Send freqency    : \x1b[36m%d\x1b[39m\n", setting->send_host_avarage);
    printf(" Save freqency    : \x1b[36m%d\x1b[39m\n", setting->save_mem_average);
    printf(" Gain (Channel 1) : \x1b[36m%d\x1b[39m\n", setting->use_pin_gain_ch1);
    printf(" Gain (Channel 2) : \x1b[36m%d\x1b[39m\n", setting->use_pin_gain_ch2);
    printf(" Gain (Channel 3) : \x1b[36m%d\x1b[39m\n", setting->use_pin_gain_ch3);
    printf(" Gain (Channel 4) : \x1b[36m%d\x1b[39m\n", setting->use_pin_gain_ch4);
  }
}


bool TSND151::__recv_loop(unsigned char cmd, int prm_size){
  int ret = pthread_mutex_trylock(&m_);
  if(ret == -1) return false;
  
  buf_->reset();
  int status = 0;
  while(1){
    s->receive(false);    
    unsigned char data = (unsigned char)s->pop_recv_data();
    if(data == PACKET_HEADER && status == 0){
      status = 1;
    }

    if(status == 1){      
      if(data == cmd) status = 2;
    }else if(status == 2){ // レスポンスコマンド受信済み
      buf_->push(data);
      // printf(".");
      if(buf_->get_length() >= prm_size) status = 3;
    }else if(status == 3){ // パラメータ受信済み
      printf(" > \x1b[32mAll data received\x1b[39m\n");
      break;
    }
  }

  pthread_mutex_unlock(&m_);
  return true;
}

bool TSND151::recv_enevt_process(){
  s->receive(false);

  int ret = pthread_mutex_trylock(&m_);
  if(ret == -1) return false;

  int serial_recv_size = s->get_recv_size();

  for(int i=0; i<serial_recv_size; i++){
    unsigned char byte = (unsigned char) s->pop_recv_data();

    if(recv_status_ == TSND151_RECV_WAIT && byte == PACKET_HEADER){
      recv_status_ = TSND151_RECV_HEAD; // recv header
    }else if(recv_status_ == TSND151_RECV_HEAD){      
      recv_cmd_ = byte;  // recv command
      recv_status_ = TSND151_RECV_CMD;

      // if(TSND151_EVENT_SIZE_TABLE[recv_cmd_] == -1){
      //   recv_status_ = TSND151_RECV_RET;       
      // }
    }else if(recv_status_ == TSND151_RECV_CMD){
      recv_->push(byte); // recv data(param)
      int recved_size = recv_->get_length();
      int cmd_prm_size = TSND151_EVENT_SIZE_TABLE[recv_cmd_]+1; // prm_size + bcc(1byte)
     
      if(recved_size >= cmd_prm_size){
        // バッファが溜まったらパース
        __parse_recv_data(recv_cmd_); // parse param

        // リセットしてLITENINGにもどす
        recv_cmd_ = 0x00; // reset data
        recv_->reset();
        recv_status_ = TSND151_RECV_WAIT;
        break;
      }
    }
  }

  int serial_recv_size_e = s->get_recv_size();
  pthread_mutex_unlock(&m_);
  return true;
}


void TSND151::__parse_recv_data_time(TSND151_Event_t *event){
   int32_t time = 0;
    time |= (recv_->pop_left() & 0x000000FF) << 0;
    time |= (recv_->pop_left() & 0x000000FF) << 8;
    time |= (recv_->pop_left() & 0x000000FF) << 16;
    time |= (recv_->pop_left() & 0x000000FF) << 24;
    event->acce.time = time;
    event->velo.time = time;
    event->ex_ad.time = time;
}

void TSND151::__parse_recv_data(unsigned char cmd){
  TSND151_Event_t event;
  event.event = cmd;
  switch(cmd){
  case NOTIFY_ACCE_AND_VELO_DATA:
    __parse_recv_data_time(&event);    
    event.acce.x = 0;
    event.acce.x |= (recv_->pop_left() & 0x000000FF) << 0;
    event.acce.x |= (recv_->pop_left() & 0x000000FF) << 8;
    event.acce.x |= (recv_->pop_left() & 0x000000FF) << 16;
    event.acce.y = 0;
    event.acce.y |= (recv_->pop_left() & 0x000000FF) << 0;
    event.acce.y |= (recv_->pop_left() & 0x000000FF) << 8;
    event.acce.y |= (recv_->pop_left() & 0x000000FF) << 16;
    event.acce.z = 0;
    event.acce.z |= (recv_->pop_left() & 0x000000FF) << 0;
    event.acce.z |= (recv_->pop_left() & 0x000000FF) << 8;
    event.acce.z |= (recv_->pop_left() & 0x000000FF) << 16;

    event.velo.x = 0;
    event.velo.x |= (recv_->pop_left() & 0x000000FF) << 0;
    event.velo.x |= (recv_->pop_left() & 0x000000FF) << 8;
    event.velo.x |= (recv_->pop_left() & 0x000000FF) << 16;
    event.velo.y = 0;
    event.velo.y |= (recv_->pop_left() & 0x000000FF) << 0;
    event.velo.y |= (recv_->pop_left() & 0x000000FF) << 8;
    event.velo.y |= (recv_->pop_left() & 0x000000FF) << 16;
    event.velo.z = 0;
    event.velo.z |= (recv_->pop_left() & 0x000000FF) << 0;
    event.velo.z |= (recv_->pop_left() & 0x000000FF) << 8;
    event.velo.z |= (recv_->pop_left() & 0x000000FF) << 16;
    events_->push(event);
    break;
  case NOTIFY_GEO_DATA:
    printf("未実装 : ごめん!\n");
    break;
  case NOTIFY_ATOM_DATA:
    printf("未実装 : ごめん!\n");
    break;
  case NOTIFY_BATTARY_VOLTAGE_DATA:
    printf("未実装 : ごめん!\n");
    break;
  case NOTIFY_EXTERNAL_PIN_DATA:
    printf("未実装 : ごめん!\n");
    break;
  case NOTIFY_EXTERNAL_DETECT_EDGE:
    printf("未実装 : ごめん!\n");
    break;
  case NOTIFY_EXTERNAL_I2C_DATA:
    printf("未実装 : ごめん!\n");
    break;
  case NOTIFY_ERROR_MEASURE:
    printf("未実装 : ごめん!\n");
    break;
  case NOTIFY_STARTED_MEASURE:    
    is_started = true;
    event.started = true;
    events_->push(event);
    break;
  case NOTIFY_FINISHED_MEASURE:
    is_started = false;
    event.started = false;
    event.ended = recv_->pop_left() & 0x000000FF;
    break;
  case NOTIFY_QUATERNION_DATA:
    break;
  case NOTIFY_EXTERNAL_I2C_DATA_2:
    break;
  case NOTIFY_EXTERNAL_AD16_DATA:
     __parse_recv_data_time(&event);
     event.ex_ad.ch1  = 0;
     event.ex_ad.ch1 |= (recv_->pop_left() & 0x000000FF) << 0;
     event.ex_ad.ch1 |= (recv_->pop_left() & 0x000000FF) << 8;
     event.ex_ad.ch2  = 0;
     event.ex_ad.ch2 |= (recv_->pop_left() & 0x000000FF) << 0;
     event.ex_ad.ch2 |= (recv_->pop_left() & 0x000000FF) << 8;
     event.ex_ad.ch3  = 0;
     event.ex_ad.ch3 |= (recv_->pop_left() & 0x000000FF) << 0;
     event.ex_ad.ch3 |= (recv_->pop_left() & 0x000000FF) << 8;
     event.ex_ad.ch4  = 0;
     event.ex_ad.ch4 |= (recv_->pop_left() & 0x000000FF) << 0;
     event.ex_ad.ch4 |= (recv_->pop_left() & 0x000000FF) << 8;
     events_->push(event);
    break;
  default:
    // printf("Command Parse : \x1b[4m%s (%02X)\x1b[0m\x1b[39m\n","\x1b[31mInvalied command", cmd & 0x000000FF);
    break;
  }
}

int  TSND151::get_recv_event_num(){
  return events_->get_length();
}

TSND151_Event_t TSND151::pop_recv_event(){
  return events_->pop_left();
}
