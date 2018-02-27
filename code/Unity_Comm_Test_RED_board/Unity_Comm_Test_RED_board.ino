//#include <SoftwareSerial.h>
#include "SerialCommand.h"
#include <CD74HC4067.h>
#include <Adafruit_PWMServoDriver.h> //download library from adafruit website

//#define S1 0
//#define S2 1
//#define S3 2
//#define S4 3
//#define S5 4

#define L1 5
#define L2 6
#define L3 9
#define L4 10
#define L5 11

SerialCommand sCmd;

//defined mux pins from redboard
#define s0 2 
#define s1 6 
#define s2 7 
#define s3 10
#define sig_pin 0

//defined flex sensor values
#define FLEXMIN 0
#define FLEXMAX 90

//defined servo values
#define SERVOMIN  130
#define SERVOMAX  290 // Need another define for wrist servo

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
CD74HC4067 demux(s0,s1,s2,s3,sig_pin);


//servo motors
struct GlovePacket {
  int gThumb, gPointer, gMiddle, gRing, gPinky;
  int gFinger[5] = {gThumb, gPointer, gMiddle, gRing, gPinky};
  int gCheckSum = 0;
} gpkt;


//pressure sensors
struct ArmPacket {
  int aThumb, aPointer, aMiddle, aRing, aPinky;
  int aFinger[5] = {aThumb, aPointer, aMiddle, aRing, aPinky};
  float temp_readings[5]={0.0,};
  int aCheckSum = 0;
} apkt;

//struct SensorPacket {
//  int s1 = 0;
//  int s2 = 0;
//  int s3 = 0;
//  int s4 = 0;
//  int s5 = 0;
//} pkt;

//byte [] 

void pingHandler () {
//  Serial.write((byte *) & pkt, sizeof(SensorPacket));
  String data = "";
  data = (String) apkt.aFinger[0] + "," + apkt.aFinger[1] + "," + apkt.aFinger[2] + "," + apkt.aFinger[3] + "," + apkt.aFinger[4];
  Serial.println(data);
}

void echoHandler() {
  char * arg;
  int pwm1, pwm2, pwm3, pwm4, pwm5;
  
  arg = sCmd.next();
  if (arg != NULL) {
    if (sscanf(arg, "%i,%i,%i,%i,%i", &pwm1, &pwm2, &pwm3, &pwm4, &pwm5) == 5) {
      pwm.setPWM(0, 0, map(pwm1,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(1, 0, map(pwm2,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(2, 0, map(pwm3,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(3, 0, map(pwm4,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(4, 0, map(pwm5,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//      analogWrite(L1, pwm1);
//      analogWrite(L2, pwm2);
//      analogWrite(L3, pwm3);
//      analogWrite(L4, pwm4);
//      analogWrite(L5, pwm5);
    } else {
        pwm.setPWM(0, 0, map(0,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(1, 0, map(0,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(2, 0, map(0,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(3, 0, map(0,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
      pwm.setPWM(4, 0, map(0,FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
//      analogWrite(L1, 0);
//      analogWrite(L2, 255);
//      analogWrite(L3, 0);
//      analogWrite(L4, 255);
//      analogWrite(L5, 0);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);

  pwm.begin();
  pwm.setPWMFreq(40);

  for (int i = 0; i < 5; i++){
    gpkt.gFinger[i] = 0;
    apkt.aFinger[i] = 0;
  }

  for (int i = 0; i < 6; i++) {
    pwm.setPWM(i, 0, 0); 
  }

  sCmd.addCommand("PING", pingHandler);
  sCmd.addCommand("ECHO", echoHandler);
//  sCmd.setDefaultHandler(errorHandler);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    sCmd.readSerial();
  }

  for (int i = 0; i < 5; i++){
    delay(5);
    apkt.aFinger[i] = demux.read_channel(i);
    delay(5);
    apkt.aFinger[i] = demux.read_channel(i);
  }

//  delay(5);
//  pkt.s1 = analogRead(S1);
//  delay(5);
//  pkt.s1 = analogRead(S1);
//  delay(5);
//  pkt.s2 = analogRead(S2);
//  delay(5);
//  pkt.s2 = analogRead(S2);
//  delay(5);
//  pkt.s3 = analogRead(S3);
//  delay(5);
//  pkt.s3 = analogRead(S3);
//  delay(5);
//  pkt.s4 = analogRead(S4);
//  delay(5);
//  pkt.s4 = analogRead(S4);
//  delay(5);
//  pkt.s5 = analogRead(S5);
//  delay(5);
//  pkt.s5 = analogRead(S5);

}
