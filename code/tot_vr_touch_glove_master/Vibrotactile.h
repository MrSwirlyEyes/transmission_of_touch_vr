#ifndef _VIBROTACTILE_H
#define _VIBROTACTILE_H

#include "Adafruit_PWMServoDriver.h"

#define PWM_DRIVER_MIN    0
#define PWM_DRIVER_MAX 4095

class Vibrotactile {
  public:
    Vibrotactile(Adafruit_PWMServoDriver _pwm_driver,byte _pin,int _min, int _max);

    void actuate(int intensity);

    void off();

    int get_min();

    int get_max();

  private:
    Adafruit_PWMServoDriver _pwm_driver;
    byte _pin;
    int _min;
    int _max;
};

#endif
