//
// deque_struct.cpp
// キューとスタックのクラス
//
// Created by KikuchiTomoo on 2022/08/25 (Auto added by Emacs 27.1)
// 

#include "deque_struct.hpp"

template<class T>
RingDequeStruct<T>::RingDequeStruct(long long int size){
  size_ = size;
  len_ = 0;

  head_ = 0;
  tail_ = 0;

  buf_ = (T*) malloc (sizeof(T) * size);
}

template<class T>
RingDequeStruct<T>::~RingDequeStruct(){
  free(buf_);  
}

template<class T>
long long int RingDequeStruct<T>::get_buffer_size(){
  return size_;
}

template<class T>
long long int RingDequeStruct<T>::get_length(){
  return len_;
}

template<class T>
void RingDequeStruct<T>::zero_reset(){
  T zero = (T){0};
  for(long long int i=0; i<size_; i++){    
    buf_[i] = zero;
  }
}

template<class T>
void RingDequeStruct<T>::reset(){
  zero_reset();
  head_ = 0;
  tail_ = 0;
  len_  = 0;
}

template <class T>
void RingDequeStruct<T>::push(T data){
  if(size_>0 && len_<size_){
    head_ = (size_ + head_ - 1) % size_;
    buf_[head_] = data;
    len_++;
  }
}

template <class T>
void RingDequeStruct<T>::write(T data, int offset){
  if(len_>0 && size_>0 && len_>offset){
    int tmp = head_;
    tmp = (tmp + size_ + offset) % size_;
    buf_[tmp] = data;
  }
}

template <class T>
T RingDequeStruct<T>::pop(){
  T tmp_ret = (T){0};
  if(len_>0 && size_>0){
    tmp_ret = buf_[head_];
    head_ = (size_ + head_ + 1) % size_;
    len_--;
  }
  return tmp_ret;
}

template <class T>
T RingDequeStruct<T>::read(int offset){
  T tmp_ret = (T){0};  
  if(len_>0 && size_>0){
    int tmp = head_;
    tmp = (tmp + size_ + offset) % size_;
    tmp_ret = buf_[tmp];
  }
  return tmp_ret;
}

template <class T>
void RingDequeStruct<T>::push_left(T data){
  if(size_>0 && len_<size_){
    buf_[tail_] = data;
    tail_ = (size_ + tail_ + 1) % size_;
    len_++;
  }
}

template <class T>
void RingDequeStruct<T>::write_left(T data, int offset){
  if(len_>0 && size_>0){
    int tmp = tail_;
    tmp = (tmp + size_ - offset - 1) % size_;
    buf_[tmp] = data;
  }
}

template <class T>
T RingDequeStruct<T>::pop_left(){
  T tmp_ret = (T){0};    
  if(len_>0 && size_>0){
    tail_ = (size_ + tail_ - 1) % size_;
    tmp_ret = buf_[tail_];
    len_--;
  }
  return tmp_ret;
}

template <class T>
T RingDequeStruct<T>::read_left(int offset){
  T tmp_ret = (T){0};  
  if(len_>0 && size_>0){
    int tmp = tail_;
    tmp = (tmp + size_ - offset - 1) % size_;
    tmp_ret = buf_[tmp];
  }
  return tmp_ret;
}

template <class T>
void RingDequeStruct<T>::print(const char* format){
  for(int i=0; i<len_; i++){
    printf(format, read_left(i));
  }  
}

template <class T>
void RingDequeStruct<T>::println(const char* format){
  for(int i=0; i<len_; i++){
    printf(format, read_left(i));
  }
  printf("\n");
}

template class RingDequeStruct<TSND151_Event_t>;
