#include <Radio.h>  // Onboard RF
#define CHANNEL 11

Radio radio(CHANNEL);

#include <CD74HC4067.h> // Demux
#define s0 0  // TODO: ASSIGN THESE
#define s1 1
#define s2 2
#define s3 3
#define sig_pin 0

CD74HC4067 demux(s0,s1,s2,s3,sig_pin);

#include <TMP36.h>  // Temperature sensor
#define NUM_TEMP_SENSORS 5
#define VCC 3.3

#define TEMP_PINKY 11
#define TEMP_RING 12
#define TEMP_MIDDLE 13
#define TEMP_INDEX 14
#define TEMP_THUMB 15

TMP36 temp[NUM_TEMP_SENSORS] = {
                                  TMP36(TEMP_PINKY,VCC),
                                  TMP36(TEMP_RING,VCC),
                                  TMP36(TEMP_MIDDLE,VCC),
                                  TMP36(TEMP_INDEX,VCC),
                                  TMP36(TEMP_THUMB,VCC)
                              };


#define NUM_TACTILE_SENSORS 5


// TODO: #include pwm driver lib
#include <Servo.h>
#define NUM_SERVO_ACTUATORS 5

#define SERVO_PINKY 0
#define SERVO_RING 1
#define SERVO_MIDDLE 2
#define SERVO_INDEX 3
#define SERVO_THUMB 4

byte servo_pins[NUM_SERVO_ACTUATORS] = {
                                          SERVO_PINKY,
                                          SERVO_RING,
                                          SERVO_MIDDLE,
                                          SERVO_INDEX,
                                          SERVO_THUMB
                                      };

Servo servo[NUM_SERVO_ACTUATORS];

struct Packet_TX {
  int checksum;

  byte temp[NUM_TEMP_SENSORS];
  byte tactile[NUM_TACTILE_SENSORS];
} pkt_tx;



#define NUM_STRAIN_SENSORS

struct Packet_RX {
  int checksum;

  byte strain[NUM_STRAIN_SENSORS];  
} pkt_rx;

void setup() {
  // put your setup code here, to run once:
  initialize_servos();
}

void loop() {
  // put your main code here, to run repeatedly:

}



/************************************
 *****     HELPER FUNCTIONS     *****
 ************************************/

void initialize_servos() {
  for(byte i=0;i<NUM_SERVO_ACTUATORS;i++) {
    servo[i].attach(servo_pins[i]);
    delay(10);
    servo[i].write(0);
    delay(10);
  }
}

