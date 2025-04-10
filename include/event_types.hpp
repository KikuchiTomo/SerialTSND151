#ifndef __SERIAL_TSND151_TYPES_HPP_
#define __SERIAL_TSND151_TYPES_HPP_

#include "packet.hpp"

namespace SerialTSND151 {

typedef bool Level;
typedef bool Edge;
typedef enum { No, Pos, Neg } EdgeMode;

static Level HIGH = true;
static Level LOW = false;

namespace Event {
typedef enum {
    NoError = 0x00,
    AccOrAngVecSensorError = 0x80,
    GeoSensorError = 0x81,
    AtmSensorError = 0x82,
    I2CError = 0x86,
    QuaternionError = 0x8A,
    I2C2Error = 0x8B,
    ADError = 0x8C
} Error;

typedef enum {
    NotifyInvaild = 0x00,
    NotifyAccAndAngVec = SerialTSND151::Packet::Event::ACC_ANG.hex,
    NotifyGeo = SerialTSND151::Packet::Event::GEO.hex,
    NotifyAtm = SerialTSND151::Packet::Event::ATM.hex,
    NotifyBattery = SerialTSND151::Packet::Event::BATTERY.hex,
    NotifyExt = SerialTSND151::Packet::Event::EXTEND_TERMINAL.hex,
    NotifyEdge = SerialTSND151::Packet::Event::EDGE.hex,
    NotifyI2C = SerialTSND151::Packet::Event::EXTEND_IC2.hex,
    NotifyEror = SerialTSND151::Packet::Event::ERROR.hex,
    NotifyStarted = SerialTSND151::Packet::Event::STARTED.hex,
    NotifyEnded = SerialTSND151::Packet::Event::ENDED.hex,
    NotifyQuaternion = SerialTSND151::Packet::Event::QUATERNION.hex,
    NotifyI2C2 = SerialTSND151::Packet::Event::I2C2.hex,
    NotifyAD16 = SerialTSND151::Packet::Event::AD16.hex,
} EventType;

typedef struct {
    bool ena;
    double time;
    double x, y, z;
} AccVec_t;

typedef struct {
    bool ena;
    double time;
    double x, y, z;
} AngVec_t;

typedef struct {
    bool ena;
    double time;
    double x, y, z;
} Geo_t;

typedef struct {
    bool ena;
    double time;
    double atm;
    double temp;
} Atm_t;

typedef struct {
    bool ena;
    double time;
    double v;
    double remain;
} Battery_t;

typedef struct {
    bool ena;
    double time;
    Level ext1, ext2, ext3, ext4;
    uint16_t ad3, ad4;
} Ext_t;

typedef struct {
    bool ena;
    double time;
    bool error;
    uint8_t data[16];
} ExtI2C_t;

typedef struct {
    bool ena;
    double time;
    Error error;
} Error_t;

typedef struct {
    bool ena;
    double time;
    bool ext1, ext2, ext3, ext4;
    SerialTSND151::EdgeMode btn;
} Edge_t;

typedef struct {
    bool ena;
    double time;
    double w, x, y, z;
} Quaternion_t;

typedef struct {
    bool enab;
    double time;
    int16_t ad1, ad2, ad3, ad4;
} AD16bit_t;

typedef struct {
    bool Validation;
    EventType Type;
    AccVec_t Acc;
    AngVec_t Ang;
    Geo_t Geo;
    Atm_t Atm;
    Battery_t Battery;
    Ext_t Ext;
    ExtI2C_t I2C;
    ExtI2C_t I2C_2;
    Error_t Error;
    Edge_t Edge;
    Quaternion_t Qua;
    AD16bit_t AD16;
} Event_t;

static const Event_t Default = {
    .Validation = false,
    .Type = NotifyInvaild,
    .Acc = {0},
    .Ang = {0},
    .Geo = {0},
    .Atm = {0},
    .Battery = {0},
    .Ext = {0},
    .I2C = {0},
    .I2C_2 = {0},
    .Error.ena = false,
    .Error.error = NoError,
    .Error.time = 0,
    .Edge.btn = No,
    .Edge.ena = false,
    .Edge.ext1 = 0,
    .Edge.ext2 = 0,
    .Edge.ext3 = 0,
    .Edge.ext4 = 0,
    .Edge.time = 0,
    .Qua = {0},
    .AD16 = {0},
};

}; // namespace Event
}; // namespace SerialTSND151

#endif