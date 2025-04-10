#include "serial.hpp"

speed_t SerialTSND151::Serial::baudrate(int baud_rate) {
    switch (baud_rate) {
    case 9600:
        return B9600;
        break;
    case 19200:
        return B19200;
        break;
    case 38400:
        return B38400;
        break;
    case 57600:
        return B57600;
        break;
    case 115200:
        return B115200;
        break;
    case 230400:
        return B230400;
        break;
    default:
        std::cerr << "fail to convert baud rate. bye." << std::endl;
        exit(-1);
        break;
    }
}

SerialTSND151::Serial::Serial(long long int bufferSize) {

    rb_ = new SerialTSND151::RingDeque<char>(bufferSize, 0);
    sb_ = new SerialTSND151::RingDeque<char>(bufferSize, 0);

    tmp_rb_ = (char *)calloc(sizeof(char), bufferSize);
    tmp_sb_ = (char *)calloc(sizeof(char), bufferSize);

    port_name_ = (char *)malloc(sizeof(char) * 512);
}

void SerialTSND151::Serial::error(const char *format, ...) {
    fprintf(stderr, "[ERROR] %s : \x1b[31m", "Serial");

    va_list va;
    va_start(va, format);
    vprintf(format, va);
    va_end(va);

    fprintf(stderr, "\x1b[39m");
}

void SerialTSND151::Serial::info(const char *format, ...) {
    printf("[INFO] %s : \x1b[32m", "Serial");

    va_list va;
    va_start(va, format);
    vprintf(format, va);
    va_end(va);

    printf("\x1b[39m");
}

SerialTSND151::Serial::~Serial() {
    delete rb_;
    delete sb_;

    free(tmp_rb_);
    free(tmp_sb_);
}

void SerialTSND151::Serial::setPort(const char *port_name) {
    strcpy(port_name_, port_name);
}

void SerialTSND151::Serial::setTimeout(long long int sec, long long int usec) {
    timeout_s_ = sec;
    timeout_us_ = usec;
}

bool SerialTSND151::Serial::begin(int baud_rate, unsigned char config) {
    // Open
    fd_ = open(port_name_, O_RDWR | O_NONBLOCK);
    struct termios tio;

    if (fd_ < 0) {
        error("Cannot open device (%s)\n", port_name_);
        return false;
    }

    if (tcgetattr(fd_, &tio) < 0) {
        error("Cannot get terminal attributes\n");
        return false;
    }

    cfsetspeed(&tio, baudrate(baud_rate));

    // Set flags
    tio.c_cflag &= ~CSIZE;
    if (config & __SERIAL_TSND151_SERIAL_CS5)
        tio.c_cflag |= CS5;
    else if (config & __SERIAL_TSND151_SERIAL_CS6)
        tio.c_cflag |= CS6;
    else if (config & __SERIAL_TSND151_SERIAL_CS7)
        tio.c_cflag |= CS7;
    else
        tio.c_cflag |= CS8;

    if (config & __SERIAL_TSND151_SERIAL_PARN) {
        tio.c_cflag &= ~PARENB;
    } else if (config & __SERIAL_TSND151_SERIAL_PARO) {
        tio.c_cflag |= PARENB;
        tio.c_cflag |= PARODD;
    } else {
        tio.c_cflag |= PARENB;
        tio.c_cflag &= ~PARODD;
    }

    if (config & __SERIAL_TSND151_SERIAL_STP1)
        tio.c_cflag &= ~CSTOPB;
    else
        tio.c_cflag |= CSTOPB;

    tio.c_cflag &= ~CRTSCTS;
    tio.c_cflag |= CREAD | CLOCAL;
    tio.c_iflag &= ~(IXON | IXOFF | IXANY);

    tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tio.c_oflag &= ~OPOST;
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 0;

    tcsetattr(fd_, TCSANOW, &tio);

    if (tcsetattr(fd_, TCSAFLUSH, &tio) < 0) {
        error("Cannot get terminal attributes\n");
        return false;
    }

    info("Opened serial port. %s", port_name_);
    return true;
}

void SerialTSND151::Serial::end() {
    info("Closed serial port. %s", port_name_);
    close(fd_);
}

unsigned char SerialTSND151::Serial::popRecvData() { return rb_->popl(); }

unsigned char SerialTSND151::Serial::readRecvData(int offset) {
    return rb_->readl(offset);
}

int SerialTSND151::Serial::getRecvSize() { return rb_->getBufferLength(); }

int SerialTSND151::Serial::getSendSize() { return sb_->getBufferLength(); }

void SerialTSND151::Serial::setSendData(unsigned char *data, int len) {
    if (!sm_.try_lock()) {
        return;
    }
    for (int i = 0; i < len; i++) {
        rb_->push(data[i]);
    }
    sm_.unlock();
}

void SerialTSND151::Serial::setSendData(unsigned char data) {
    if (!sm_.try_lock()) {
        return;
    }
    rb_->push(data);
    sm_.unlock();
}

void SerialTSND151::Serial::clearSendBuffer() {
    if (!sm_.try_lock()) {
        return;
    }
    sb_->resetIndexes();
    sm_.unlock();
}

void SerialTSND151::Serial::clearRecvBuffer() {
    if (!rm_.try_lock()) {
        return;
    }
    rb_->resetIndexes();
    rm_.unlock();
}

void SerialTSND151::Serial::recv(bool b) {
    // ブロックするか否か
    if (b) {
        rm_.lock();
    } else {
        bool is_lock = rm_.try_lock();
        if (!is_lock)
            return;
    }

    FD_ZERO(&set_);
    FD_SET(fd_, &set_);

    timeout_.tv_sec = timeout_s_;
    timeout_.tv_usec = timeout_us_;

    // 受信バッファに何もないなら抜ける
    rv_ = select(fd_ + 1, &set_, NULL, NULL, &timeout_);
    if (rv_ <= 0) {
        rm_.unlock();
        return;
    }

    // 受信
    int ret = read(fd_, tmp_rb_, __SERIAL_TSND151_DEFAULT_BUFFER_SIZE);
    if (ret < 0) {
        error("No data received.\n");
        rm_.unlock();
        return;
    }

    // debug print
    // fflush(stdout);

    rm_.unlock();
}

void SerialTSND151::Serial::send(bool b) {
    // データないなら送信しない．
    if (sb_->getBufferLength() <= 0)
        return;

    if (b) {
        sm_.lock();
    } else {
        bool is_lock = sm_.try_lock();
        if (!is_lock)
            return;
    }

    tcflush(fd_, TCIOFLUSH);
    long long int send_len = sb_->getBufferLength();
    for (int i = 0; i < send_len; i++) {
        tmp_sb_[i] = sb_->popl();
    }

    write(fd_, tmp_sb_, send_len);
    info("Sended. %d\n", send_len);
    sm_.unlock();
}
