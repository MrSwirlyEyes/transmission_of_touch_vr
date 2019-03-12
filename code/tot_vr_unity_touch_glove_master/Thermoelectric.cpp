#include "Thermoelectric.h"

Thermoelectric::Thermoelectric(PCA9685 & _pwm_driver,
                              byte _hot_pin,byte _cold_pin,
                              int _min_hot, int _max_hot,
                              int _min_cold,int _max_cold,
                              boolean _dir=1) : _pwm_driver(_pwm_driver),_hot_pin(_hot_pin),_cold_pin(_cold_pin),_dir(_dir) {

  this->_min_cold = constrain(abs(_min_cold),PWM_DRIVER_MIN,PWM_DRIVER_MAX);
  this->_max_cold = constrain(abs(_max_cold),PWM_DRIVER_MIN,PWM_DRIVER_MAX);

  this->_min_hot = constrain(abs(_min_hot),PWM_DRIVER_MIN,PWM_DRIVER_MAX);
  this->_max_hot = constrain(abs(_max_hot),PWM_DRIVER_MIN,PWM_DRIVER_MAX);
}

void Thermoelectric::actuate(int intensity) {

  // HOT dir = 1/true
//  if(dir) {
//    this->_pwm_driver.set_pwm(this->_cold_pin,0,0);
//    this->_pwm_driver.set_pwm(this->_hot_pin,0,constrain(intensity,this->_min_hot,this->_max_hot));
//  } else { // COLD dir = 0/false
//    this->_pwm_driver.set_pwm(this->_hot_pin,0,0);
//    this->_pwm_driver.set_pwm(this->_cold_pin,0,constrain(intensity,this->_min_cold,this->_max_cold));
//  }

  if(this->_dir) {
    if(intensity > 0) {
      this->_pwm_driver.set_pwm(this->_cold_pin,0,0);
      this->_pwm_driver.set_pwm(this->_hot_pin,0,constrain(abs(intensity),this->_min_hot,this->_max_hot));
    } else if (intensity < 0) {
      this->_pwm_driver.set_pwm(this->_hot_pin,0,0);
      this->_pwm_driver.set_pwm(this->_cold_pin,0,constrain(abs(intensity),this->_min_cold,this->_max_cold));
    } else {
      off();
    }
  } else {
    if(intensity > 0) {
      this->_pwm_driver.set_pwm(this->_hot_pin,0,0);
      this->_pwm_driver.set_pwm(this->_cold_pin,0,constrain(abs(intensity),this->_min_cold,this->_max_cold));
    } else if (intensity < 0) {
      this->_pwm_driver.set_pwm(this->_cold_pin,0,0);
      this->_pwm_driver.set_pwm(this->_hot_pin,0,constrain(abs(intensity),this->_min_hot,this->_max_hot));
    } else {
      off();
    }
  }
}

void Thermoelectric::off() {
  this->_pwm_driver.set_pwm(this->_cold_pin,0,0);
  this->_pwm_driver.set_pwm(this->_hot_pin,0,0);
}
