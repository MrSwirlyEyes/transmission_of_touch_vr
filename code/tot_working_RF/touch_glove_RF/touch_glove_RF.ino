//USE THIS IF YOU HAVE Radio.h library instead of radio.h
#include <Radio.h>

//#include <radio.h>

//include this library from github
#include <CD74HC4067.h>

#define s0 2 
#define s1 6 
#define s2 7 
#define s3 10
#define sig_pin 0

Radio radio(11);

struct Packet {
  int thumb = 0;
  int pointer = 0;
  int middle = 0;
  int ring = 0;
  int pinky  = 0;
  int checkSum = 0;
} pkt;

CD74HC4067 demux(s0,s1,s2,s3,sig_pin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  pkt.thumb = demux.read_channel(0);
  pkt.pointer = demux.read_channel(1);
  pkt.middle = demux.read_channel(2);
  pkt.ring = demux.read_channel(3);
  pkt.pinky = demux.read_channel(4);
  pkt.checkSum = pkt.thumb + pkt.pointer + pkt.middle + pkt.ring + pkt.pinky;
  radio.rfWrite((uint8_t *) & pkt, sizeof(Packet));
  Serial.print("(");
  Serial.print(pkt.thumb);  Serial.print(",");
  Serial.print(pkt.pointer);  Serial.print(",");
  Serial.print(pkt.middle); Serial.print(",");
  Serial.print(pkt.ring); Serial.print(",");
  Serial.print(pkt.pinky);  Serial.println(")");
//  Serial.print(pkt.checkSum); Serial.println(")");
}
