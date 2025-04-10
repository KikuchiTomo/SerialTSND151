#ifndef __SERIAL_TSND151_TSND151_HPP_
#define __SERIAL_TSND151_TSND151_HPP_

#include <mutex>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#include "event_types.hpp"
#include "packet.hpp"
#include "response_types.hpp"
#include "serial.hpp"

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
    pthread_t thread_;
    bool willStop_;
    bool isMeasuring_;
    Serial *device_;

    std::mutex thread_m_;
    std::mutex rs_m_;
    std::mutex ev_m_;
    std::mutex sd_m_;

    Packet::receive_t rv_cmd_;
    uint8_t *rv_buffer_;
    uint16_t rv_index_;

    uint8_t *sd_buffer_;
    uint8_t *sd_params_;
    uint16_t sd_index_;

    RingDeque<Event::Event_t> *ev_datas_;
    RingDeque<Response::Response_t> *rs_datas_;

    R_Status r_status_ = R_W_HEAD;
    S_Status s_status_ = S_WAIT;

    void _loop();
    static void *_thread_task(void *param) {
        reinterpret_cast<TSND151 *>(param)->_loop();
    }

    void _create_sd_packet(uint8_t *buf, const uint8_t cmd,
                           const uint8_t *params, int params_len);
    uint8_t _calc_bcc(const uint8_t *buf, int len);
    bool _search_command(uint8_t byte, Packet::receive_t &cmd);
    void _parse(Packet::receive_t cmd, const uint8_t *rv_buf, uint16_t rv_idx,
                bool check);
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

    void pop_received_data(Response::Response_t *response,
                           Event::Event_t *event);

    bool begin(const char *port_name, int baud_rate = 115200);
    bool end();

    bool start();
    bool stop();

    Response::Response_t get_info_b();
};
}; // namespace SerialTSND151
#endif
