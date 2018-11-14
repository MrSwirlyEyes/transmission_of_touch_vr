#include "Adafruit_PWMServoDriver.h" //download library from adafruit website

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


//defined vib motor values
#define TECMIN 0
#define TECMAX 4095 // 1.5V MAX on TECs

#define TECMAX_HOT 2095
#define TECMAX_COLD 4095

#define PHASE_COLD 0
#define PHASE_HOT 1
#define TECDELAY 1

void setup() {
  // put your setup code here, to run once:
  pwm.begin();
  pwm.setPWMFreq(60);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  test_thermoelectrics();
}

void test_thermoelectrics() {
  //  actuate_thermoelectric(15,14);
  //  actuate_thermoelectric(13,12);
  //  actuate_thermoelectric(11,10);
  //  actuate_thermoelectric(9,8);
  //  actuate_thermoelectric(7,6);

  actuate_all_thermoelectrics();
}

void actuate_thermoelectric(byte _en, byte _ph) {

  Serial.print("Actuating thermoelectric: ");
  Serial.print("(");
  Serial.print(_en);  Serial.print(",");
  Serial.print(_en);  Serial.println(")");

  // COLD
  pwm.setPWM(_ph, 0, 0);
  bool dir = 0;
  drive_thermoelectric(_en, dir);

  // HOT
  pwm.setPWM(_ph, 0, TECMAX);
  dir = 1;
  drive_thermoelectric(_en, dir);
}

void drive_thermoelectric(byte _en, bool dir) {
  for (int j = 0; j < 255; j++) {
    Serial.println(j);
    if (dir) {
      pwm.setPWM(_en, 0, map(j, 0, 255, TECMIN, TECMAX_HOT));
    } else {
      pwm.setPWM(_en, 0, map(j, 0, 255, TECMIN, TECMAX_COLD));
    }
    delay(TECDELAY);
  }
  delay(500);
  for (int j = 255; j >= 0; j--) {
    Serial.println(j);
    if (dir) {
      pwm.setPWM(_en, 0, map(j, 0, 255, TECMIN, TECMAX_HOT));
    } else {
      pwm.setPWM(_en, 0, map(j, 0, 255, TECMIN, TECMAX_COLD));
    }
    delay(TECDELAY);
  }
  delay(500);
}

void actuate_all_thermoelectrics() {
  Serial.print("Actuating ALL thermoelectrics: ");
  pwm.setPWM(14, 0, 0);
  pwm.setPWM(12, 0, 0);
  pwm.setPWM(10, 0, 0);
  pwm.setPWM(8, 0, 0);
  pwm.setPWM(6, 0, 0);
  bool dir = 0;

  for (int j = 0; j < 255; j++) {
    for (int i = 7; i <= 15 ; i += 2) {

      if (dir) {
        pwm.setPWM(i, 0, map(j, 0, 255, TECMIN, TECMAX_HOT));
      } else {
        pwm.setPWM(i, 0, map(j, 0, 255, TECMIN, TECMAX_COLD));
      }
      delay(TECDELAY);
    }
    Serial.println(j);
  }
  delay(500);
  dir = 1;
  for (int j = 255; j >= 0; j--) {
    for (int i = 7; i <= 15 ; i += 2) {

      if (dir) {
        pwm.setPWM(i, 0, map(j, 0, 255, TECMIN, TECMAX_HOT));
      } else {
        pwm.setPWM(i, 0, map(j, 0, 255, TECMIN, TECMAX_COLD));
      }
      delay(TECDELAY);
    }
    Serial.println(j);
  }

//  delay(500);
//  pwm.setPWM(14, 0, TECMAX);
//  pwm.setPWM(12, 0, TECMAX);
//  pwm.setPWM(10, 0, TECMAX);
//  pwm.setPWM(8, 0, TECMAX);
//  pwm.setPWM(6, 0, TECMAX);
  delay(500);

  for (int j = 0; j < 255; j++) {
    for (int i = 7; i <= 15 ; i += 2) {

      pwm.setPWM(i, 0, map(j, 0, 255, TECMIN, TECMAX));
      delay(TECDELAY);
    }
    Serial.println(j);
  }
  delay(1000);
  for (int j = 255; j >= 0; j--) {
    for (int i = 7; i <= 15 ; i += 2) {

      pwm.setPWM(i, 0, map(j, 0, 255, TECMIN, TECMAX));
      delay(TECDELAY);
    }
    Serial.println(j);
  }
}
