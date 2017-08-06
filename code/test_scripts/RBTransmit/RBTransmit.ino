#include <radio.h>

#define CHANNEL 11

Radio radio(CHANNEL);  // Initialize ATmega128RFA1 radio on channel CHANNEL (can be 11-26)

struct Packet {
  int test1= 0;

  int test2= 0;

  int test3= 0;
  
};

Packet pkt;

void setup() {
  // put your setup code here, to run once:
  pkt.test1 = 5;
  pkt.test2 = 6;
  pkt.test3 = 7;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  radio.rfWrite((uint8_t *) & pkt, sizeof(Packet));
}
