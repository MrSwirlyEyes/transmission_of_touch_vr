#include <radio.h>

#define s0 2 
#define s1 6 
#define s2 7 
#define s3 10
#define SIGpin 0

Radio radio(11);  // Initialize ATmega128RFA1 radio on channel CHANNEL (can be 11-26)

struct Packet {
  int start_check = 0xFEED;

  int sensordat[16];

  int end_check = 0xDEAD;
  
} pkt;

byte controlPin[]={s0,s1,s2,s3};

bool muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };



void setup() {
  analogReference(EXTERNAL);
  pkt.start_check=0xFFFFFEED;
  pkt.end_check=0xFFFFDEAD;
  // put your setup code here, to run once:
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
//  pinMode(SIGpin,INPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  Serial.begin(9600);

}

void loop() {
  
//  test();
  for(int j = 0; j<10; j++) {
    for(int i = 0; i<4; i++){
      digitalWrite(controlPin[i], muxChannel[j][i]);
    }
//    Serial.println(analogRead(SIGpin));
    
    pkt.sensordat[j] = analogRead(SIGpin);
//    Serial.println(pkt.sensordat[j]);
    
  }
  radio.rfWrite((uint8_t *) & pkt, sizeof(Packet));
  Serial.println(pkt.start_check,HEX);
  Serial.println(pkt.end_check,HEX);
}


void test() {
  Serial.println(analogRead(SIGpin));
}

