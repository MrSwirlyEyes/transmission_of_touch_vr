//#include <radio.h> //include this library from github
#include "Radio.h" //if you're Colin
#include "CD74HC4067.h" //include this library from github
//#include <Wire.h>
#include "Adafruit_PWMServoDriver.h" //download library from adafruit website
int k = 0;
//defined mux pins from redboard
#define s0 22  // 2
#define s1 23  // 6
#define s2 24  // 7
#define s3 25 // 10
#define sig_pin 1

#define DEBUG false

//defined vib motor values
#define PRESSMIN 115
#define PRESSMAX 850

//defined vib motor values
#define VIBMIN 0
#define VIBMAX 4095

//defined vib motor values
#define TECMIN 0
#define TECMAX 4095 // 1.5V MAX on TECs

#define TECMAX_HOT 2095
#define TECMAX_COLD 4095

#define TECDELAY 30

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Radio radio(11);
CD74HC4067 demux(s0, s1, s2, s3, sig_pin);

//flex sensors
struct GlovePacket {
  //  int gThumb, gPointer, gMiddle, gRing, gPinky;
  int gFinger[5] = {0,};
  int gCheckSum = 0;
} gpkt;

//vibration motors
struct ArmPacket {
  //  int aThumb, aPointer, aMiddle, aRing, aPinky;
  int aFinger[5] = {0,};
  float temp_readings[5] = {0.0,};
  float aCheckSum = 0.0;
} apkt;

void setup()
{
  delay(2000);

  //  if(DEBUG)
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(40);
  //  pinMode(sig_pin,INPUT);
  for (int i = 0; i < 5; i++) {
    gpkt.gFinger[i] = 0;
    apkt.aFinger[i] = 0;
  }

//  analogReference(EXTERNAL);

    start_up_sequence();
}

void loop() {
  //  delay(50);
  //GLOVE PACKET: SENDING FLEX SENSOR VALUES FROM GLOVE TO MOVE SERVO MOTORS ON ROBO ARM
  read_flex_sensors();

  transmit();
  //  delay(5);
  //ARM PACKET: RECEIVING PRESSURE SENSOR VALS FROM ROBO ARM TO MOVE VIB MOTORS
  receive();
  radio.rfFlush();
  //  delay(5);
  if (is_valid_pkt()) {
    Serial.println("good");
    for (int i = 0; i < 5; i++) {
      pwm.setPWM(i, 0, map(apkt.aFinger[i], PRESSMIN, PRESSMAX, VIBMIN, VIBMAX));
    }
    Serial.print("(");
    for (int i = 0; i < 5; i++) {
      Serial.print(apkt.aFinger[i]);
      Serial.print(",");
    }

    for (int i = 0; i < 5; i++) {
      //      Serial.print(apkt.temp_readings[i]);
      if ( i < 4 ) {
        Serial.print(",");
      }
    }
    Serial.print(apkt.aCheckSum);
    Serial.println(")");
  } else {
    Serial.println("ERROR");
  }
  //  if(DEBUG)
  print_flex_sensors();
}

void print_flex_sensors() {
  //  Serial.print("(");
  for (byte i = 0 ; i < 5 ; i++) {
    Serial.print(gpkt.gFinger[i]);
    if (i < 4)
      Serial.print(",");
  }
  Serial.println();
  //  Serial.print(")");
}


void read_flex_sensors() {
  //  Serial.print("(");
  //  int a,b,c,d,e;
  for (int i = 0; i < 5; i++) {
    //    a = demux.read_channel(k);
    //    delayMicroseconds(150);
    //    b = demux.read_channel(k);
    //    delayMicroseconds(150);
    //    c = demux.read_channel(k);
    //    delayMicroseconds(150);
    //    d = demux.read_channel(k);
    //    delayMicroseconds(150);
    //    e = demux.read_channel(k);
    //    delayMicroseconds(150);
    //    gpkt.gFinger[k] = (a+b+c)/3;

    //    delay(2);
    //    delay(50);
    gpkt.gFinger[i] = demux.read_channel(i);
    gpkt.gFinger[i] = 1;
    //    delay(5);
    //    Serial.print(gpkt.gFinger[i]);
    //    if(i<4)
    //     Serial.print(",");

  }
  //  gpkt.gFinger[k] = demux.read_channel(k);
  //  gpkt.gFinger[k] = (a+b+c+d+e)/5;
  //  if(k>=5)
  //    k=0;
  //  else
  //    k++;


  gpkt.gCheckSum = gpkt.gFinger[0] + gpkt.gFinger[1] + gpkt.gFinger[2] + gpkt.gFinger[3] + gpkt.gFinger[4];
  //  Serial.print(gpkt.gCheckSum);
  //  Serial.println(")");

}


void transmit() {

  radio.rfWrite((uint8_t *) & gpkt, sizeof(GlovePacket));
}


void receive() {
  if (radio.rfAvailable()) {
    radio.rfRead((uint8_t *) & apkt, sizeof(ArmPacket));
    radio.rfFlush();

    Serial.print("checksum = ");
    Serial.println(apkt.aCheckSum);
  }
}


bool is_valid_pkt() {
  float aCheckSumTot = apkt.aFinger[0] + apkt.aFinger[1] + apkt.aFinger[2] + apkt.aFinger[3] + apkt.aFinger[4]
                       + apkt.temp_readings[0] + apkt.temp_readings[1] + apkt.temp_readings[2] + apkt.temp_readings[3] + apkt.temp_readings[4];

  return (aCheckSumTot != 0 && aCheckSumTot == apkt.aCheckSum);
}


void start_up_sequence() {
  //  delay(3000);
    test_vibrotactiles();
  //  delay(3000);
//  test_thermoelectrics();
  //  delay(3000);
}


void test_vibrotactiles() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 255; j++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
      delay(2);
    }
    delay(100);
    for (int j = 255; j >= 0; j--) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
      delay(2);
    }
    delay(100);
  }

  delay(100);

  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < 6; i++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));

    }
    delay(2);
  }
  delay(100);
  for (int j = 255; j >= 0; j--) {
    for (int i = 0; i < 6; i++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));

    }
    delay(2);
  }
  delay(100);
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

  delay(500);
  pwm.setPWM(14, 0, TECMAX);
  pwm.setPWM(12, 0, TECMAX);
  pwm.setPWM(10, 0, TECMAX);
  pwm.setPWM(8, 0, TECMAX);
  pwm.setPWM(6, 0, TECMAX);
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
