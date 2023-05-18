#ifndef __SERIAL_TSND151_TSND151_HPP_
#define __SERIAL_TSND151_TSND151_HPP_

#include <mutex>
#include <stdint.h>
#include <time.h>

#include "serial.hpp"
#include "packet.hpp"

namespace SerialTSND151{
  class TSND151{
  private:
    Serial *device_;
    
    std::mutex m_;
    
    void loop();
        
  public:
    void begin(const char* port_name);
    void end();

    bool start();
    bool stop();   
    bool reserve(struct tm start_datetime, uint16_t start_msec, struct tm end_datetime, uint16_t end_msec);

    bool getReservation(bool &enabled, struct tm &start_datetime, uint16_t &start_msec, struct tm &end_datetime, uint16_t &end_msec);        
    bool getInformation(uint8_t& sn, uint8_t& ble_addr, uint8_t& version, uint8_t& product);    
    bool setTime(struct tm datetime, unsigned int msec);
    bool getTime(struct tm &datetime, unsigned int &msec);    
    bool setGyroAccMeasuringSetting(uint8_t  measure_cycle, uint8_t  send_cycle, uint8_t  record_cycle);
    bool getGyroAccMeasuringSetting(uint8_t &measure_cycle, uint8_t &send_cycle, uint8_t &record_cycle);
    bool setGeoMeasuringSetting(uint8_t  measure_cycle, uint8_t  send_cycle, uint8_t  record_cycle);
    bool getGeoMeasuringSetting(uint8_t &measure_cycle, uint8_t &send_cycle, uint8_t &record_cycle);
    bool setAtmMeasuringSetting(uint8_t  measure_cycle, uint8_t  send_cycle, uint8_t  record_cycle);
    bool getAtmMeasuringSetting(uint8_t &measure_cycle, uint8_t &send_cycle, uint8_t &record_cycle);
    bool setBatteryVoltageMeasuringSetting(uint8_t  send_cycle, uint8_t  record_cycle);
    bool getBatteryVoltageMeasuringSetting(uint8_t &send_cycle, uint8_t &record_cycle);
    bool setExtendTerminalMeasuringEdgeSetting(uint8_t  measure_cycle, uint8_t  send_cycle, uint8_t  record_cycle, bool  send_ext_edge, bool  record_ext_edge);
    bool getExtendTerminalMeasuringEdgeSetting(uint8_t &measure_cycle, uint8_t &send_cycle, uint8_t &record_cycle, bool &send_ext_edge, bool &record_ext_edge);
    bool setExtendI2CSetting(uint8_t  measure_cycle, uint8_t  send_cycle, uint8_t  record_cycle);
    bool getExtendI2CSetting(uint8_t &measure_cycle, uint8_t &send_cycle, uint8_t &record_cycle);
    bool setAccRangeSetting(uint8_t  range);
    bool getAccRangeSetting(uint8_t &range);
    bool setAccCorrectSetting(uint8_t  mode_x, uint8_t  mode_y, uint8_t  mode_z, int32_t  x, int32_t  y, int32_t  z);
    bool setGyroRangeSetting(uint8_t range);
    bool getGyroRangeSetting(uint8_t &range);
    
    
    
  
    
  };
};
#endif
