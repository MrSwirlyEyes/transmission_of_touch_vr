#include <radio.h>

#define CHANNEL 11

Radio radio(CHANNEL);  // Initialize ATmega128RFA1 radio on channel CHANNEL (can be 11-26)

struct Packet {
  int test1= 0;

  int test2= 0;

  int test3= 0;
  
};

Packet pkt;




void setup()
{
  Serial.begin(9600);
  
}

void loop()
{
  if(radio.rfAvailable()) {           // If data receievd on radio...
    radio.rfRead((uint8_t *) & pkt,sizeof(Packet)); // ... send it out serial.
    Serial.print("Test1: ");
    Serial.println(pkt.test1);
    Serial.print("Test2: ");
    Serial.println(pkt.test2);
    Serial.print("Test3: ");
    Serial.println(pkt.test3);
  }
  
}
