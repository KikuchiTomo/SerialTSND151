//
// deque.hpp
// キューとスタックのクラス
//
// Created by KikuchiTomoo on 2022/08/25 (Auto added by Emacs 27.1)
// 

#ifndef DEQUE_HPP__
#define DEQUE_HPP__

#include <stdio.h>
#ifdef __linux
#include <malloc.h>
#else
#include <stdlib.h>
#endif

#include "packet_define_tsnd151.hpp"

template<class T>
class RingDeque {
private:
  T *buf_;

  long long int size_; // max buffer size
  long long int len_;  // content length
  
  int head_;
  int tail_;
public:
  RingDeque(long long int size);
  ~RingDeque();

  long long int get_buffer_size();
  long long int get_length();
  
  void push(T data);
  void write(T data, int offset);
  T pop();
  T read(int offset);

  void push_left(T data);
  void write_left(T data, int offset);
  T pop_left();
  T read_left(int offset);

  void print(const char* format);
  void println(const char* format);


  void zero_reset();
  void reset();
};


#endif
