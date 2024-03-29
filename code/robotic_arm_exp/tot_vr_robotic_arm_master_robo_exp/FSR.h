#ifndef _FSR_H
#define _FSR_H

#include "CD74HC4067.h"

#define ANALOG_MIN    0
#define ANALOG_MAX 1023

class FSR {
  public:
    FSR(CD74HC4067& _multiplexer,byte _pin,int _min_limit,int _max_limit,int _min_mapping, int _max_mapping);

    int read();

    int read_raw();

    void set_max(int _max);
    void set_min(int _min);

    int get_max();
    int get_min();

  private:
    CD74HC4067& _multiplexer;

    byte _pin;
    
    int _min_limit;
    int _max_limit;
    
    int _min_mapping;
    int _max_mapping;
};

#endif
