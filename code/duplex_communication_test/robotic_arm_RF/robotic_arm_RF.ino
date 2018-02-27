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

//servos
struct GlovePacket {
  int gthumb = 0;
  int gpointer = 0;
  int gmiddle = 0;
  int gring = 0;
  int gpinky  = 0;
  int gcheckSum = 0;
} gpkt;

//pressure sensors
struct ArmPacket {
  int athumb = 0;
  int apointer = 0;
  int amiddle = 0;
  int aring = 0;
  int apinky  = 0;
  int acheckSum = 0;
} apkt;

void setup() 
{
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(40);
}

void loop() 
{
  //GLOVE PACKET
  if(radio.rfAvailable()) {
    radio.rfRead((uint8_t *) & gpkt, sizeof(GlovePacket));
  }
   int gcheckSum1 = gpkt.gthumb + gpkt.gpointer + gpkt.gmiddle + gpkt.gring + gpkt.gpinky;
   if(gcheckSum1 == gpkt.gcheckSum) {
       //IMPORTANT MAPPING
       //motor 1: ring finger
       //motor 2: middle finger
       //motor 3: pointer finger
       //motor 4: pinky
       //motor 5: thumb
       angle1 = map(gpkt.gthumb,400,700,0,90);
       Serial.println(angle1);
       turnTo(5, angle1);
       angle2 = map(gpkt.gpointer,400,700,0,90);
       Serial.println(angle2);
       turnTo(3, angle2);
       angle3 = map(gpkt.gmiddle,400,700,0,90);
       Serial.println(angle3);
       turnTo(2, angle3);
       angle4 = map(gpkt.gring,400,700,0,90);
       Serial.println(angle4);
       turnTo(1, angle4);
       angle5 = map(gpkt.gpinky,400,700,0,90);
       Serial.println(angle5);
       turnTo(4, angle5);
   }

  //ARM PACKET
  delay(5);
  apkt.athumb = demux.read_channel(0);
  delay(5);
  apkt.athumb = demux.read_channel(0);
  delay(5);
  apkt.apointer = demux.read_channel(1);
  delay(5);
  apkt.apointer = demux.read_channel(1);
  delay(5);
  apkt.amiddle = demux.read_channel(2);
  delay(5);
  apkt.amiddle = demux.read_channel(2);
  delay(5);
  apkt.aring = demux.read_channel(3);
  delay(5);
  apkt.aring = demux.read_channel(3);
  delay(5);
  apkt.apinky = demux.read_channel(4);
  delay(5);
  apkt.apinky = demux.read_channel(4);
  delay(5);
  apkt.acheckSum = apkt.athumb + apkt.apointer + apkt.amiddle + apkt.aring + apkt.apinky;
  radio.rfWrite((uint8_t *) & apkt, sizeof(ArmPacket));
  Serial.println(apkt.athumb);
  Serial.println(apkt.apointer);
  Serial.println(apkt.amiddle);
  Serial.println(apkt.aring);
  Serial.println(apkt.apinky);
  Serial.println(apkt.acheckSum);
}

void turnTo(int servoNum, int angle)
{
  angle = map(angle, 0, 90, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoNum, 0, angle);
}
