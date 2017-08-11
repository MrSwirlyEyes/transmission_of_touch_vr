// Based on: https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor

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



/*************************
 *****     SETUP     *****
 *************************/

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(9600);  
}



/************************
 *****     LOOP     *****
 ************************/
 
void loop() {
  int raw_temp_reading = analogRead(temp_pin);

  float voltage = get_voltage(raw_temp_reading);

  float temp_C = get_temp_C(voltage);

  print_temp_reading(temp_C);
  
  delay(100);
}



/************************************
 *****     HELPER FUNCTIONS     *****
 ************************************/

void print_temp_reading(float temp_C) {
  Serial.print("(temp_C,temp_F)=(");
  Serial.print(temp_C);
  Serial.print(",");
  Serial.print(get_temp_F(temp_C));

  Serial.println(")");
}

float get_voltage(int raw_temp_reading) {
  return ((raw_temp_reading * VCC) / 1024.0);
}

float get_temp_C(float voltage) {
  return (voltage - 0.5) * 100;
}

float get_temp_F(float temp_C) {
  return ((temp_C * (9.0 / 5.0)) + 32.0);
}

