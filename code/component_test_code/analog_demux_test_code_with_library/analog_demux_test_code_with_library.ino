#include <CD74HC4067.h>

#define s0 2 
#define s1 6 
#define s2 7 
#define s3 10
#define sig_pin 0

#define NUM_CHANNELS 16

CD74HC4067 demux(s0,s1,s2,s3,sig_pin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  for(int i = 0 ; i < NUM_CHANNELS ; i++)
    Serial.println(demux.read_channel(i));  
}
