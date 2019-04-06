#include "radio.h"


struct Packet {  
  int a = 0;
  int b = 0;
} pkt_tx,pkt_rx;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  rfBegin(11);

  pkt_tx.a = 1;
  pkt_tx.b = 2;
}

void loop() {
  // put your main code here, to run repeatedly:
  rfWrite((uint8_t *) & pkt_tx,sizeof(Packet));

  Serial.print("TX: (a,b)=(");
  Serial.print(pkt_tx.a); Serial.print(",");
  Serial.print(pkt_tx.b); 
  Serial.println(")");

  if(rfAvailable() >= sizeof(Packet)) {
    rfRead((uint8_t *) & pkt_rx,sizeof(Packet));

    Serial.print("RX: (a,b)=(");
    Serial.print(pkt_rx.a); Serial.print(",");
    Serial.print(pkt_rx.b); 
    Serial.println(")");
    rfFlush();
  }
  delay(10);
}
