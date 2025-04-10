#ifndef __SERIAL_TSND151_SERIAL_HPP_
#define __SERIAL_TSND151_SERIAL_HPP_

#include "buffer.hpp"
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define __SERIAL_TSND151_SERIAL_CS8 (0x0001)
#define __SERIAL_TSND151_SERIAL_CS7 (0x0002)
#define __SERIAL_TSND151_SERIAL_CS6 (0x0004)
#define __SERIAL_TSND151_SERIAL_CS5 (0x0008)
#define __SERIAL_TSND151_SERIAL_PARN (0x0010)
#define __SERIAL_TSND151_SERIAL_PARO (0x0020)
#define __SERIAL_TSND151_SERIAL_PARE (0x0040)
#define __SERIAL_TSND151_SERIAL_STP1 (0x0100)
#define __SERIAL_TSND151_SERIAL_STP2 (0x0200)

#define __SERIAL_TSND151_SERIAL_8N2                                            \
    (__SERIAL_TSND151_SERIAL_CS8 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_8N1                                            \
    (__SERIAL_TSND151_SERIAL_CS8 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_8O2                                            \
    (__SERIAL_TSND151_SERIAL_CS8 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_8O1                                            \
    (__SERIAL_TSND151_SERIAL_CS8 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_8E2                                            \
    (__SERIAL_TSND151_SERIAL_CS8 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_8E1                                            \
    (__SERIAL_TSND151_SERIAL_CS8 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP1)

#define __SERIAL_TSND151_SERIAL_7N2                                            \
    (__SERIAL_TSND151_SERIAL_CS7 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_7N1                                            \
    (__SERIAL_TSND151_SERIAL_CS7 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_7O2                                            \
    (__SERIAL_TSND151_SERIAL_CS7 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_7O1                                            \
    (__SERIAL_TSND151_SERIAL_CS7 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_7E2                                            \
    (__SERIAL_TSND151_SERIAL_CS7 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_7E1                                            \
    (__SERIAL_TSND151_SERIAL_CS7 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP1)

#define __SERIAL_TSND151_SERIAL_6N2                                            \
    (__SERIAL_TSND151_SERIAL_CS6 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_6N1                                            \
    (__SERIAL_TSND151_SERIAL_CS6 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_6O2                                            \
    (__SERIAL_TSND151_SERIAL_CS6 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_6O1                                            \
    (__SERIAL_TSND151_SERIAL_CS6 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_6E2                                            \
    (__SERIAL_TSND151_SERIAL_CS6 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_6E1                                            \
    (__SERIAL_TSND151_SERIAL_CS6 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP1)

#define __SERIAL_TSND151_SERIAL_5N2                                            \
    (__SERIAL_TSND151_SERIAL_CS5 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_5N1                                            \
    (__SERIAL_TSND151_SERIAL_CS5 | __SERIAL_TSND151_SERIAL_PARN |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_5O2                                            \
    (__SERIAL_TSND151_SERIAL_CS5 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_5O1                                            \
    (__SERIAL_TSND151_SERIAL_CS5 | __SERIAL_TSND151_SERIAL_PARO |              \
     __SERIAL_TSND151_SERIAL_STP1)
#define __SERIAL_TSND151_SERIAL_5E2                                            \
    (__SERIAL_TSND151_SERIAL_CS5 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP2)
#define __SERIAL_TSND151_SERIAL_5E1                                            \
    (__SERIAL_TSND151_SERIAL_CS5 | __SERIAL_TSND151_SERIAL_PARE |              \
     __SERIAL_TSND151_SERIAL_STP1)

#define __SERIAL_TSND151_DEFAULT_BUFFER_SIZE (512)

namespace SerialTSND151 {
class Serial {
  private:
    int baud_rate_;

    char config_;
    char *port_name_;

    int fd_;
    int rv_;

    RingDeque<char> *rb_;
    RingDeque<char> *sb_;

    char *tmp_rb_;
    char *tmp_sb_;

    long long int timeout_s_ = 0;
    long long int timeout_us_ = 0;

    fd_set set_;
    struct timeval timeout_ = {0, 0};

    std::mutex sm_, rm_;

    speed_t baudrate(int baudrate);

    void error(const char *format, ...);
    void info(const char *format, ...);

  public:
    Serial(long long int bufferSize = __SERIAL_TSND151_DEFAULT_BUFFER_SIZE);
    ~Serial();

    void setPort(const char *port_name);
    void setTimeout(long long int sec, long long int usec);

    bool begin(int baud_rate,
               unsigned char config = __SERIAL_TSND151_SERIAL_8E1);
    void end();

    unsigned char popRecvData();
    unsigned char readRecvData(int offset = 0);

    void setSendData(unsigned char data);
    void setSendData(unsigned char *data, int len);
    int getRecvSize();
    int getSendSize();

    void clearSendBuffer();
    void clearRecvBuffer();

    void recv(bool b = false);
    void send(bool b = false);
};
}; // namespace SerialTSND151

#endif
