#include "Thermoelectric.h"

Thermoelectric::Thermoelectric(PCA9685 _pwm_driver,
        byte _hot_pin,byte _cold_pin,
        int _min_hot, int _max_hot,
        int _min_cold,int _max_cold,
        boolean _dir=1) {

  this->_pwm_driver = _pwm_driver;

  this->_hot_pin = _hot_pin;
  this->_cold_pin = _cold_pin;

  this->_min_cold = constrain(_min_cold,PWM_DRIVER_MIN,PWM_DRIVER_MAX);
  this->_max_cold = constrain(_max_cold,PWM_DRIVER_MIN,PWM_DRIVER_MAX);

  this->_min_hot = constrain(_min_hot,PWM_DRIVER_MIN,PWM_DRIVER_MAX);
  this->_max_hot = constrain(_max_hot,PWM_DRIVER_MIN,PWM_DRIVER_MAX);

  this->_dir = _dir;
}

void Thermoelectric::actuate(int intensity, boolean dir) {

  // HOT dir = 1/true
  if(dir) {
    this->_pwm_driver.setPWM(this->_cold_pin,0,0);
//    this->_pwm_driver.setPWM(this->_hot_pin,0,constrain(intensity,PWM_DRIVER_MIN,PWM_DRIVER_MAX));
    this->_pwm_driver.setPWM(this->_hot_pin,0,constrain(intensity,this->_min_hot,this->_max_hot));
  } else { // COLD dir = 0/false
    this->_pwm_driver.setPWM(this->_hot_pin,0,0);
//    this->_pwm_driver.setPWM(this->_cold_pin,0,constrain(intensity,PWM_DRIVER_MIN,PWM_DRIVER_MAX));
    this->_pwm_driver.setPWM(this->_cold_pin,0,constrain(intensity,this->_min_cold,this->_max_cold));
  }
}

void Thermoelectric::off() {
  this->_pwm_driver.setPWM(this->_cold_pin,0,0);
  this->_pwm_driver.setPWM(this->_hot_pin,0,0);
}
