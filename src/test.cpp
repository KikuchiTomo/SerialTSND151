#include "buffer.hpp"
#include "serial.hpp"
#include "tsnd151.hpp"
#include <cassert>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

void testRingDeque() {
    SerialTSND151::RingDeque<int> *d0 =
        new SerialTSND151::RingDeque<int>(1024, 0);

    d0->push(2);
    d0->push(42);
    d0->push(-1);
    d0->push(43);
    d0->push(9);

    std::cout << "checking (pop test)";
    assert(d0->pop() == 9);
    assert(d0->pop() == 43);
    assert(d0->pop() == -1);
    assert(d0->pop() == 42);
    assert(d0->pop() == 2);
    std::cout << " ..... Pass" << std::endl;

    d0->pushl(2);
    d0->pushl(42);
    d0->pushl(-1);
    d0->pushl(43);
    d0->pushl(9);

    std::cout << "checking (popl test)";
    assert(d0->popl() == 9);
    assert(d0->popl() == 43);
    assert(d0->popl() == -1);
    assert(d0->popl() == 42);
    assert(d0->popl() == 2);
    std::cout << " ..... Pass" << std::endl;

    d0->push(2);
    d0->push(42);
    d0->push(-1);
    d0->push(43);
    d0->push(9);

    std::cout << "checking (queue test)";
    assert(d0->popl() == 2);
    assert(d0->popl() == 42);
    assert(d0->popl() == -1);
    assert(d0->popl() == 43);
    assert(d0->popl() == 9);
    std::cout << " ..... Pass" << std::endl;

    delete d0;
}

void testSerial() {
    SerialTSND151::Serial *d1 = new SerialTSND151::Serial();
    d1->setPort("/dev/tty.usbmodemAP091823081");
    d1->begin(9600);
    sleep(1);
    d1->end();
}

void testTSND151() {
    SerialTSND151::TSND151 *d1 = new SerialTSND151::TSND151();
    d1->begin("/dev/tty.usbmodemAP091823081", 9600);
    sleep(1);
    SerialTSND151::Response::Response_t res = d1->get_info_b();
    printf("S/N %s\n", res.Info.SerialNumber);
    printf("BLE ");
    for (int i = 0; i < 10; i++) {
        printf("%02X ", res.Info.BluetoothAddress[i] & 0x00FF);
    }
    printf("\n");
    printf("Typ %s\n", res.Info.Type);
    d1->end();
}

int main(void) {

    testRingDeque();
    // testSerial();
    testTSND151();

    return 0;
}
