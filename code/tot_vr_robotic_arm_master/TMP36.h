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
    
    int read_raw();

    float get_tempC();
    float get_tempF();
  
  private:
    CD74HC4067 & _multiplexer;
    
    byte _pin;
    
    float _vcc;
    
    float _tempC;

    float read();
    float convert_tempC_to_tempF(float _tempC);
};

#endif
