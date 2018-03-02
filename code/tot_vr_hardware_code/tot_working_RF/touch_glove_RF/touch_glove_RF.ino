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

  read_flex();
  
  print_flex();

//  transmit();
//  Serial.print(pkt.checkSum); Serial.println(")");
}

void print_flex() {
  Serial.print("(");
  Serial.print(pkt.thumb);  Serial.print(",");
  Serial.print(pkt.pointer);  Serial.print(",");
  Serial.print(pkt.middle); Serial.print(",");
  Serial.print(pkt.ring); Serial.print(",");
  Serial.print(pkt.pinky);  Serial.println(")");
}

void read_flex() {
  delay(5);
  pkt.thumb = demux.read_channel(0);
  delay(5);
  pkt.thumb = demux.read_channel(0);  
  delay(5);
  pkt.pointer = demux.read_channel(1);
  delay(5);
  pkt.pointer = demux.read_channel(1);  
  delay(5);
  pkt.middle = demux.read_channel(2);
  delay(5);
  pkt.middle = demux.read_channel(2);  
  delay(5);
  pkt.ring = demux.read_channel(3);
  delay(5);
  pkt.ring = demux.read_channel(3);  
  delay(5);
  pkt.pinky = demux.read_channel(4);
  delay(5);
  pkt.pinky = demux.read_channel(4);
  delay(5);
  
//  delay(50);

  demux.read_channel(5);
  demux.read_channel(5);
  demux.read_channel(5);
//  delay(5);
//  demux.read_channel(6);
//  delay(5);
//  demux.read_channel(7);
//  delay(5);
//  demux.read_channel(8);
//  delay(5);
//  demux.read_channel(9);
//  delay(5);
//  demux.read_channel(10);
//  delay(5);
//  demux.read_channel(11);
//  delay(5);
//  demux.read_channel(12);
//  delay(5);
//  demux.read_channel(13);
//  delay(5);
//  demux.read_channel(14);
//  delay(5);
//  demux.read_channel(15);
//  delay(5);
  
}

void transmit() {
  pkt.checkSum = pkt.thumb + pkt.pointer + pkt.middle + pkt.ring + pkt.pinky;
  radio.rfWrite((uint8_t *) & pkt, sizeof(Packet));
}

