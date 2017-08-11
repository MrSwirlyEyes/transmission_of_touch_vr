#include <TMP36.h>

//    ---
//  (    )
//  ----- |
//  |   | |
//  |   |/
//  -----
//  | | |
//  | | |
//  | | |
//  1 2 3

// pin 1 --> 3.3V
// pin 2 --> A0
// pin 3 --> GND

// From ATMEGA128RFA1
// AREF --> 3.3V

// Optional: 0.1uF cap across 3.3V and GND



// Using ATMEGA128RFA1 pins
#define temp_pin 0
#define VCC 3.3 // ATMEGA128RFA1 uses 3.3V

TMP36 tmp(temp_pin,VCC);

/*************************
 *****     SETUP     *****
 *************************/

void setup() {
//  analogReference(EXTERNAL);
  Serial.begin(9600);  
}



/************************
 *****     LOOP     *****
 ************************/
 
void loop() {
  tmp.read();
  print_temp_reading();
  
  delay(10);
}



/************************************
 *****     HELPER FUNCTIONS     *****
 ************************************/

void print_temp_reading() {
  Serial.print("(temp_C,temp_F)=(");
  Serial.print(tmp.get_temperature_C());  
  Serial.print(",");
  Serial.print(tmp.get_temperature_F()); 
  Serial.println(")");
}
