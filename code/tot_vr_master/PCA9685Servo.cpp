#include "PCA9685Servo.h"

PCA9685Servo::PCA9685Servo(PCA9685 & _pwm_driver,byte _pin,int _min_limit, int _max_limit,int _min_mapping, int _max_mapping) : _pwm_driver(_pwm_driver),_pin(_pin) {  

  this->_min_limit = constrain(_min_limit,PWM_DRIVER_MIN,PWM_DRIVER_MAX);
  this->_max_limit = constrain(_max_limit,PWM_DRIVER_MIN,PWM_DRIVER_MAX);

  this->_min_mapping = constrain(_min_mapping,_min_limit,_max_limit);
  this->_max_mapping = constrain(_max_mapping,_min_limit,_max_limit);
}

void PCA9685Servo::actuate(int _position) {
  this->_pwm_driver.set_pwm(this->_pin,0,constrain(map(_position,this->_min_limit,this->_max_limit,this->_min_mapping,this->_max_mapping),this->_min_mapping,this->_max_mapping));
}

int PCA9685Servo::get_min() {
  return this->_min_limit;
}

int PCA9685Servo::get_max() {
  return this->_max_limit;
}
