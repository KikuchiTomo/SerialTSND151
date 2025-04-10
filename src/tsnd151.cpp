#include "tsnd151.hpp"
#include "packet.hpp"

#define __SERIAL_TSND151_CODING
#ifdef __SERIAL_TSND151_CODING
#include "../include/packet.hpp"
#include "../include/tsnd151.hpp"
#include "event_types.hpp"
#include "response_types.hpp"
#endif

SerialTSND151::TSND151::TSND151(int rv_buf_size) {
    sd_buffer_ = (uint8_t *)calloc(sizeof(uint8_t), rv_buf_size);
    sd_params_ = (uint8_t *)calloc(sizeof(uint8_t), rv_buf_size);
    rv_buffer_ = (uint8_t *)calloc(sizeof(uint8_t), rv_buf_size);

    ev_datas_ = new RingDeque<Event::Event_t>(
        __SERIAL_TSND151_DEFAULT_BUFFER_SIZE, Event::Default);
    rs_datas_ = new RingDeque<Response::Response_t>(
        __SERIAL_TSND151_DEFAULT_BUFFER_SIZE, Response::Default);

    isMeasuring_ = false;
    willStop_ = false;
}

SerialTSND151::TSND151::~TSND151() {
    free(sd_buffer_);
    free(sd_params_);
    free(rv_buffer_);

    delete ev_datas_;
    delete rs_datas_;
}

bool SerialTSND151::TSND151::begin(const char *port_name, int baud_rate) {
    device_ = new SerialTSND151::Serial(__SERIAL_TSND151_DEFAULT_BUFFER_SIZE);
    device_->setPort(port_name);
    device_->begin(baud_rate);
    sd_index_ = 0;
    rv_index_ = 0;

    int ret = pthread_create(&thread_, NULL, TSND151::_thread_task, this);
    if (ret != 0) {
        return false;
    }

    return true;
}

bool SerialTSND151::TSND151::end() {
    willStop_ = true;
    pthread_cancel(thread_);
    int ret = pthread_join(thread_, NULL);
    if (ret != 0) {
        return false;
    }

    device_->end();
    delete device_;
    return true;
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

void SerialTSND151::TSND151::_create_sd_packet(uint8_t *buf, const uint8_t cmd,
                                               const uint8_t *params,
                                               int params_len) {
    sd_m_.lock();

    int len = 0;
    sd_buffer_[len++] = Packet::PREFIX;
    sd_buffer_[len++] = cmd;
    for (int i = 0; i < params_len; i++) {
        sd_buffer_[len++] = params[i];
    }
    sd_buffer_[len++] = _calc_bcc(sd_buffer_, len);
    // for send thread
    sd_index_ = len;

    sd_m_.unlock();
}

SerialTSND151::Response::Response_t SerialTSND151::TSND151::get_info_b() {
    // if (isMeasuring_)
    //     return Response::Default;
    rs_m_.lock();
    // params
    sd_params_[0] = 0x00;
    // create
    _create_sd_packet(sd_buffer_, Packet::Command::Get::TSND151_INFO.hex,
                      sd_params_, 1);

    printf("%d\n", sd_index_);
    int rs_num0 = rs_datas_->getBufferLength();
    int rs_num1 = rs_num0;
    device_->setSendData(sd_buffer_, sd_index_);

    while (rs_num0 == rs_num1) {
        sleep(1);
        printf("%d %d\n", rs_num0, rs_num1);
        rs_num1 = rs_datas_->getBufferLength();
    }

    Response::Response_t res = rs_datas_->popl();
    rs_m_.unlock();
    return res;
}

void SerialTSND151::TSND151::_parse(Packet::receive_t cmd,
                                    const uint8_t *rv_buf, uint16_t rv_idx,
                                    bool check) {
    uint8_t h = cmd.hex;

    Response::Response_t response = Response::Default;
    response.Validation = check;

    Event::Event_t event = Event::Default;
    event.Validation = check;

    if (h == Packet::Response::CMD_RESPONSE_INFO.hex) {
        bool status = (rv_buf[0] == 1);
        response.Type = Response::ResponseType::ResponseCommand;
        response.Success = status;
        rs_datas_->push(response);
    } else if (h == Packet::Response::TSND151_INFO.hex) {
        printf("Response: %02X\n", h);
        response.Type = Response::ResponseType::ResponseGetInfo;
        _copy_buf((unsigned char *)response.Info.SerialNumber, rv_buf, 10);
        _copy_buf(response.Info.BluetoothAddress, rv_buf, 6, 10);
        _copy_buf(response.Info.Version, rv_buf, 4, 16);
        _copy_buf((unsigned char *)response.Info.Type, rv_buf, 10, 20);
        rs_datas_->push(response);
    } else if (h == Packet::Response::TIME_INFO.hex) {
        int idx = 0;
        response.Type = Response::ResponseType::ResponseGetTime;
        response.Time.year = rv_buf[idx++] + 2000;
        response.Time.mon = rv_buf[idx++];
        response.Time.day = rv_buf[idx++];
        response.Time.hour = rv_buf[idx++];
        response.Time.min = rv_buf[idx++];
        response.Time.sec = rv_buf[idx++];
        response.Time.msec = (rv_buf[idx++] << 8) | (rv_buf[idx++]);
        rs_datas_->push(response);
    } else if (h == Packet::Response::MEASURING_SETTING_INFO.hex) {
        int idx = 0;
        bool enable_reserve = (rv_buf[0] == 1);
        response.Type = Response::ResponseType::ResponseGetMeasureTime;
        response.MeasureTime.start.year = rv_buf[idx++] + 2000;
        response.MeasureTime.start.mon = rv_buf[idx++];
        response.MeasureTime.start.day = rv_buf[idx++];
        response.MeasureTime.start.hour = rv_buf[idx++];
        response.MeasureTime.start.min = rv_buf[idx++];
        response.MeasureTime.start.sec = rv_buf[idx++];
        response.MeasureTime.end.year = rv_buf[idx++] + 2000;
        response.MeasureTime.end.mon = rv_buf[idx++];
        response.MeasureTime.end.day = rv_buf[idx++];
        response.MeasureTime.end.hour = rv_buf[idx++];
        response.MeasureTime.end.min = rv_buf[idx++];
        response.MeasureTime.end.sec = rv_buf[idx++];
        rs_datas_->push(response);
    } else if (h == Packet::Response::ACC_SETTING_INFO.hex) {
        int idx = 0;
        response.Type = Response::ResponseType::ResponseGetAccAngSetting;
        response.AccAng.measureFreq = rv_buf[idx++];
        response.AccAng.sendFreq = rv_buf[idx++];
        response.AccAng.saveFreq = rv_buf[idx++];
        rs_datas_->push(response);
    } else if (h == Packet::Response::GEO_SETTING_INFO.hex) {
        int idx = 0;
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
    if (!thread_m_.try_lock()) {
        return;
    }

    while (willStop_ == false) {
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
                r_status_ = _recv_bcc(byte, (const uint8_t *)rv_buffer_,
                                      rv_index_, check);
                _parse(rv_cmd_, (const uint8_t *)rv_buffer_, rv_index_, check);
            }
            default:
                break;
            }
        }

        // Send loop
        device_->send();
    } // while

    thread_m_.unlock();
}
