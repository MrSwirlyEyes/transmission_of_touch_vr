#include "CD74HC4067.h" //include this library from github

//defined mux pins from redboard
#define s0 22  // 2
#define s1 23  // 6
#define s2 25  // 7
#define s3 24 // 10
#define sig_pin 0

CD74HC4067 demux(s0, s1, s2, s3, sig_pin);

#define NUM_FLEX 10
int flex[NUM_FLEX] = {0,};


#include "Adafruit_PWMServoDriver.h" //download library from adafruit website

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//defined vib motor values
#define VIBMIN 0
#define VIBMAX 4095

#include "radio.h"

struct Packet {  
  int flex[NUM_FLEX] = {0,};
  int checksum = 0;
} pkt_tx,pkt_rx;

int checksum = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("start...");

  pwm.begin();
  pwm.setPWMFreq(40);

  rfBegin(11);

  analogReference(EXTERNAL);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_flex_sensors();

//  print_flex_sensors();
//  delay(100);
  
  test_vibrotactiles();
//  delay(100);
//  yield();
//  pwm.end();
//  delay(100);
  rfTx();

  rfRx();
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
  for (int i = 0; i < NUM_FLEX; i++) {
    flex[i] = demux.read_channel(i);  
    pkt_tx.flex[i] = flex[i];
  }
}

void test_vibrotactiles() {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 255; j++) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
//      delay(1);
    }
    delay(100);
    for (int j = 255; j >= 0; j--) {
      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
//      delay(1);
    }
//    delay(50);
  }

//  delay(50);

//  for (int j = 0; j < 255; j++) {
//    for (int i = 0; i < 6; i++) {
//      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
//
//    }
//    delay(2);
//  }
//  delay(100);
//  for (int j = 255; j >= 0; j--) {
//    for (int i = 0; i < 6; i++) {
//      pwm.setPWM(i, 0, map(j, 0, 255, VIBMIN, VIBMAX));
//
//    }
//    delay(2);
//  }
//  delay(100);
}

void rfTx() {
  for(int i = 0 ; i < NUM_FLEX; i++) {
    pkt_tx.checksum ^= pkt_tx.flex[i];
  }
  rfWrite((uint8_t *) & pkt_tx,sizeof(Packet));
}

void rfRx() {
  if(rfAvailable() >= sizeof(Packet)) {
    rfRead((uint8_t *) & pkt_rx,sizeof(Packet));

//      for(int i = 0 ; i < NUM_FLEX; i++) {
//      checksum ^= pkt_rx.flex[i];
//    }

//    if(checksum = pkt_rx.checksum) {
      Serial.print("RX: (");    
      for (byte i = 0 ; i < NUM_FLEX ; i++) {
        Serial.print(pkt_rx.flex[i]);
        if (i < NUM_FLEX - 1)
          Serial.print(",");
      }  
      Serial.println(")");
      rfFlush();
//    } else {
//      Serial.println("ERROR");
//    }
  } else {
    Serial.println("not available?");
  }
}
