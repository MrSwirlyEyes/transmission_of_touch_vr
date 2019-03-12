#include "FSR.h"

FSR::FSR(CD74HC4067 & _multiplexer,byte _pin,int _min_limit,int _max_limit,int _min_mapping,int _max_mapping) : _multiplexer(_multiplexer) {
  this->_pin = _pin;

  this->_min_limit = constrain(_min_limit,ANALOG_MIN,ANALOG_MAX);
  this->_max_limit = constrain(_max_limit,ANALOG_MIN,ANALOG_MAX);
  
  this->_min_mapping = _min_mapping;
  this->_max_mapping = _max_mapping;
}

int FSR::read() {
  return constrain(map(read_raw(),this->_min_limit,this->_max_limit,this->_min_mapping,this->_max_mapping),this->_min_mapping,this->_max_mapping);
}

int FSR::read_raw() {
  return this->_multiplexer.read_channel(this->_pin);
}
