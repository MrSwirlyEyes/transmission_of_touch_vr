#include <Adafruit_PWMServoDriver.h>

#define ON 4095
#define OFF 0

#define pwm_frequency 60
Adafruit_PWMServoDriver pwm_driver = Adafruit_PWMServoDriver();

#define NUM_PIXELS 3

void setup() {
  // put your setup code here, to run once:
  pwm_driver.begin();

  pwm_driver.setPWMFreq(pwm_frequency);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < NUM_PIXELS; i++) {
    pwm_driver.setPWM(i,0,ON);
    delay(1000);
  }
  delay(1000);

  for(int i = 0; i < NUM_PIXELS; i++) {
    pwm_driver.setPWM(i,0,OFF);
    delay(1000);
  }
  delay(1000);
}
