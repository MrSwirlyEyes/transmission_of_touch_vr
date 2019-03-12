#ifndef _THERMOELECTRIC_H
#define _THERMOELECTRIC_H

#include "PCA9685.h"

#define PWM_DRIVER_MIN    0
#define PWM_DRIVER_MAX 4095

class Thermoelectric {
  public:
    Thermoelectric(PCA9685 & _pwm_driver,
        byte _hot_pin,byte _cold_pin,
        int _min_hot, int _max_hot,
        int _min_cold,int _max_cold,
        boolean _dir=1);

    void actuate(int intensity, boolean dir);

    void off();

  private:
    PCA9685 _pwm_driver;
    
    byte _hot_pin;
    byte _cold_pin;

    int _min_cold;
    int _max_cold;

    int _min_hot;
    int _max_hot;

    boolean _dir;  // Set hot direction
};

#endif
