#include <iostream>
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include "buffer.hpp"
#include "serial.hpp"


void testRingDeque(){
  SerialTSND151::RingDeque<int> *d0 = new SerialTSND151::RingDeque<int>(1024, 0);

  d0->push(2);
  d0->push(42);
  d0->push(-1);
  d0->push(43);
  d0->push(9);

  std::cout << "checking (pop test)";
  assert(d0->pop() == 9 );
  assert(d0->pop() == 43);
  assert(d0->pop() == -1);
  assert(d0->pop() == 42);
  assert(d0->pop() == 2 );
  std::cout << " ..... Pass" << std::endl;

  d0->pushl(2);
  d0->pushl(42);
  d0->pushl(-1);
  d0->pushl(43);
  d0->pushl(9);

  std::cout << "checking (popl test)";
  assert(d0->popl() == 9 );
  assert(d0->popl() == 43);
  assert(d0->popl() == -1);
  assert(d0->popl() == 42);
  assert(d0->popl() == 2 );
  std::cout << " ..... Pass" << std::endl;

  d0->push(2);
  d0->push(42);
  d0->push(-1);
  d0->push(43);
  d0->push(9);

  std::cout << "checking (queue test)";
  assert(d0->popl() == 2 );
  assert(d0->popl() == 42);
  assert(d0->popl() == -1);
  assert(d0->popl() == 43);
  assert(d0->popl() == 9 );      
  std::cout << " ..... Pass" << std::endl;

  delete d0;
}

void testSerial(){
  SerialTSND151::Serial *d1 = new SerialTSND151::Serial();
  d1->setPort("/dev/tty.usbmodemAP091823081");
  d1->begin(9600);
  sleep(1);
  d1->end();
}

int main(void){
 
  testRingDeque();
  testSerial();
  
  return 0;
}
