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

#include "radio.h"

struct Packet {  
  int flex[NUM_FLEX] = {0,};
  int checksum = 0;
} pkt_tx,pkt_rx;

int checksum = 0;

void tx() {
  for(int i = 0 ; i < NUM_FLEX; i++) {
    pkt_tx.checksum ^= pkt_tx.flex[i];
  }
  rfWrite((uint8_t *) & pkt_tx,sizeof(Packet));
}

void rx() {
  if(rfAvailable() >= sizeof(Packet)) {
    rfRead((uint8_t *) & pkt_rx,sizeof(Packet));

    for(int i = 0 ; i < NUM_FLEX; i++) {
    checksum ^= pkt_rx.flex[i];
  }

    if(checksum = pkt_rx.checksum) {
      Serial.print("RX: (flex0,flex1,flex2,flex3,flex4,flex5,flex6,flex7,flex8,flex9)=(");    
      for (byte i = 0 ; i < NUM_FLEX ; i++) {
        Serial.print(pkt_rx.flex[i]);
        if (i < NUM_FLEX - 1)
          Serial.print(",");
      }  
      Serial.println(")");
      rfFlush();
    } else {
      Serial.println("ERROR");
    }
  }
}

void print_flex_sensors() {
    Serial.print("TX: (flex0,flex1,flex2,flex3,flex4,flex5,flex6,flex7,flex8,flex9)=(");
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  rfBegin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_flex_sensors();

//  print_flex_sensors();

  tx();

  rx();
}
