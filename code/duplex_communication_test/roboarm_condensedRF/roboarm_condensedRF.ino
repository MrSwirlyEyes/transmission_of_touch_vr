//#include <radio.h> //include this library from github
#include <Radio.h> //if you're Colin
#include <CD74HC4067.h> //include this library from github
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //download library from adafruit website

#include <TMP36.h>  // Temperature sensor
#define NUM_TEMP_SENSORS 5
#define VCC 3.3

#define TEMP_PINKY 11
#define TEMP_RING 12
#define TEMP_MIDDLE 13
#define TEMP_INDEX 14
#define TEMP_THUMB 15

byte temp_pin[NUM_TEMP_SENSORS] = {
                                    TEMP_PINKY,
                                    TEMP_RING,
                                    TEMP_MIDDLE,
                                    TEMP_INDEX,
                                    TEMP_THUMB
                                  };

TMP36 temp[NUM_TEMP_SENSORS] = {
                                  TMP36(TEMP_PINKY,VCC),
                                  TMP36(TEMP_RING,VCC),
                                  TMP36(TEMP_MIDDLE,VCC),
                                  TMP36(TEMP_INDEX,VCC),
                                  TMP36(TEMP_THUMB,VCC)
                              };

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
#define SERVOMAX  290 // Need another define for wrist servo

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
  float temp_readings[5]={0.0,};
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

  for (int i = 0; i < 6; i++) {
    pwm.setPWM(i, 0, 0); 
  }

//  delay(3000);

//  start_up_sequence();
//  delay(3000);
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

  // Get temp readings
  for (int i = 0; i < 5; i++){
    
//    apkt.aFinger[i] = demux.read_channel(i);
//    Serial.println(demux.read_channel(temp_pin[i]));
    temp[i].demux_read(demux.read_channel(temp_pin[i]));
    apkt.temp_readings[i]=temp[i].get_temperature_C();
    delay(7);
//    apkt.aFinger[i] = demux.read_channel(i);
    Serial.println(demux.read_channel(temp_pin[i]));
    temp[i].demux_read(demux.read_channel(temp_pin[i]));
    apkt.temp_readings[i]=temp[i].get_temperature_C();
    delay(7);
  }
  
  apkt.aCheckSum = apkt.aThumb + apkt.aPointer + apkt.aMiddle + apkt.aRing + apkt.aPinky;
  radio.rfWrite((uint8_t *) & apkt, sizeof(ArmPacket));

  print_sensor_readings();
  
}

void print_sensor_readings() {
  Serial.print("(");
  for (int i = 0 ; i < 5 ; i++) {
    Serial.print(apkt.aFinger[i]);
//    if (i < 4)
    Serial.print(",");
  }
  for (int i = 0 ; i < 5 ; i++) {
    Serial.print(apkt.temp_readings[i]);
    if (i < 4)
    Serial.print(",");
  }
  Serial.println(")");
}

void start_up_sequence() {

  // Curl fingers individually
  for (int i = 0; i < 5; i++) {
    for (int j = 0 ; j < 90 ; j++) {
      pwm.setPWM(i, 0, map(j,0,90,SERVOMIN, SERVOMAX));
      delay(10);
    }
  }

  delay(500);

  // Relax fingers individually
  for (int i = 0; i < 5; i++) {
    for (int j = 90 ; j >= 0 ; j--) {
      pwm.setPWM(i, 0, map(j,0,90,SERVOMIN, SERVOMAX));
      delay(10);
    }
  }

  delay(500);

  // Curl fingers simultaneously
  for (int j = 0 ; j < 90 ; j++) {
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(i, 0, map(j,0,90,SERVOMIN, SERVOMAX));
    }
    delay(10);
  }

  delay(500);  

  // Rotate wrist (high)
  for (int j = 0 ; j < 90 ; j++) {
    pwm.setPWM(5, 0, map(j,0,90,SERVOMIN, SERVOMAX));
    delay(10);
  }

  delay(500);

  // Rotate wrist (low)
  for (int j = 90 ; j >= 0 ; j--) {
    pwm.setPWM(5, 0, map(j,0,90,SERVOMIN, SERVOMAX));
    delay(10);
  }

  delay(500);

  // Relax fingers simultaneously
  for (int j = 90 ; j >= 0 ; j--) {
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(i, 0, map(j,0,90,SERVOMIN, SERVOMAX));
    }
    delay(10);
  }  

  delay(500);
}

