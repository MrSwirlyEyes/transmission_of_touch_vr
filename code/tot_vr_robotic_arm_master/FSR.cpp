#include "FSR.h"

FSR::FSR(CD74HC4067& _multiplexer,byte _pin,int _min_limit,int _max_limit,int _min_mapping,int _max_mapping) : _multiplexer(_multiplexer) {
  this->_pin = _pin;

  this->_min_limit = _min_limit;
  this->_max_limit = _max_limit;
  
  this->_min_mapping = _min_mapping;
  this->_max_mapping = _max_mapping;
}

int FSR::read() {
  return constrain(map(read_raw(),this->_min_limit,this->_max_limit,this->_min_mapping,this->_max_mapping),this->_min_mapping,this->_max_mapping);
}

int FSR::read_raw() {
  return this->_multiplexer.read_channel(_pin);
}

void FSR::set_max(int _max) {
  this->_max_limit = _max;
}

void FSR::set_min(int _min) {
  this->_min_limit = _min;
}

int FSR::get_max() {
  return this->_max_limit;
}

int FSR::get_min() {
  return this->_min_limit;
}