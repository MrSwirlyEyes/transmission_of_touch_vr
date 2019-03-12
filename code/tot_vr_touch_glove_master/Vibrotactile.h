#ifndef _VIBROTACTILE_H
#define _VIBROTACTILE_H

#include "PCA9685.h"

#define PWM_DRIVER_MIN    0
#define PWM_DRIVER_MAX 4095

class Vibrotactile {
  public:
    Vibrotactile(PCA9685 & _pwm_driver,byte _pin,int _min, int _max);

    void actuate(int intensity);

    void off();

    int get_min();

    int get_max();

  private:
    PCA9685 _pwm_driver;
    byte _pin;
    int _min;
    int _max;
};

#endif
