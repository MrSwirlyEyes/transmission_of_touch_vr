#include "FSR.h"

FSR::FSR(CD74HC4067& _multiplexer,byte _pin,int _min_limit,int _max_limit,int _min_mapping,int _max_mapping) : _multiplexer(_multiplexer),_pin(_pin) {

  this->_min_limit = constrain(_min_limit,ANALOG_MIN,ANALOG_MAX);
  this->_max_limit = constrain(_max_limit,ANALOG_MIN,ANALOG_MAX);
  
  this->_min_mapping = _min_mapping;
  this->_max_mapping = _max_mapping;
}

int FSR::read() {
  int _reading = read_raw();
//  if(_reading > this->_min_limit) {
//    this->_min_limit = _reading;
//  } else if(_reading < this->_max_limit) {
//    this->_max_limit = _reading;
//  }
  return constrain(map(_reading,this->_min_limit,this->_max_limit,this->_min_mapping,this->_max_mapping),this->_min_mapping,this->_max_mapping);
}

int FSR::read_raw() {
  return this->_multiplexer.read_channel(_pin);
}

void FSR::set_max(int _max) {
  this->_max_limit = constrain(_max,ANALOG_MIN,ANALOG_MAX);
}

void FSR::set_min(int _min) {
  this->_min_limit = constrain(_min,ANALOG_MIN,ANALOG_MAX);
}

int FSR::get_max() {
  return this->_max_limit;
}

int FSR::get_min() {
  return this->_min_limit;
}
