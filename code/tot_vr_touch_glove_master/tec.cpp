#include "TEC.h"

TEC::TEC(Adafruit_PWMServoDriver _pwm,byte _hot_pin,byte _cold_pin,boolean _dir=1) {
  this->_hot_pin = _hot_pin;
  this->_cold_pin = _cold_pin;
  this->_pwm = _pwm;

  this->_dir = _dir;

//  this->_pwm.begin();
//  this->_pwm.setPWMFreq(PWM_FREQUENCY);

  // initialize
  this->_pwm.setPWM(this->_cold_pin,0,0);
  this->_pwm.setPWM(this->_hot_pin,0,0);
}

void TEC::actuate(int intensity, boolean dir) {

  // HOT dir = 1/true
  if(dir) {
    this->_pwm.setPWM(this->_cold_pin,0,0);
    this->_pwm.setPWM(this->_hot_pin,0,intensity);
  } else { // COLD dir = 0/false
    this->_pwm.setPWM(this->_hot_pin,0,0);
    this->_pwm.setPWM(this->_cold_pin,0,intensity);
  }
}

void TEC::off() {
  this->_pwm.setPWM(this->_cold_pin,0,0);
  this->_pwm.setPWM(this->_hot_pin,0,0);
}
