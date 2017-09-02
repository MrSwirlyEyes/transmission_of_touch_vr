#include <radio.h> //include this library from github
//#include <Radio.h> if you're Colin
#include <CD74HC4067.h> //include this library from github
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //download library from adafruit website

#define SERVOMIN  130 //This is the 'minimum' pulse length count (out of 4096).
#define SERVOMAX  290 //This is the 'maximum' pulse length count (out of 4096).

#define s0 2 
#define s1 6 
#define s2 7 
#define s3 10
#define sig_pin 0

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Radio radio(11);
CD74HC4067 demux(s0,s1,s2,s3,sig_pin);

int servoNum, angle1, angle2, angle3, angle4, angle5, i;

//flex sensor
struct GlovePacket {
  int gthumb = 0;
  int gpointer = 0;
  int gmiddle = 0;
  int gring = 0;
  int gpinky  = 0;
  int gcheckSum = 0;
} gpkt;

//vib motors
struct ArmPacket {
  int athumb = 0;
  int apointer = 0;
  int amiddle = 0;
  int aring = 0;
  int apinky  = 0;
  int acheckSum = 0;
} apkt;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(40);
}

void loop() {
  //GLOVE PACKET
  delay(5);
  gpkt.gthumb = demux.read_channel(0);
  delay(5);
  gpkt.gthumb = demux.read_channel(0);
  delay(5);
  gpkt.gpointer = demux.read_channel(1);
  delay(5);
  gpkt.gpointer = demux.read_channel(1);
  delay(5);
  gpkt.gmiddle = demux.read_channel(2);
  delay(5);
  gpkt.gmiddle = demux.read_channel(2);
  delay(5);
  gpkt.gring = demux.read_channel(3);
  delay(5);
  gpkt.gring = demux.read_channel(3);
  delay(5);
  gpkt.gpinky = demux.read_channel(4);
  delay(5);
  gpkt.gpinky = demux.read_channel(4);
  delay(5);
  gpkt.gcheckSum = gpkt.gthumb + gpkt.gpointer + gpkt.gmiddle + gpkt.gring + gpkt.gpinky;
  radio.rfWrite((uint8_t *) & gpkt, sizeof(GlovePacket));
  Serial.println(gpkt.gthumb);
  Serial.println(gpkt.gpointer);
  Serial.println(gpkt.gmiddle);
  Serial.println(gpkt.gring);
  Serial.println(gpkt.gpinky);
  Serial.println(gpkt.gcheckSum);

  //ARM PACKET
  if(radio.rfAvailable()) {
    radio.rfRead((uint8_t *) & apkt, sizeof(ArmPacket));
  }
  
  int acheckSum1 = apkt.athumb + apkt.apointer + apkt.amiddle + apkt.aring + apkt.apinky;
  if(acheckSum1 == apkt.acheckSum) {
      //IMPORTANT MAPPING
      //motor 1: ring finger
      //motor 2: middle finger
      //motor 3: pointer finger
      //motor 4: pinky
      //motor 5: thumb
      angle1 = map(apkt.athumb,115,850,0,4095);
      //Serial.println(angle1);
      pwm.setPWM(0, 0,angle1);
      angle2 = map(apkt.apointer,115,850,0,4095);
      //Serial.println(angle2);
      pwm.setPWM(1, 0, angle2);
      angle3 = map(apkt.amiddle,115,850,0,4095);
      //Serial.println(angle3);
      pwm.setPWM(2, 0, angle3);
      angle4 = map(apkt.aring,115,850,0,4095);
      //Serial.println(angle4);
      pwm.setPWM(3, 0, angle4);
      angle5 = map(apkt.apinky,115,850,0,4095);
      //Serial.println(angle5);
      pwm.setPWM(4, 0, angle5);
  }
}
