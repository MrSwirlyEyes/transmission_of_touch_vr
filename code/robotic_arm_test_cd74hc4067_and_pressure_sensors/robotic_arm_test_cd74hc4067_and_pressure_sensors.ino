#include "CD74HC4067.h"

#define s0 4
#define s1 3
#define s2 6
#define s3 5
#define sig_pin 2

CD74HC4067 demux(s0, s1, s2, s3, sig_pin);

#define NUM_FSR 6

int fsr[NUM_FSR] = {0,};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_fsr_sensors();

  print_fsr_sensors();
}

void print_fsr_sensors() {
    Serial.print("TX: (fsr0,fsr1,fsr2,fsr3,fsr4,fsr5)=(");
  for (byte i = 0 ; i < NUM_FSR ; i++) {
    Serial.print(fsr[i]);
    if (i < NUM_FSR - 1)
      Serial.print(",");
  }  
    Serial.println(")");
}


void read_fsr_sensors() {
  for (int i = 0; i < NUM_FSR; i++) {
    fsr[i] = demux.read_channel(i);
//    pkt_tx.flex[i] = flex[i];
  }
}
