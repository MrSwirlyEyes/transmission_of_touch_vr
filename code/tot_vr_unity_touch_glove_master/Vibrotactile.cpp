#include "Vibrotactile.h"

Vibrotactile::Vibrotactile(Adafruit_PWMServoDriver _pwm_driver,byte _pin,int _min, int _max) {
  this->_pwm_driver = _pwm_driver;

  this->_pin = _pin;

  this->_min = constrain(_min,PWM_DRIVER_MIN,PWM_DRIVER_MAX);
  this->_max = constrain(_max,PWM_DRIVER_MIN,PWM_DRIVER_MAX);
}

void Vibrotactile::actuate(int intensity) {
//  this->_pwm_driver.setPWM(this->_pin,0,intensity);
  this->_pwm_driver.setPWM(this->_pin,0,constrain(intensity,this->_min,this->_max));
}

int Vibrotactile::get_min() {
  return this->_min;
}

int Vibrotactile::get_max() {
  return this->_max;
}

void Vibrotactile::off() {
  this->_pwm_driver.setPWM(this->_pin,0,0);
}
