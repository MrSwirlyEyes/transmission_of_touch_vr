#include <radio.h> //include this library from github
//#include <Radio.h> if you're Colin
#include <CD74HC4067.h> //include this library from github
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //download library from adafruit website

//defined mux pins from redboard
#define s0 2 
#define s1 6 
#define s2 7 
#define s3 10
#define sig_pin 0

//defined flex sensor values
#define FLEXMIN 400
#define FLEXMAX 700

//defined servo values
#define SERVOMIN  130
#define SERVOMAX  290 

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Radio radio(11);
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
  int aCheckSum = 0;
} apkt;

void setup() 
{
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(40);
  
  for (int i = 0; i < 5; i++){
    gpkt.gFinger[i] = 0;
    apkt.aFinger[i] = 0;
  }
}

void loop() 
{
  //GLOVE PACKET: RECEIVING FLEX VALS FROM GLOVE TO MOVE SERVO MOTORS ON ROBO ARM
  if(radio.rfAvailable()) {
    radio.rfRead((uint8_t *) & gpkt, sizeof(GlovePacket));
  }
  int gCheckSumTot = gpkt.gFinger[0] + gpkt.gFinger[1] + gpkt.gFinger[2] + gpkt.gFinger[3] + gpkt.gFinger[4];
  if(gCheckSumTot == gpkt.gCheckSum) {
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(i, 0, map(gpkt.gFinger[i],FLEXMIN,FLEXMAX,SERVOMIN, SERVOMAX));
    }
  }

  //ARM PACKET: SENDING PRESSURE SENSOR VALS FROM ROBO ARM TO MOVE VIB MOTORS
  for (int i = 0; i < 5; i++){
    delay(5);
    apkt.aFinger[i] = demux.read_channel(i);
    delay(5);
    apkt.aFinger[i] = demux.read_channel(i);
  }
  apkt.aCheckSum = apkt.aThumb + apkt.aPointer + apkt.aMiddle + apkt.aRing + apkt.aPinky;
  radio.rfWrite((uint8_t *) & apkt, sizeof(ArmPacket));
}
