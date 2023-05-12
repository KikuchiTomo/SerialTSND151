#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// DEBUG OUTPUT 有効
// #define DEBUG_OUT
#define PORT_NAME "/dev/tty.usbmodemAP031607481"

#include "tsnd151.hpp"

int main(void){

  TSND151 *tsnd = new TSND151(PORT_NAME);

  TSND151_info_t info_d;  
  TSND151_time_t time_d;
  TSND151_external_ad_t  ad_d;
  TSND151_external_pin_t pins_d;

  // Gett information
  printf("TSND151 Get Information \n");
  tsnd->get_b_info(&info_d);

  // Set time
  printf("TSND151 Set Current time : ");
  struct timeval now_time;
  struct tm *pnow;
  gettimeofday(&now_time, NULL);
  pnow = localtime(&now_time.tv_sec);
     
  time_d.year = pnow->tm_year + 1900;
  time_d.mon  = pnow->tm_mon  + 1;
  time_d.day  = pnow->tm_mday;
  time_d.hour = pnow->tm_hour;
  time_d.min  = pnow->tm_min;
  time_d.sec  = pnow->tm_sec;
  time_d.msec = (int) (now_time.tv_usec / 1000);
  printf("%d/%d/%d %d:%d:%d:%d\n", time_d.year, time_d.mon, time_d.day, time_d.hour, time_d.min, time_d.sec, time_d.msec);
  tsnd->set_b_time(time_d);

  // Get Time
  printf("TSND151 Get Current time \n");
  tsnd->get_b_time(&time_d);

  // Get pins
  printf("TSND151 Get pins info \n");
  tsnd->get_b_external_pins(&pins_d);

  ad_d.time_interval = 1;
  ad_d.send_host_avarage = 1;
  ad_d.save_mem_average = 0;
  ad_d.use_pin_gain_ch1 = 2;
  ad_d.use_pin_gain_ch2 = 2;
  ad_d.use_pin_gain_ch3 = 2;
  ad_d.use_pin_gain_ch4 = 2;

  printf("TSND151 Set AD settings\n");
  tsnd->set_b_external_16bitAD(ad_d);
  
  // Get AD settings
  printf("TSND151 Get AD settings\n");
  tsnd->get_b_external_16bitAD(&ad_d);

  // Start
  printf("TSND151 計測開始\n");
  tsnd->set_b_start_freerun();

  while(1){
    bool isOk = tsnd->recv_enevt_process();

    TSND151_Event_t event = tsnd->pop_recv_event();

    if(event.event == NOTIFY_EXTERNAL_AD16_DATA){
      printf("%d %8d %8d %8d %8d\n", event.ex_ad.time, event.ex_ad.ch1, event.ex_ad.ch2, event.ex_ad.ch3, event.ex_ad.ch4);    
    }    
  }
  return 0;
}
