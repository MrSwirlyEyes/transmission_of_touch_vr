/*
TMP36.cpp - Library to obtain values from a TMP36Z Temperature Sensor
Created by Colin "MrSwirlyEyes"
For Project in a Box, University of California, San Diego.
*/

#include "TMP36.h"



//----- CONSTRUCTORS -----//
TMP36::TMP36(CD74HC4067 & _multiplexer,byte _pin,float _vcc) : _multiplexer(_multiplexer),_pin(_pin),_vcc(_vcc),_tempC(0.0) {
	analogReference(EXTERNAL);
}

TMP36::TMP36(CD74HC4067 & _multiplexer,byte _pin,float _vcc,
              float _min_limit_cold,float _max_limit_cold,
              float _min_limit_hot,float _max_limit_hot,
              int _min_mapping,int _max_mapping) : 
                _multiplexer(_multiplexer),_pin(_pin),_vcc(_vcc),
                _min_limit_cold(_min_limit_cold),_max_limit_cold(_max_limit_cold),
                _min_limit_hot(_min_limit_hot),_max_limit_hot(_max_limit_hot),
                _min_mapping(_min_mapping),_max_mapping(_max_mapping),_tempC(0.0) {
  analogReference(EXTERNAL);
}



//----- PUBLIC FUNCTIONS -----//
float TMP36::get_tempC() {
  this->_tempC = (this->read() - 0.5) * 100.0;
  return this->_tempC;
}

int TMP36::get_tempC_mapped() {
  this->_tempC = this->get_tempC();
  if(this->_pin == 11) {
    this->_tempC += 1.0;
  }
  if(this->_tempC < this->_min_limit_cold) { // COLD
    return constrain(map(this->_tempC,this->_min_limit_cold,this->_max_limit_cold,0,this->_min_mapping),this->_min_mapping,this->_max_mapping);
  } else if(this->_tempC > this->_min_limit_hot) { // HOT
    return constrain(map(this->_tempC,this->_min_limit_hot,this->_max_limit_hot,0,this->_max_mapping),this->_min_mapping,this->_max_mapping);
  } else {
    return 0;
  }
}

float TMP36::get_tempF() {
  return convert_tempC_to_tempF(get_tempC());
}



//----- PRIVATE FUNCTIONS -----//
int TMP36::read_raw() {
  return this->_multiplexer.read_channel(this->_pin);
}

float TMP36::read() {
  return ((this->_multiplexer.read_channel(this->_pin) * this->_vcc) / 1024.0);
}

float TMP36::convert_tempC_to_tempF(float _tempC) {
  this->_tempC = _tempC;
  return ((this->_tempC * (9.0 / 5.0)) + 32.0);
}
