/*
TMP36.cpp - Library to obtain values from a TMP36Z Temperature Sensor
Created by Colin "MrSwirlyEyes"
For Project in a Box, University of California, San Diego.
*/

#include "TMP36.h"



TMP36::TMP36(CD74HC4067 & _multiplexer,byte _pin,float _vcc) : _multiplexer(_multiplexer),_pin(_pin),_vcc(_vcc),_tempC(0.0) {
	analogReference(EXTERNAL);
}



int TMP36::read_raw() {
//	float voltage = ((analogRead(this->_pin) * this->_vcc) / 1024.0);
//
//	this->_tempC = (voltage - 0.5) * 100;
//	this->_tempF = (this->_tempC * (9.0 / 5.0)) + 32.0;
  return this->_multiplexer.read_channel(this->_pin);
}

float TMP36::read() {
//	float _voltage = ((voltage * this->_vcc) / 1024.0);
  return ((this->_multiplexer.read_channel(this->_pin) * this->_vcc) / 1024.0);

//	this->_tempC = (_voltage - 0.5) * 100;
//	this->_tempF = (this->_tempC * (9.0 / 5.0)) + 32.0;
}

float TMP36::get_tempC() {
  this->_tempC = (read() - 0.5) * 100;
	return this->_tempC;
}

float TMP36::get_tempF() {
	return convert_tempC_to_tempF(get_tempC());
}

float TMP36::convert_tempC_to_tempF(float _tempC) {
  this->_tempC = _tempC;
  return ((this->_tempC * (9.0 / 5.0)) + 32.0);
}
