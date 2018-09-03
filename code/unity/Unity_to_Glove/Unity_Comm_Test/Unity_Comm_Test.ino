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

struct SensorPacket {
  int flex1 = 0;
  int flex2 = 0;
  int flex3 = 0;
  int flex4 = 0;
  int flex5 = 0;
} outpkt;

struct ActuatorPacket {
  int pwm1 = 0;
  int pwm2 = 0;
  int pwm3 = 0;
  int pwm4 = 0;
  int pwm5 = 0;
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
//  pwm.setPWMFreq(40);
//
//  for (int i = 0; i < 5; i++) {
//    pwm.setPin(i, 0); 
//  }

//  analogReference(EXTERNAL);

//  numRead = sizeof(int);
}

void loop() {
//  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
      numRead = Serial.readBytes((byte *) &inpkt, sizeof(inpkt));
      sendFingers();
//      analogWrite(led, ledVal++);
      updateActuators();
  }

//  updateSensors();

read_flex_sensors();
//
  print_flex_sensors();
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

//  Serial.print("(");
//  Serial.print(outpkt.flex1); Serial.print(",");
//  Serial.print(outpkt.flex2); Serial.print(",");
//  Serial.print(outpkt.flex3); Serial.print(",");
//  Serial.print(outpkt.flex4); Serial.print(",");
//  Serial.print(outpkt.flex5); 
//  Serial.println(")");

  // Non-op amp (row "1") flex channels (horiz row)
//  outpkt.s1 = demux.read_channel(7);
//  outpkt.s2 = demux.read_channel(6);
//  outpkt.s3 = demux.read_channel(5);
//  outpkt.s4 = demux.read_channel(4);
//  outpkt.s5 = demux.read_channel(3);
}

void updateActuators() {
  // Writes to the vibe motors [0-4095]
  pwm.setPWM(0,0,inpkt.pwm1);
  pwm.setPWM(1,0,inpkt.pwm2);
  pwm.setPWM(2,0,inpkt.pwm3);
  pwm.setPWM(3,0,inpkt.pwm4);
  pwm.setPWM(4,0,inpkt.pwm5);
  
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
    
//  byte DELAY = 5;
////  digitalWrite(s1,0);
////  digitalWrite(s1,0);
////  digitalWrite(s2,0);
////  digitalWrite(s3,0);
//  digitalWrite(s3,0);
//  digitalWrite(s2,0);
//  digitalWrite(s1,0);
//  digitalWrite(s0,0);
//  flex[0] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,0);
////  digitalWrite(s2,0);
////  digitalWrite(s1,0);
////  digitalWrite(s0,1);
//  digitalWrite(s0,0);
//  digitalWrite(s1,0);
//  digitalWrite(s2,0);
//  digitalWrite(s3,1);
//  flex[1] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,0);
////  digitalWrite(s2,0);
////  digitalWrite(s1,1);
////  digitalWrite(s0,0);
//  digitalWrite(s0,0);
//  digitalWrite(s1,0);
//  digitalWrite(s2,1);
//  digitalWrite(s3,0);
//  flex[2] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,0);
////  digitalWrite(s2,0);
////  digitalWrite(s1,1);
////  digitalWrite(s0,1);
//  digitalWrite(s0,0);
//  digitalWrite(s1,0);
//  digitalWrite(s2,1);
//  digitalWrite(s3,1);
//  flex[3] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,0);
////  digitalWrite(s2,1);
////  digitalWrite(s1,0);
////  digitalWrite(s0,0);
//  digitalWrite(s0,0);
//  digitalWrite(s1,1);
//  digitalWrite(s2,0);
//  digitalWrite(s3,0);
//  flex[4] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,0);
////  digitalWrite(s2,1);
////  digitalWrite(s1,0);
////  digitalWrite(s0,1);
//  digitalWrite(s0,0);
//  digitalWrite(s1,1);
//  digitalWrite(s2,0);
//  digitalWrite(s3,1);
//  flex[5] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,0);
////  digitalWrite(s2,1);
////  digitalWrite(s1,1);
////  digitalWrite(s0,0);
//  digitalWrite(s0,0);
//  digitalWrite(s1,1);
//  digitalWrite(s2,1);
//  digitalWrite(s3,0);
//  flex[6] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,0);
////  digitalWrite(s2,1);
////  digitalWrite(s1,1);
////  digitalWrite(s0,1);
//  digitalWrite(s0,0);
//  digitalWrite(s1,1);
//  digitalWrite(s2,1);
//  digitalWrite(s3,1);
//  flex[7] = analogRead(sig_pin);
//
//  delay(DELAY);
//
////  digitalWrite(s3,1);
////  digitalWrite(s2,0);
////  digitalWrite(s1,0);
////  digitalWrite(s0,0);
//  digitalWrite(s0,1);
//  digitalWrite(s1,0);
//  digitalWrite(s2,0);
//  digitalWrite(s3,0);
//  flex[8] = analogRead(sig_pin); 
//
//   delay(DELAY);
//
////  digitalWrite(s3,1);
////  digitalWrite(s2,0);
////  digitalWrite(s1,0);
////  digitalWrite(s0,1);
//  digitalWrite(s0,1);
//  digitalWrite(s1,0);
//  digitalWrite(s2,0);
//  digitalWrite(s3,1);
//  flex[9] = analogRead(sig_pin);
//
//  delay(DELAY);

//  digitalWrite(s0,1);
//  digitalWrite(s1,0);
//  digitalWrite(s2,1);
//  digitalWrite(s3,0);
//  flex[10] = analogWrite(sig_pin);
//
//  digitalWrite(s0,1);
//  digitalWrite(s1,0);
//  digitalWrite(s2,1);
//  digitalWrite(s3,1);
//  flex[11] = analogWrite(sig_pin);
//
//  digitalWrite(s0,1);
//  digitalWrite(s1,1);
//  digitalWrite(s2,0);
//  digitalWrite(s3,0);
//  flex[12] = analogWrite(sig_pin);
//
//  digitalWrite(s0,1);
//  digitalWrite(s1,1);
//  digitalWrite(s2,0);
//  digitalWrite(s3,1);
//  flex[13] = analogWrite(sig_pin);
//
//  digitalWrite(s0,1);
//  digitalWrite(s1,1);
//  digitalWrite(s2,1);
//  digitalWrite(s3,0);
//  flex[14] = analogWrite(sig_pin);
//
//  digitalWrite(s0,1);
//  digitalWrite(s1,1);
//  digitalWrite(s2,1);
//  digitalWrite(s3,1);
//  flex[15] = analogWrite(sig_pin);
}
