#ifndef __SERIAL_TSND151_RESPONSE_TYPES_HPP_
#define __SERIAL_TSND151_RESPONSE_TYPES_HPP_

namespace SerialTSND151 {
namespace Response {
typedef enum {
    Undefined,
    NoResponse,
    ResponseCommand,
    ResponseGetInfo,
    ResponseGetTime,
    ResponseGetMeasureTime,
    ResponseGetAccAngSetting,
    ResponseGetGeoSetting,
    ResponseGetAtmSetting,
    ResponseGetBatterySetting,
    ResponseGetExtAndEdgeSetting,
    ResponseGetI2CSetting,
    ResponseGetAccRangeSetting,
    ResponseGetAngRangeSetting,
    ResponseGetI2CDeviceSetting,
    ResponseTestI2C,
    ResponseGetButtonSetting,
    ResponseGetOverwriteMemorySetting,
    ResponseGetExtSetting,
    ResponseGetBuzzerVolumeSetting,
    ResponseGetEntryNumber,
    ResponseGetEntry,
    ResponseGetEntryDetail,
    ResponseGetEntryReading,
    ResponseGetMemoryRemainSize,
    ResponseGetBatteryStatus,
    ResponseGetStatus,
    ResponseGetAccOffset,
    ResponseGetAngOffset,
    ResponseGetAutoPowerOffSetting,
    ResponseGetBLESetting,
    ResponseGetQuaternionSetting,
    ResponseGetI2C2Setting,
    ResponseGetAD16bitSetting,
    ResponseGetEntry2,
    ResponseGetCanSave,
} ResponseType;

typedef struct {
    char SerialNumber[10];
    uint8_t BluetoothAddress[6];
    uint8_t Version[4];
    char Type[10];
} Info_t;

typedef struct {
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
    int msec;
} Time_t;

typedef struct {
    bool reserve;
    Time_t start;
    Time_t end;
} MesureTime_t;

typedef struct {
    uint8_t measureFreq;
    uint8_t sendFreq;
    uint8_t saveFreq;
} AccAngMeasure_t;

typedef struct {
    bool Validation;
    ResponseType Type;
    bool Success;
    Info_t Info;
    Time_t Time;
    AccAngMeasure_t AccAng;
    MesureTime_t MeasureTime;
} Response_t;

const static Response_t Default = {
    .Validation = false,
    .Type = Undefined,
    .Success = false,
    .Info = {0},
    .Time = {0},
    .MeasureTime = {0},
};

}; // namespace Response
}; // namespace SerialTSND151
#endif