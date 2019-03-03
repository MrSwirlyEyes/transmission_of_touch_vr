#include "Adafruit_PWMServoDriver.h" //download library from adafruit website

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//defined vib motor values
#define VIBMIN 0
#define VIBMAX 4095

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setPWMFreq(40);
}

void loop() {
  // put your main code here, to run repeatedly:
  test_vibrotactiles();
}

void test_vibrotactiles() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 255; j++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
      delay(2);
    }
    delay(100);
    for (int j = 255; j >= 0; j--) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
      delay(2);
    }
    delay(100);
  }

  delay(100);

  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < 6; i++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));

    }
    delay(2);
  }
  delay(100);
  for (int j = 255; j >= 0; j--) {
    for (int i = 0; i < 6; i++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));

    }
    delay(2);
  }
  delay(100);
}
