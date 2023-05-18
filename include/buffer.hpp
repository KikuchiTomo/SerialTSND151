#ifndef __SERIAL_TSND151_BUFFER_HPP_
#define __SERIAL_TSND151_BUFFER_HPP_

#ifdef __APPLE__
#include <stdlib.h>
#elif __GNUC__
#include <malloc.h>
#endif


namespace SerialTSND151{
  template<class T>
  class RingDeque{
  private:
    T* b_;

    long long int s_;
    long long int l_;

    long long int h_;
    long long int t_;

    T d_;    
  public:
    RingDeque(long long int size, T defaultValue){
      s_ = size;
      l_ = 0;

      h_ = 0;
      t_ = 0;

      b_ = (T*) calloc(sizeof(T), size);
      d_ = defaultValue;
    }
    
    ~RingDeque(){
      free(b_);      
    }

    long long int getBufferCapacity(){
      return s_;
    }
    
    long long int getBufferLength(){
      return l_;
    }

    void push(T data, bool *fail = NULL){
      if(fail != NULL) *fail = true;      
      if(s_>0 && l_<s_){
        h_ = (s_ + h_ - 1) % s_;
        b_[h_] = data;
        l_++;
        if(fail != NULL) *fail = false;
      }
    }
    
    void write(T data, int offset, bool *fail = NULL){
      if(fail != NULL) *fail = true;      
      if(l_>0 && s_>0 && l_>offset){
        int tmp = h_;
        tmp = (tmp + s_ + offset) % s_;
        b_[tmp] = data;
        if(fail != NULL) *fail = false;
      }
    }
    
    T pop(bool *fail = NULL){
      T tmp = d_;
      if(fail != NULL) *fail = true;      
      if(l_>0 && s_>0){
        tmp = b_[h_];
        h_ = (s_ + h_ + 1) % s_;
        l_--;
        if(fail != NULL) *fail = false;
      }      
      return tmp;
    }
    
    T read(int offset, bool *fail = NULL){
      T tmp = d_;
      if(fail != NULL) *fail = true;            
      if(l_>0 && s_>0){
        int idx = h_;
        idx = (idx + s_ + offset) % s_;
        tmp = b_[idx];

        if(fail != NULL) *fail = false;
      }
      return tmp;      
    }

    void pushl(T data, bool *fail = NULL){
      if(fail != NULL) *fail = true;      
      if(s_>0 && l_<s_){
        b_[t_] = data;
        t_ = (s_ + t_ + 1) % s_;
        l_++;

        if(fail != NULL) *fail = false;
      }
    }
    
    void writel(T data, int offset, bool *fail = NULL){
      if(fail != NULL) *fail = true;
      if(l_>0 && s_ > 0){
        int tmp = t_;
        tmp = (tmp + s_ - offset - 1) % s_;
        b_[tmp] = data;

        if(fail != NULL) *fail = false;        
      }     
    }
    
    T popl(bool *fail = NULL){
      T tmp = d_;
      if(fail != NULL) *fail = true;
      if(l_>0 && s_>0){
        t_ = (s_ + t_ - 1) % s_;
        tmp = b_[t_];
        l_--;

        if(fail != NULL) *fail = false;
      }
      return tmp;
    }
    
    T readl(int offset, bool *fail = NULL){
      T tmp = d_;
      if(fail != NULL) *fail = true;      
      if(l_>0 && s_>0){
        int idx = t_;
        idx = (idx + s_ - offset - 1) % s_;
        tmp = b_[idx];
        if(fail != NULL) *fail = false;
      }
      return tmp;
    }
    
    void resetIndexes(){
      h_ = t_ = l_ = 0;
    }

    void resetBuffer(){
      for(long long int i=0; i<s_; i++){
        b_[i] = d_;
      }
    }
  };
};

#endif
