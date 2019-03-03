#include "Adafruit_PWMServoDriver.h" //download library from adafruit website
#include "CD74HC4067.h" //include this library from github

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//defined vib motor values
#define VIBMIN 0
#define VIBMAX 4095
#define NUM_VIBE 5


// PCB Demux pins
#define s0 22  // 2
#define s1 23  // 6
#define s2 25  // 7
#define s3 24 // 10
// signal Demux pin
#define sig_pin 0

#define thumbVibe 4
#define indexVibe 3
#define middleVibe 2
#define ringVibe 1
#define pinkyVibe 0

#define thumbTec 4
#define indexTec 3
#define middleTec 2
#define ringTec 1
#define pinkyTec 0

CD74HC4067 demux(s0, s1, s2, s3, sig_pin);

#define NUM_FLEX 10
int flex[NUM_FLEX] = {0,};

//#define led 3

//defined flex sensor values
#define FLEXMIN 0
#define FLEXMAX 255

//defined servo values
#define SERVOMIN  130
#define SERVOMAX  290 // Need another define for wrist servo

// Type of Touch Glove
// Cols - how do I make an enumeration?
// 0 for no electrotactiles, 1 for has electrotactile
#define HAS_ET 0

struct SensorPacket {
  int flex1 = 0;
  int flex2 = 0;
  int flex3 = 0;
  int flex4 = 0;
  int flex5 = 0;
} outpkt;

struct ActuatorPacket {
  int vibeThumb = 0;
  int vibeIndex = 0;
  int vibeMiddle = 0;
  int vibeRing = 0;
  int vibePinky = 0;

  int tecThumb = 0;
  int tecIndex = 0;
  int tecMiddle = 0;
  int tecRing = 0;
  int tecPinky = 0;

  /*
  int dirThumb = -1;
  int dirIndex = -1;
  int dirMiddle = -1;
  int dirRing = -1;
  int dirPinky = -1;
  int dirWrist = -1;
  */
  
} inpkt;

//byte * stuff;

byte numRead;

//byte ledVal = 0;

void setup() {

//  pinMode(sig_pin,INPUT);

//  pinMode(s0,OUTPUT);
//  pinMode(s1,OUTPUT);
//  pinMode(s2,OUTPUT);
//  pinMode(s3,OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
//  while (!Serial);
//  Serial.println("ON");

  pwm.begin();
  pwm.setPWMFreq(40);

  for (int i = 0; i < 5; i++) {
    pwm.setPin(i, 0); 
  }

//  analogReference(EXTERNAL);

//  numRead = sizeof(int);
}

void loop() {
//  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
      numRead = Serial.readBytes((byte *) &inpkt, sizeof(inpkt));
      sendFingers();
      updateActuators();
  }

  updateSensors();

//read_flex_sensors();
////
//  print_flex_sensors();
}

void sendFingers() {
    String data = (String) outpkt.flex1 + "," + outpkt.flex2 + "," + outpkt.flex3 + "," + outpkt.flex4 + "," + outpkt.flex5;
//    String data = (String) numRead;
    Serial.println(data);
}

void updateSensors() {

  // Op amp (row "2") flex channels (vert row)
  outpkt.flex1 = demux.read_channel(2);
  outpkt.flex2 = demux.read_channel(1);
  outpkt.flex3 = demux.read_channel(0);
  outpkt.flex4 = demux.read_channel(9);
  outpkt.flex5 = demux.read_channel(8);

  // Non-op amp (row "1") flex channels (horiz row)
//  outpkt.s1 = demux.read_channel(7);
//  outpkt.s2 = demux.read_channel(6);
//  outpkt.s3 = demux.read_channel(5);
//  outpkt.s4 = demux.read_channel(4);
//  outpkt.s5 = demux.read_channel(3);
}

void updateActuators() {
  // Writes to the vibe motors [0-4095]
  pwm.setPWM(thumbVibe,0,inpkt.vibeThumb);
  pwm.setPWM(indexVibe,0,inpkt.vibeIndex);
  pwm.setPWM(middleVibe,0,inpkt.vibeMiddle);
  pwm.setPWM(ringVibe,0,inpkt.vibeRing);
  pwm.setPWM(pinkyVibe,0,inpkt.vibePinky);

  pwm.setPWM(thumbTec,0,inpkt.tecThumb);
  pwm.setPWM(indexTec,0,inpkt.tecIndex);
  pwm.setPWM(middleTec,0,inpkt.tecMiddle);
  pwm.setPWM(ringTec,0,inpkt.tecRing);
  pwm.setPWM(pinkyTec,0,inpkt.tecPinky);

  if (HAS_ET) {
    // set the electrotaciles to do their thing
    // based on the direction of movement for each finger
  }
}


void print_flex_sensors() {
    Serial.print("(flex0,flex1,flex2,flex3,flex4,flex5,flex6,flex7,flex8,flex9)=(");
  for (byte i = 0 ; i < NUM_FLEX ; i++) {
    Serial.print(flex[i]);
    if (i < NUM_FLEX - 1)
      Serial.print(",");
  }  
    Serial.println(")");
}


void read_flex_sensors() {  
  for (int i = 0; i < NUM_FLEX; i++)
    flex[i] = demux.read_channel(i);
}
