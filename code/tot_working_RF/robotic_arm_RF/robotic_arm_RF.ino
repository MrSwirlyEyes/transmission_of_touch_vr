//USE THIS IF YOU HAVE Radio.h library instead of radio.h
#include <Radio.h>

//#include <radio.h>

//include this library from github
#include <CD74HC4067.h>
//#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //library needs to be downloaded

#define SERVOMIN  130 //This is the 'minimum' pulse length count (out of 4096).
#define SERVOMAX  290 //This is the 'maximum' pulse length count (out of 4096).

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Radio radio(11);

int servoNum, angle1, angle2, angle3, angle4, angle5, i;

struct Packet {
  int thumb = 0;
  int pointer = 0;
  int middle = 0;
  int ring = 0;
  int pinky  = 0;
  int checkSum = 0;
} pkt;

void setup() 
{
  //Set up serial communication and the number of servos being tested.
  Serial.begin(9600);

  //Set up PWM.
  pwm.begin();
  pwm.setPWMFreq(40);
}

void loop() 
{
  
  //Loop through and read all 16 values
  //Reports back Value at channel 6 is: 346
  if(radio.rfAvailable()) {
    radio.rfRead((uint8_t *) & pkt, sizeof(Packet));
  }
  int checkSum1 = pkt.thumb + pkt.pointer + pkt.middle + pkt.ring + pkt.pinky;
  if(checkSum1 == pkt.checkSum) {
      //IMPORTANT MAPPING
      //motor 1: ring finger
      //motor 2: middle finger
      //motor 3: pointer finger
      //motor 4: pinky
      //motor 5: thumb
      Serial.print("(");
      angle1 = map(pkt.thumb,400,700,0,90);
      Serial.print(angle1); Serial.print(",");
      turnTo(5, angle1);
      angle2 = map(pkt.pointer,400,700,0,90);
      Serial.print(angle2); Serial.print(",");
      turnTo(3, angle2);  
      angle3 = map(pkt.middle,400,700,0,90);
      Serial.print(angle3);
      turnTo(2, angle3);  Serial.print(",");
      angle4 = map(pkt.ring,400,700,0,90);
      Serial.print(angle4);
      turnTo(1, angle4);  Serial.print(",");
      angle5 = map(pkt.pinky,400,700,0,90);
      Serial.print(angle5); Serial.println(")");
      turnTo(4, angle5);
  }
}

void turnTo(int servoNum, int angle)
{
  angle = map(angle, 0, 90, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoNum, 0, angle);
}
