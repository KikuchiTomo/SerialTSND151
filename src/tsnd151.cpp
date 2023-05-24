#include "tsnd151.hpp"
#include "packet.hpp"

#define __SERIAL_TSND151_CODING
#ifdef __SERIAL_TSND151_CODING
#include "types.hpp"
#endif

SerialTSND151::TSND151::TSND151(int rv_buf_size) {
    sd_buffer_ = (uint8_t *)calloc(sizeof(uint8_t), rv_buf_size);
    rv_buffer_ = (uint8_t *)calloc(sizeof(uint8_t), rv_buf_size);

    rv_datas_ = new RingDeque<Event_t>(__SERIAL_TSND151_DEFAULT_BUFFER_SIZE,
                                       EventDefault);
}

SerialTSND151::TSND151::~TSND151() {
    free(sd_buffer_);
    free(rv_buffer_);
}

void SerialTSND151::TSND151::begin(const char *port_name, int baud_rate) {
    device_ = new SerialTSND151::Serial(__SERIAL_TSND151_DEFAULT_BUFFER_SIZE);
    device_->setPort(port_name);
    device_->begin(baud_rate);
    sd_index_ = 0;
    rv_index_ = 0;
}

void SerialTSND151::TSND151::end() {
    device_->end();
    delete device_;
}

bool SerialTSND151::TSND151::start() { return true; }

bool SerialTSND151::TSND151::stop() { return true; }

SerialTSND151::TSND151::R_Status
SerialTSND151::TSND151::_recv_wait(uint8_t byte) {
    if (byte == Packet::PREFIX) {
        return R_Status::R_W_CMD;
    }
    return R_Status::R_W_HEAD;
}

bool SerialTSND151::TSND151::_search_command(uint8_t byte,
                                             Packet::receive_t &cmd) {
    // Response
    for (auto it = Packet::Response::List.begin();
         it != Packet::Response::List.end(); it++) {
        if (it->hex == byte) {
            cmd = *it;
            return true;
        }
    }

    // Event
    for (auto it = Packet::Event::List.begin(); it != Packet::Event::List.end();
         it++) {
        if (it->hex == byte) {
            cmd = *it;
            return true;
        }
    }

    cmd = {0, 0};
    return false;
}

SerialTSND151::TSND151::R_Status
SerialTSND151::TSND151::_recv_cmd(uint8_t byte, Packet::receive_t &rv_cmd,
                                  uint16_t &rv_idx) {
    // Zero clear
    rv_cmd.hex = 0x00;
    rv_cmd.len = 0x00;
    rv_idx = 0;
    // Search response definition
    if (!_search_command(byte, rv_cmd))
        return R_Status::R_W_HEAD;
    return R_Status::R_W_PARAM;
}

SerialTSND151::TSND151::R_Status
SerialTSND151::TSND151::_recv_param(uint8_t byte, Packet::receive_t rv_cmd,
                                    uint8_t *rv_buf, uint16_t &rv_idx) {
    if (rv_cmd.len >= rv_idx) {
        return R_Status::R_W_BCC;
    } else {
        rv_buf[rv_idx++] = byte;
    }
    return R_Status::R_W_PARAM;
}

uint8_t SerialTSND151::TSND151::_calc_bcc(const uint8_t *buf, int len) {
    uint8_t bcc = buf[0];
    for (int i = 1; i < len; i++) {
        bcc ^= buf[i];
    }
    return bcc;
}

SerialTSND151::TSND151::R_Status
SerialTSND151::TSND151::_recv_bcc(uint8_t byte, const uint8_t *rv_buf,
                                  uint16_t rv_idx, bool &check) {
    uint8_t rv_bcc = byte;
    uint8_t cl_bcc = _calc_bcc(rv_buf, rv_idx);
    check = (rv_bcc == cl_bcc);
    return R_Status::R_W_HEAD;
}

void SerialTSND151::TSND151::_copy_buf(uint8_t *dst, const uint8_t *src,
                                       int len, int offset = 0) {
    for (int i = 0; i < 10; i++) {
        dst[i] = src[offset + i];
    }
}

