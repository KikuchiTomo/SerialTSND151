#ifndef __SERIAL_TSND151_TSND151_HPP_
#define __SERIAL_TSND151_TSND151_HPP_

#include <mutex>
#include <stdint.h>
#include <time.h>

#include "packet.hpp"
#include "serial.hpp"
#include "types.hpp"

#define __SERIAL_TSND151_DEFAULT_BUFFER_SIZE (4096)
namespace SerialTSND151 {

class TSND151 {
  public:
    typedef enum {
        R_W_HEAD,
        R_W_CMD,
        R_W_PARAM,
        R_W_BCC,
    } R_Status;

    typedef enum {
        S_WAIT,
        S_SEND,
    } S_Status;

  private:
    Serial *device_;

    std::mutex m_;

    Packet::receive_t rv_cmd_;
    uint8_t *rv_buffer_;
    uint16_t rv_index_;

    uint8_t *sd_buffer_;
    uint16_t sd_index_;

    RingDeque<Event_t> *rv_datas_;

    R_Status r_status_ = R_W_HEAD;
    S_Status s_status_ = S_WAIT;

    void _loop();

    uint8_t _calc_bcc(const uint8_t *buf, int len);
    bool _search_command(uint8_t byte, Packet::receive_t &cmd);
    void _parse(Packet::receive_t cmd, const uint8_t *rv_buf, uint16_t rv_idx);
    R_Status _recv_wait(uint8_t byte);
    R_Status _recv_cmd(uint8_t byte, Packet::receive_t &rv_cmd,
                       uint16_t &rv_idx);
    R_Status _recv_param(uint8_t byte, Packet::receive_t rv_cmd,
                         uint8_t *rv_buf, uint16_t &rv_idx);
    R_Status _recv_bcc(uint8_t byte, const uint8_t *rv_buf, uint16_t rv_idx,
                       bool &check);
    void _copy_buf(uint8_t *dst, const uint8_t *src, int len, int offset);

  public:
    TSND151(int rv_buf_size = 4096);
    ~TSND151();

    void popReceivedData();

    void begin(const char *port_name, int baud_rate);
    void end();

    bool start();
    bool stop();
};
}; // namespace SerialTSND151
#endif
