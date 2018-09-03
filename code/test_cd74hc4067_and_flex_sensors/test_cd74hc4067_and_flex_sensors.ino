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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_flex_sensors();

  print_flex_sensors();
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
  for (int i = 0; i < NUM_FLEX; i++)
    flex[i] = demux.read_channel(i);  
}
