#ifndef __SERIAL_TSND151_PACKET_HPP_
#define __SERIAL_TSND151_PACKET_HPP_

namespace SerialTSND151{

  namespace Packet{
    typedef unsigned char uchar;       
    uchar PREFIX = 0x9A;
    
    namespace Command{
      typedef struct{
        uchar hex;
        bool measuringAvailable;
      }command_t;

      namespace Set{
        command_t TIME_INFO = {0x11, false};
        command_t ACCMEASURE__INFO = {0x16, false};
        command_t GEO_MEASURE_INFO = {0x18, false};
        command_t ATM_MEASURE_INFO = {0x1A, false};
        command_t BATTRY_MEASURE_INFO = {0x1C, false};
        command_t EXTEND_MEASURE_INFO = {0x1E, false};
        command_t I2C_SETTING_INFO = {0x20, false};
        command_t ACC_RANGE_INFO = {0x22, false};
        command_t ACC_CORRECT_INFO = {0x24, false};
        command_t EXTEND_I2C_SETTING = {0x29, false};
        command_t OPTION_BUTTON_SETTING = {0x2C, false};
        command_t OVERWRITE_MEMORY_SETTING= {0x2E, false};
        command_t EXTEND_TERMINAL_SETTING = {0x30, true};
        command_t BUZZER_VOLUME_SETTING = {0x32, false};
        command_t AUTO_POWER_OFF_SETTING  = {0x50, false};
        command_t BLE_SETTING = {0x52, false};                
      };
      
      namespace Get{
        command_t TSND151_INFO = {0x10, false};
        command_t TIME_INFO = {0x12, false};
        command_t MEASURE_RESERVE_INFO = {0x14, false};
        command_t ACC_MEASURE_INFO = {0x17, false};
        command_t GEO_MEASURE_INFO = {0x19, false};
        command_t ATM_MEASURE_INFO = {0x1B, false};
        command_t BATTERY_MEASURE_INFO = {0x1D, false};
        command_t EXTEND_MEASURE_INFO = {0x1F, false};
        command_t I2C_SETTING_INFO = {0x21, false};
        command_t ACC_RANGE_INFO = {0x23, false};
        command_t EXTEND_I2C_SETTING_INFO = {0x2A, false};
        command_t OPTION_BUTTON_SETTING = {0x2D, false};        
        command_t OVERWRITE_MEMORY_SETTING= {0x2F, false};
        command_t EXTEND_TERMINAL_SETTING = {0x31, true};
        command_t BUZZER_VOLUME_SETTING = {0x33, false};
        command_t MEASURED_ENTRY_LENGTH = {0x37, false};
        command_t MEASURED_ENTRY_DETAIL = {0x38, false};
        command_t MEASURE_MEMORY_REMAIN_SIZE = {0x3A, false};
        command_t BATTERY_STATUS = {0x3B, false};
        command_t STATUS = {0x3C, true};
        command_t ACC_OFFSET = {0x3D, false};
        command_t RAD_ACC_OFFSET = {0x3E, false};
        command_t AUTO_POWER_OFF_SETTING = {0x51, false};
        command_t BLE_SETTING = {0x53, false};
      };           
      
      namespace Execute{
        command_t START_MEASURE = {0x13, false};
        command_t STOP_MEASURE = {0x15, true};
        command_t CALIBRATION_GEO = {0x28, false};
        command_t TEST_I2C = {0x2B, false};
        command_t CHIME_BUZZER = {0x34, true};
        command_t CLEAR_MEASURED_DATA = {0x35, false};
        command_t READ_MEASURED_DATA_MEMORY = {0x39, false};
        command_t RESET_SETTINGS = {0x3F, false};
        command_t BREAK_READING_MEMORY = {0x54, false};        
      };           
    };

    namespace Response{
      
    };

    namespace Event{
      typedef
    };
  };
};

#endif
