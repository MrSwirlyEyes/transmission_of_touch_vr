#include "CD74HC4067.h"
#include "TMP36.h"

#define s0 4
#define s1 3
#define s2 6
#define s3 5
#define sig_pin 2

CD74HC4067 demux(s0, s1, s2, s3, sig_pin);

#define NUM_TEMP 5
#define VCC 1.8

#define TEMP_PINKY 11
#define TEMP_RING 12
#define TEMP_MIDDLE 13
#define TEMP_INDEX 14
#define TEMP_THUMB 15

byte temp_pin[NUM_TEMP] = {
                            TEMP_PINKY,
                            TEMP_RING,
                            TEMP_MIDDLE,
                            TEMP_INDEX,
                            TEMP_THUMB
                          };

TMP36 temp[NUM_TEMP] = {
                          TMP36(TEMP_PINKY,VCC),
                          TMP36(TEMP_RING,VCC),
                          TMP36(TEMP_MIDDLE,VCC),
                          TMP36(TEMP_INDEX,VCC),
                          TMP36(TEMP_THUMB,VCC)
                        };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_temp_sensors();

  print_temp_sensors();
}

void print_temp_sensors() {
    Serial.print("TX: (temp0,temp1,temp2,temp3,temp4,temp5)=(");
  for (byte i = 0 ; i < NUM_TEMP ; i++) {
    Serial.print(temp[i].get_temperature_C());
    if (i < NUM_TEMP - 1)
      Serial.print(",");
  }  
    Serial.println(")");
}


void read_temp_sensors() {
  for (int i = 0; i < NUM_TEMP; i++) {
    temp[i].demux_read(demux.read_channel(temp_pin[i]));
//    apkt.temp_readings[i]=temp[i].get_temperature_C();
//    pkt_tx.flex[i] = flex[i];
  }
}
