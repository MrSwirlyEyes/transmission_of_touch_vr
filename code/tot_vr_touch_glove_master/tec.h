#ifndef _TEC_H
#define _TEC_H

#include "Adafruit_PWMServoDriver.h"

class TEC {
  public:
    TEC(Adafruit_PWMServoDriver _pwm,byte _hot_pin,byte _cold_pin,boolean _dir=1);

    void actuate(int intensity, boolean dir);

    void off();

  private:
    Adafruit_PWMServoDriver _pwm;
    byte _hot_pin;
    byte _cold_pin;

    boolean _dir;  // Set hot direction
};

#endif
