#include "Adafruit_PWMServoDriver.h" //download library from adafruit website

#define NUM_SERVOS 5

#define SERVO_INITIAL 8
#define SERVO_FINAL SERVO_INITIAL + NUM_SERVOS

//defined servo values
#define SERVO_MIN  130
#define SERVO_MAX  290 // Need another define for wrist servo

#define SERVO_DELAY 1

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);
  Serial.println("start...");
  Serial.println("initializing servos...");
  
  pwm.begin();
  pwm.setPWMFreq(40);

  Serial.println("completed servo initialization...");
  
  for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {
    pwm.setPWM(i, 0, SERVO_MIN);
//    delay(10);
//    pwm.setPWM(i, 0, SERVO_MAX);
  }

  delay(5000);
}

void loop() {
  for (int j = SERVO_MIN; j < SERVO_MAX; j++) {
    for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {    
      pwm.setPWM(i, 0, j);
      delay(SERVO_DELAY);
    }
  }

  delay(500);

  for (int j = SERVO_MAX; j > SERVO_MIN; j--) {
    for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {    
      pwm.setPWM(i, 0, j);
      delay(SERVO_DELAY);
    }
  }
}
