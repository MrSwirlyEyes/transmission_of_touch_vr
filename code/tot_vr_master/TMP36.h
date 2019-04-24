/*
TMP36.h - Library to obtain values from a TMP36Z Temperature Sensor
Created by Colin "MrSwirlyEyes"
For Project in a Box, University of California, San Diego.
*/

#ifndef TMP36_h
#define TMP36_h

#include <Arduino.h>
#include "CD74HC4067.h"

class TMP36 {
  public:
    TMP36(CD74HC4067 & _multiplexer,byte _pin,float _vcc);
    TMP36(CD74HC4067 & _multiplexer,byte _pin,float _vcc,
          float _min_limit_cold,float _max_limit_cold,
          float _min_limit_hot,float _max_limit_hot,
          int _min_mapping,int _max_mapping);

    float get_tempC();
    int get_tempC_mapped();

    float get_tempF();
  
  private:
    CD74HC4067 & _multiplexer;
    
    byte _pin;

    float _vcc;
    float _tempC;

    float _min_limit_cold;
    float _max_limit_cold;
    float _min_limit_hot;
    float _max_limit_hot;

    int _min_mapping;
    int _max_mapping;

    float read();
    int read_raw();
    float convert_tempC_to_tempF(float _tempC);
};

#endif
