#include "Adafruit_PWMServoDriver.h" //download library from adafruit website

#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4

#define L1 5
#define L2 6
#define L3 9
#define L4 10
#define L5 11
//#define L6 2
//#define L7 3
//#define L8 4
//#define L9 7
//#define L10 8

#define led 3

//defined flex sensor values
#define FLEXMIN 0
#define FLEXMAX 255

//defined servo values
#define SERVOMIN  130
#define SERVOMAX  290 // Need another define for wrist servo

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

struct SensorPacket {
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  int s4 = 0;
  int s5 = 0;
} outpkt;

struct ActuatorPacket {
  int pwm1 = 0;
  int pwm2 = 50;
  int pwm3 = 100;
  int pwm4 = 150;
  int pwm5 = 200;
//  int pwm6 = 0;
//  int pwm7 = 0;
//  int pwm8 = 0;
//  int pwm9 = 0;
//  int pwm10 = 0;
//  int pwm11 = 0;
} inpkt;

byte * stuff;

byte numRead;

byte ledVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial.println("ON");

  pwm.begin();
  pwm.setPWMFreq(40);

  for (int i = 0; i < 6; i++) {
    pwm.setPWM(i, 0, 0); 
  }

  numRead = sizeof(int);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
      numRead = Serial.readBytes((byte *) &inpkt, sizeof(inpkt));
      sendFingers();
      analogWrite(led, ledVal++);
      updateActuators();
  }

  updateSensors();
}

void sendFingers() {
    String data = (String) outpkt.s1 + "," + outpkt.s2 + "," + outpkt.s3 + "," + outpkt.s4 + "," + outpkt.s5;
//    String data = (String) numRead;
    Serial.println(data);
}

void updateSensors() {
  delay(5);
  outpkt.s1 = analogRead(S1);
  delay(5);
  outpkt.s1 = analogRead(S1);
  delay(5);
  outpkt.s2 = analogRead(S2);
  delay(5);
  outpkt.s2 = analogRead(S2);
  delay(5);
  outpkt.s3 = analogRead(S3);
  delay(5);
  outpkt.s3 = analogRead(S3);
  delay(5);
  outpkt.s4 = analogRead(S4);
  delay(5);
  outpkt.s4 = analogRead(S4);
  delay(5);
  outpkt.s5 = analogRead(S5);
  delay(5);
  outpkt.s5 = analogRead(S5);
}

void updateActuators() {
  //fingers
//  pwm.setPWM(0, 0, map(inpkt.pwm1,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//  pwm.setPWM(1, 0, map(inpkt.pwm2,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//  pwm.setPWM(2, 0, map(inpkt.pwm3,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//  pwm.setPWM(3, 0, map(inpkt.pwm4,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//  pwm.setPWM(4, 0, map(inpkt.pwm5,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//
//  //wrist
//  pwm.setPWM(5, 0, map(inpkt.pwm11,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//
//  //leds
//  pwm.setPWM(6, 0, map(inpkt.pwm6,FLEXMIN,FLEXMAX,0, 4095));
//  pwm.setPWM(7, 0, map(inpkt.pwm7,FLEXMIN,FLEXMAX,0, 4095));
//  pwm.setPWM(8, 0, map(inpkt.pwm8,FLEXMIN,FLEXMAX,0, 4095));
//  pwm.setPWM(9, 0, map(inpkt.pwm9,FLEXMIN,FLEXMAX,0, 4095));
//  pwm.setPWM(10, 0, map(inpkt.pwm10,FLEXMIN,FLEXMAX,0, 4095));
  
  analogWrite(L1, inpkt.pwm1);
  analogWrite(L2, inpkt.pwm2);
  analogWrite(L3, inpkt.pwm3);
  analogWrite(L4, inpkt.pwm4);
  analogWrite(L5, inpkt.pwm5);
//  analogWrite(L5, inpkt.pwm11);
//  digitalWrite(L6, thresholdPWM(inpkt.pwm6));
//  digitalWrite(L7, thresholdPWM(inpkt.pwm7));
//  digitalWrite(L8, thresholdPWM(inpkt.pwm8));
//  digitalWrite(L9, thresholdPWM(inpkt.pwm9));
//  digitalWrite(L10, thresholdPWM(inpkt.pwm10));
}

byte thresholdPWM(byte val) {
  return (val > 125);
}

