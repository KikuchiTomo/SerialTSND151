#ifndef __SERIAL_TSND151_PACKET_HPP_
#define __SERIAL_TSND151_PACKET_HPP_

#include <vector>

namespace SerialTSND151 {
namespace Packet {
static uint8_t PREFIX = 0x9A;
typedef struct {
    uint8_t hex;
    uint16_t len;
} receive_t;

typedef struct {
    uint8_t hex;
    bool measuringAvailable;
} command_t;

namespace Command {
namespace Set {
static const command_t TIME_SETTING = {0x11, false};
static const command_t ACC_MEASURE_SETTING = {0x16, false};
static const command_t GEO_MEASURE_SETTING = {0x18, false};
static const command_t ATM_MEASURE_SETTING = {0x1A, false};
static const command_t BATTRY_MEASURE_SETTING = {0x1C, false};
static const command_t EXTEND_MEASURE_SETTING = {0x1E, false};
static const command_t I2C_SETTING = {0x20, false};
static const command_t ACC_RANGE_SETTING = {0x22, false};
static const command_t ANG_RANGE_SETTING = {0x24, false};
static const command_t ACC_CORRECT_SETTING = {0x24, false};
static const command_t EXTEND_I2C_SETTING = {0x29, false};
static const command_t EXTEND_I2C_SETTING2 = {0x57, false};
static const command_t OPTION_BUTTON_SETTING = {0x2C, false};
static const command_t OVERWRITE_MEMORY_SETTING = {0x2E, false};
static const command_t EXTEND_TERMINAL_SETTING = {0x30, true};
static const command_t BUZZER_VOLUME_SETTING = {0x32, false};
static const command_t AUTO_POWER_OFF_SETTING = {0x50, false};
static const command_t BLE_SETTING = {0x52, false};
static const command_t QUATERNINON_SETTING = {0x55, false};
}; // namespace Set

namespace Get {
static const command_t TSND151_INFO = {0x10, false};
static const command_t TIME_INFO = {0x12, false};
static const command_t MEASURE_RESERVE_INFO = {0x14, false};
static const command_t ACC_MEASURE_INFO = {0x17, false};
static const command_t GEO_MEASURE_INFO = {0x19, false};
static const command_t ATM_MEASURE_INFO = {0x1B, false};
static const command_t BATTERY_MEASURE_INFO = {0x1D, false};
static const command_t EXTEND_MEASURE_INFO = {0x1F, false};
static const command_t I2C_SETTING_INFO = {0x21, false};
static const command_t ACC_RANGE_INFO = {0x23, false};
static const command_t ANG_RANGE_INFO = {0x35, false};
static const command_t EXTEND_I2C_SETTING_INFO = {0x2A, false};
static const command_t EXTEND_I2C_SETTING2_INFO = {0x58, false};
static const command_t OPTION_BUTTON_INFO = {0x2D, false};
static const command_t OVERWRITE_MEMORY_INFO = {0x2F, false};
static const command_t EXTEND_TERMINAL_INFO = {0x31, true};
static const command_t BUZZER_VOLUME_INFO = {0x33, false};
static const command_t MEASURED_ENTRY_LENGTH_INFO = {0x37, false};
static const command_t MEASURED_ENTRY_DETAIL_INFO = {0x38, false};
static const command_t MEASURE_MEMORY_REMAIN_SIZE_INFO = {0x3A, false};
static const command_t BATTERY_STATUS_INFO = {0x3B, false};
static const command_t STATUS_INFO = {0x3C, true};
static const command_t ACC_OFFSET_INFO = {0x3D, false};
static const command_t RAD_ACC_OFFSET_INFO = {0x3E, false};
static const command_t AUTO_POWER_OFF_INFO = {0x51, false};
static const command_t BLE_SETTING_INFO = {0x53, false};
static const command_t QUATERNINON_SETTING_INFO = {0x55, false};
static const std::vector<command_t> List = {
    TSND151_INFO,
    TIME_INFO,
    MEASURE_RESERVE_INFO,
    ACC_MEASURE_INFO,
    GEO_MEASURE_INFO,
    ATM_MEASURE_INFO,
    BATTERY_MEASURE_INFO,
    EXTEND_MEASURE_INFO,
    I2C_SETTING_INFO,
    ACC_RANGE_INFO,
    ANG_RANGE_INFO,
    EXTEND_I2C_SETTING_INFO,
    EXTEND_I2C_SETTING2_INFO,
    OPTION_BUTTON_INFO,
    OVERWRITE_MEMORY_INFO,
    EXTEND_TERMINAL_INFO,
    BUZZER_VOLUME_INFO,
    MEASURED_ENTRY_LENGTH_INFO,
    MEASURED_ENTRY_DETAIL_INFO,
    MEASURE_MEMORY_REMAIN_SIZE_INFO,
    BATTERY_STATUS_INFO,
    STATUS_INFO,
    ACC_OFFSET_INFO,
    RAD_ACC_OFFSET_INFO,
    AUTO_POWER_OFF_INFO,
    BLE_SETTING_INFO,
    QUATERNINON_SETTING_INFO,
};
}; // namespace Get

namespace Execute {
static command_t START_MEASURE = {0x13, false};
static command_t STOP_MEASURE = {0x15, true};
static command_t CALIBRATION_GEO = {0x28, false};
static command_t TEST_I2C = {0x2B, false};
static command_t CHIME_BUZZER = {0x34, true};
static command_t CLEAR_MEASURED_DATA = {0x35, false};
static command_t READ_MEASURED_DATA_MEMORY = {0x39, false};
static command_t RESET_SETTINGS = {0x3F, false};
static command_t BREAK_READING_MEMORY = {0x54, false};
}; // namespace Execute
}; // namespace Command

namespace Response {
static const receive_t CMD_RESPONSE_INFO = {0x8F, 1};
static const receive_t TSND151_INFO = {0x90, 30};
static const receive_t TIME_INFO = {0x92, 8};
static const receive_t MEASURING_SETTING_INFO = {0x93, 13};
static const receive_t ACC_SETTING_INFO = {0x97, 3};
static const receive_t GEO_SETTING_INFO = {0x99, 3};
static const receive_t ATM_SETTING_INFO = {0x9B, 3};
static const receive_t BATTERY_SETTING_INFO = {0x9D, 2};
static const receive_t EXTEND_TERMINAL_MEASURING_SETTING_INFO = {0x9F, 5};
static const receive_t EXTEND_I2C_SETTING_INFO = {0xA1, 3};
static const receive_t ACC_RANGE_SETTING_INFO = {0xA3, 1};
static const receive_t ANG_RANGE_SETTING_INFO = {0xA6, 1};
static const receive_t EXTEND_I2C_DEVICE_SETTING_INFO = {0xAA, 12};
static const receive_t EXTEND_I2C_TEST = {0xAB, 9};
static const receive_t OPTION_BUTTON_SETTING_INFO = {0xAD, 1};
static const receive_t OVERWRITE_MEMORY_SETTING_INFO = {0xAF, 1};
static const receive_t EXTEND_TERMINAL_SETTING_INFO = {0xB1, 4};
static const receive_t BUZZER_VOLUME_SETTING_INFO = {0xB3, 1};
static const receive_t MEASURED_ENTRY_NUM_INFO = {0xB6, 1};
static const receive_t MEASURED_ENTRY_INFO = {0xB7, 24};
static const receive_t MEASURED_ENTRY_DETAIL_INFO = {0xB8, 60};
static const receive_t MEASURE_MEMORY_REMAIN_SIZE_INFO = {0xBA, 5};
static const receive_t BATTERY_STATUS_INFO = {0xBB, 3};
static const receive_t STATUS_INFO = {0xBC, 1};
static const receive_t ACC_OFFSET_INFO = {0xBD, 12};
static const receive_t AUTO_POWER_OFF_INFO = {0xD1, 1};
static const receive_t BLE_SETTING_INFO = {0x53, 1};
static const receive_t BREAK_READING_MEMORY_INFO = {0xB9, 1};
static const std::vector<receive_t> List = {
    CMD_RESPONSE_INFO,
    TSND151_INFO,
    TIME_INFO,
    MEASURING_SETTING_INFO,
    ACC_SETTING_INFO,
    GEO_SETTING_INFO,
    ATM_SETTING_INFO,
    BATTERY_SETTING_INFO,
    EXTEND_TERMINAL_MEASURING_SETTING_INFO,
    EXTEND_I2C_SETTING_INFO,
    ACC_RANGE_SETTING_INFO,
    ANG_RANGE_SETTING_INFO,
    EXTEND_I2C_DEVICE_SETTING_INFO,
    EXTEND_I2C_TEST,
    OPTION_BUTTON_SETTING_INFO,
    OVERWRITE_MEMORY_SETTING_INFO,
    EXTEND_TERMINAL_SETTING_INFO,
    BUZZER_VOLUME_SETTING_INFO,
    MEASURED_ENTRY_NUM_INFO,
    MEASURED_ENTRY_INFO,
    MEASURED_ENTRY_DETAIL_INFO,
    MEASURE_MEMORY_REMAIN_SIZE_INFO,
    BATTERY_STATUS_INFO,
    STATUS_INFO,
    ACC_OFFSET_INFO,
    AUTO_POWER_OFF_INFO,
    BLE_SETTING_INFO,
    BREAK_READING_MEMORY_INFO,
};
}; // namespace Response

namespace Event {
static const receive_t ACC_ANG = {0x80, 22};
static const receive_t GEO = {0x81, 13};
static const receive_t ATM = {0x82, 9};
static const receive_t BATTERY = {0x83, 7};
static const receive_t EXTEND_TERMINAL = {0x84, 9};
static const receive_t EDGE = {0x85, 6};
static const receive_t EXTEND_IC2 = {0x86, 13};
static const receive_t ERROR = {0x87, 5};
static const receive_t STARTED = {0x88, 1};
static const receive_t ENDED = {0x89, 1};
static const receive_t I2C2 = {0x8B, 22};
static const receive_t QUATERNION = {0x8A, 30};
static const receive_t AD16 = {0x89, 12};
static const std::vector<receive_t> List = {
    ACC_ANG, GEO,     ATM,   BATTERY, EXTEND_TERMINAL, EDGE, EXTEND_IC2,
    ERROR,   STARTED, ENDED, I2C2,    QUATERNION,      AD16};
}; // namespace Event
}; // namespace Packet
}; // namespace SerialTSND151

#endif