void SerialTSND151::TSND151::_parse(Packet::receive_t cmd,
                                    const uint8_t *rv_buf, uint16_t rv_idx) {
    uint8_t h = cmd.hex;

    if (h == Packet::Response::CMD_RESPONSE_INFO.hex) {
        bool status = (rv_buf[0] == 1);
    } else if (h == Packet::Response::TSND151_INFO.hex) {
        uint8_t sn[10];
        uint8_t addr[6];
        uint8_t ver[4];
        uint8_t type[10];
        _copy_buf(sn, rv_buf, 10);
        _copy_buf(addr, rv_buf, 6, 10);
        _copy_buf(ver, rv_buf, 4, 16);
        _copy_buf(type, rv_buf, 10, 20);
    } else if (h == Packet::Response::TIME_INFO.hex) {
        uint16_t year, mon, day, hour, min, sec, msec;
        year = rv_buf[0] + 2000;
        mon = rv_buf[1];
        day = rv_buf[2];
        hour = rv_buf[3];
        min = rv_buf[4];
        sec = rv_buf[5];
        msec = (rv_buf[6] << 8) | (rv_buf[7]);
    } else if (h == Packet::Response::MEASURING_SETTING_INFO.hex) {
        bool enable_reserve = (rv_buf[0] == 1);
        uint16_t s_year, s_mon, s_day, s_hour, s_min, s_sec;
        uint16_t e_year, e_mon, e_day, e_hour, e_min, e_sec;

        int idx = 0;
        s_year = rv_buf[idx++] + 2000;
        s_mon = rv_buf[idx++];
        s_day = rv_buf[idx++];
        s_hour = rv_buf[idx++];
        s_min = rv_buf[idx++];
        s_sec = rv_buf[idx++];
        e_year = rv_buf[idx++] + 2000;
        e_mon = rv_buf[idx++];
        e_day = rv_buf[idx++];
        e_hour = rv_buf[idx++];
        e_min = rv_buf[idx++];
        e_sec = rv_buf[idx++];
    } else if (h == Packet::Response::ACC_SETTING_INFO.hex) {
        int idx = 0;
        uint8_t measure_freq = rv_buf[idx++];
        uint8_t send_freq = rv_buf[idx++];
        uint8_t save_freq = rv_buf[idx++];
    }
    /*/
    break;
case Packet::Response::GEO_SETTING_INFO.hex: {

} break;
case Packet::Response::ATM_SETTING_INFO.hex:
    break;
case Packet::Response::BATTERY_SETTING_INFO.hex:
    break;
case Packet::Response::EXTEND_TERMINAL_MEASURING_SETTING_INFO.hex:
    break;
case Packet::Response::EXTEND_I2C_SETTING_INFO.hex:
    break;
case Packet::Response::ACC_RANGE_SETTING_INFO.hex:
    break;
case Packet::Response::ANG_RANGE_SETTING_INFO.hex:
    break;
case Packet::Response::EXTEND_I2C_DEVICE_SETTING_INFO.hex:
    break;
case Packet::Response::EXTEND_I2C_TEST.hex:
    break;
case Packet::Response::OPTION_BUTTON_SETTING_INFO.hex:
    break;
case Packet::Response::OVERWRITE_MEMORY_SETTING_INFO.hex:
    break;
case Packet::Response::EXTEND_TERMINAL_SETTING_INFO.hex:
    break;
case Packet::Response::BUZZER_VOLUME_SETTING_INFO.hex:
    break;
case Packet::Response::MEASURED_ENTRY_NUM_INFO.hex:
    break;
case Packet::Response::MEASURED_ENTRY_INFO.hex:
    break;
case Packet::Response::MEASURED_ENTRY_DETAIL_INFOL.hex:
    break;
case Packet::Response::MEASURE_MEMORY_REMAIN_SIZE_INFO.hex:
    break;
case Packet::Response::BATTERY_STATUS_INFO.hex:
    break;
case Packet::Response::STATUS_INFO.hex:
    break;
case Packet::Response::ACC_OFFSET_INFO.hex:
    break;
case Packet::Response::AUTO_POWER_OFF_INFO.hex:
    break;
case Packet::Response::BLE_SETTING_INFO.hex:
    break;
case Packet::Response::BREAK_READING_MEMORY_INFO.hex:
    break;
case Packet::Event::ACC_ANG.hex:
    break;
case Packet::Event::GEO.hex:
    break;
case Packet::Event::ATM.hex:
    break;
case Packet::Event::BATTERY.hex:
    break;
case Packet::Event::EXTEND_TERMINAL.hex:
    break;
case Packet::Event::EDGE.hex:
    break;
case Packet::Event::EXTEND_IC2.hex:
    break;
case Packet::Event::ERROR.hex:
    break;
case Packet::Event::STARTED.hex:
    break;
case Packet::Event::ENDED.hex:
    break;
}
*/
}

void SerialTSND151::TSND151::_loop() {
    int available = 0;
    bool check = false;

    // Receive loop
    while ((available = device_->getRecvSize()) != 0) {
        uint8_t byte = device_->popRecvData();

        switch (r_status_) {
        case R_Status::R_W_HEAD:
            r_status_ = _recv_wait(byte);
            break;
        case R_Status::R_W_CMD:
            r_status_ = _recv_cmd(byte, rv_cmd_, rv_index_);
            break;
        case R_Status::R_W_PARAM:
            r_status_ = _recv_param(byte, rv_cmd_, rv_buffer_, rv_index_);
            break;
        case R_Status::R_W_BCC: {
            r_status_ =
                _recv_bcc(byte, (const uint8_t *)rv_buffer_, rv_index_, check);
            _parse(rv_cmd_, (const uint8_t *)rv_buffer_, rv_index_);
        }
        default:
            break;
        }
    }
}