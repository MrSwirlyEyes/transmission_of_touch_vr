#ifndef _PCA9685SERVO_H
#define _PCA9685SERVO_H

#include "PCA9685.h"

#define PWM_DRIVER_MIN    0
#define PWM_DRIVER_MAX 4095

class PCA9685Servo {
  public:
    PCA9685Servo(PCA9685 & _pwm_driver,byte _pin,int _min_limit, int _max_limit,int _min_mapping, int _max_mapping);

    void actuate(int intensity);

    void off();

    int get_min();

    int get_max();

  private:
    PCA9685 _pwm_driver;

    byte _pin;

    int _min_limit;
    int _max_limit;

    int _min_mapping;
    int _max_mapping;
};

#endif
