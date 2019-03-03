#include "Adafruit_PWMServoDriver.h" //download library from adafruit website

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//defined vib motor values
#define VIBMIN 0
#define VIBMAX 4095

#define NUM_VIBE 4

#include "radio.h"


struct Packet {  
  int vibe[NUM_VIBE] = {0,};
  int checksum = 0;
} pkt_tx,pkt_rx;

int checksum = 0;

void tx() {
  for(int i = 0 ; i < NUM_VIBE; i++) {
    pkt_tx.vibe[i] = 125;
  }
  for(int i = 0 ; i < NUM_VIBE; i++) {
    pkt_tx.checksum ^= pkt_tx.vibe[i];
  }
  rfWrite((uint8_t *) & pkt_tx,sizeof(Packet));
}

void rx() {
  if(rfAvailable() >= sizeof(Packet)) {
    rfRead((uint8_t *) & pkt_rx,sizeof(Packet));

    for(int i = 0 ; i < NUM_VIBE; i++) {
    checksum ^= pkt_rx.vibe[i];
  }

    if(checksum = pkt_rx.checksum) {
      Serial.print("RX: (flex0,flex1,flex2,flex3,flex4,flex5,flex6,flex7,flex8,flex9)=(");    
      for (byte i = 0 ; i < NUM_VIBE ; i++) {
        Serial.print(pkt_rx.vibe[i]);
        if (i < NUM_VIBE - 1)
          Serial.print(",");
      }  
      Serial.println(")");
      rfFlush();
    } else {
      Serial.println("ERROR");
    }
  }
}


void test_vibrotactiles() {
  for (int i = 0; i < NUM_VIBE; i++) {
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
    for (int i = 0; i < NUM_VIBE; i++) {
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(40);

  rfBegin(11);
}

void loop() {
  // put your main code here, to run repeatedly:
//  test_vibrotactiles();

//  print_flex_sensors();

  tx();

  rx();
}
