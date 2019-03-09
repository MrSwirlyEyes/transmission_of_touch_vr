#include "radio.h"
#include "CD74HC4067.h"
#include "TMP36.h"
#include "FSR.h"
#include "Adafruit_PWMServoDriver.h"
#include "PCA9685Servo.h"



#define DEBUG 1

#define RF_CHANNEL 11
#define BAUDRATE 9600



////////////////////////////////
//     ANALOG MULTIPLEXER     //
////////////////////////////////
#define s0 4
#define s1 3
#define s2 6
#define s3 5
#define sig_pin 2

CD74HC4067 multiplexer(s0, s1, s2, s3, sig_pin);



///////////////////////////////////////
//     FORCE SENSITIVE RESISTORS     //
///////////////////////////////////////
#define NUM_FSR 5

#define FSR_PINKY  0
#define FSR_RING   1
#define FSR_MIDDLE 2
#define FSR_INDEX  3
#define FSR_THUMB  4

//int fsr[NUM_FSR] = {0,};

#define FSR_MIN    0
#define FSR_MAX 4095

#define FSR_MIN_PINKY     0
#define FSR_MIN_RING      0
#define FSR_MIN_MIDDLE    0
#define FSR_MIN_INDEX     0
#define FSR_MIN_THUMB     0

#define FSR_MAX_PINKY  1023
#define FSR_MAX_RING   1023
#define FSR_MAX_MIDDLE 1023
#define FSR_MAX_INDEX  1023
#define FSR_MAX_THUMB  1023

//int fsr_min[NUM_FSR] = {
//                            FSR_MIN_PINKY,
//                            FSR_MIN_RING,
//                            FSR_MIN_MIDDLE,
//                            FSR_MIN_INDEX,
//                            FSR_MIN_THUMB,
//                          };
//
//int fsr_max[NUM_FSR] = {
//                            FSR_MAX_PINKY,
//                            FSR_MAX_RING,
//                            FSR_MAX_MIDDLE,
//                            FSR_MAX_INDEX,
//                            FSR_MAX_THUMB,
//                          };

FSR fsr[NUM_FSR] = {
                      FSR(multiplexer,FSR_PINKY,FSR_MIN_PINKY,FSR_MAX_PINKY,FSR_MIN,FSR_MAX),
                      FSR(multiplexer,FSR_RING,FSR_MIN_RING,FSR_MAX_RING,FSR_MIN,FSR_MAX),
                      FSR(multiplexer,FSR_MIDDLE,FSR_MIN_MIDDLE,FSR_MAX_MIDDLE,FSR_MIN,FSR_MAX),
                      FSR(multiplexer,FSR_INDEX,FSR_MIN_INDEX,FSR_MAX_INDEX,FSR_MIN,FSR_MAX),
                      FSR(multiplexer,FSR_THUMB,FSR_MIN_THUMB,FSR_MAX_THUMB,FSR_MIN,FSR_MAX),
                    };



/////////////////////////////////
//     TEMPERATURE SENSORS     //
/////////////////////////////////
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



////////////////////////
//     pwm_driver DRIVER     //
////////////////////////
#define PWM_FREQUENCY 60

Adafruit_PWMServoDriver pwm_driver = Adafruit_PWMServoDriver();



////////////////////
//     SERVOS     //
////////////////////
#define NUM_SERVO 5

#define FLEX_MIN 0
#define FLEX_MAX 1023

#define SERVO_INITIAL 8
#define SERVO_FINAL (SERVO_INITIAL + NUM_SERVO)

#define SERVO_PINKY   8
#define SERVO_RING    9
#define SERVO_MIDDLE 10
#define SERVO_INDEX  11
#define SERVO_THUMB  12

#define SERVO_MIN 185 //225 (light settings)
#define SERVO_MAX 450 //350 (light settings)
#define SERVO_CENTER ((SERVO_MAX + SERVO_MIN) / 2.0)

#define SERVO_MIN_PINKY  185
#define SERVO_MIN_RING   185
#define SERVO_MIN_MIDDLE 185
#define SERVO_MIN_INDEX  185
#define SERVO_MIN_THUMB  185

#define SERVO_MAX_PINKY  450
#define SERVO_MAX_RING   450
#define SERVO_MAX_MIDDLE 450
#define SERVO_MAX_INDEX  450
#define SERVO_MAX_THUMB  450

byte servo_pin[NUM_SERVO] = {
                              SERVO_PINKY,
                              SERVO_RING,
                              SERVO_MIDDLE,
                              SERVO_INDEX,
                              SERVO_THUMB,
                            };

int servo_min[NUM_SERVO] = {
                            SERVO_MIN_PINKY,
                            SERVO_MIN_RING,
                            SERVO_MIN_MIDDLE,
                            SERVO_MIN_INDEX,
                            SERVO_MIN_THUMB,
                          };

int servo_max[NUM_SERVO] = {
                            SERVO_MAX_PINKY,
                            SERVO_MAX_RING,
                            SERVO_MAX_MIDDLE,
                            SERVO_MAX_INDEX,
                            SERVO_MAX_THUMB,
                          };

PCA9685Servo servo[NUM_SERVO] = {
                            PCA9685Servo(pwm_driver,SERVO_PINKY,SERVO_MIN_PINKY,SERVO_MAX_PINKY),
                            PCA9685Servo(pwm_driver,SERVO_RING,SERVO_MIN_RING,SERVO_MAX_RING),
                            PCA9685Servo(pwm_driver,SERVO_MIDDLE,SERVO_MIN_MIDDLE,SERVO_MAX_MIDDLE),
                            PCA9685Servo(pwm_driver,SERVO_INDEX,SERVO_MIN_INDEX,SERVO_MAX_INDEX),
                            PCA9685Servo(pwm_driver,SERVO_THUMB,SERVO_MIN_THUMB,SERVO_MAX_THUMB),
                          };


////////////////////////////
//     COMMUNICATIONS     //
////////////////////////////
struct PacketTX {    
  int fsr[NUM_FSR] = {0,};
  float temp[NUM_TEMP] = {0.0,};
  float checksum = 0.0;
} pkt_tx;

struct PacketRX {  
  int servo[NUM_SERVO] = {0,};
  int checksum = 0;
} pkt_rx;

int checksum;





///////////////////
//     SETUP     //
///////////////////
void setup() {
  #ifdef DEBUG
    Serial.begin(BAUDRATE);
  #endif

  pwm_driver.begin();
  pwm_driver.setPWMFreq(PWM_FREQUENCY);

  for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {
    servo[i].actuate(SERVO_CENTER);
  }

  rfBegin(RF_CHANNEL);

  delay(10000);
  test_servo();
  delay(5000);
}



//////////////////
//     LOOP     //
//////////////////
void loop() {

  read_fsr_sensors();
  #ifdef DEBUG
    print_fsr_sensors();
  #endif
  
  read_temp_sensors();
  #ifdef DEBUG
    print_temp_sensors();
  #endif

  pkt_tx.checksum = 0.0;

  for(int i = 0; i < NUM_FSR; i++) {
    pkt_tx.checksum += pkt_tx.fsr[i];
  }

  for(int i = 0; i < NUM_TEMP; i++) {
    pkt_tx.checksum += pkt_tx.temp[i];
  }
  
  rfWrite((uint8_t *) & pkt_tx,sizeof(PacketTX));

  #ifdef DEBUG
    print_tx_pkt();
  #endif
  

  if(rfAvailable() >= sizeof(PacketRX)) {
    rfRead((uint8_t *) & pkt_rx,sizeof(PacketRX));

    checksum = 0;
    for (int i=0; i < NUM_SERVO; i++) {
      checksum += pkt_rx.servo[i];
    }
    if(checksum == pkt_rx.checksum) {
      #ifdef DEBUG
        print_rx_pkt();
      #endif

      actuate_servos(); 
    } else {
      #ifdef DEBUG
        Serial.println("ERROR");
      #endif
    }
    
    rfFlush();
  }
  delay(5);
}





///////////////////////
//     FUNCTIONS     //
///////////////////////
void read_fsr_sensors() {
  for (int i = 0; i < NUM_FSR; i++) {
//    fsr[i] = multiplexer.read_channel(i);
//    pkt_tx.fsr[i] = multiplexer.read_channel(i);
//    pkt_tx.fsr[i] = constrain(map(multiplexer.read_channel(i),fsr_min[i],fsr_max[i],FSR_MIN,FSR_MAX),FSR_MIN,FSR_MAX);
    pkt_tx.fsr[i] = fsr[i].read();
  }
}

void read_temp_sensors() {
  for (int i = 0; i < NUM_TEMP; i++) {
    temp[i].read(multiplexer.read_channel(temp_pin[i]));
    pkt_tx.temp[i]=temp[i].get_temperature_C();
  }
}

void actuate_servos() {
  
//  for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {
////    pwm_driver.setpwm_driver(i, 0, SERVO_CENTER);
//    pwm_driver.setpwm_driver(i, 0, constrain(map(pkt_rx.servo[i - SERVO_INITIAL],flex_max[i - SERVO_INITIAL],flex_min[i - SERVO_INITIAL],SERVO_MIN,SERVO_MAX),SERVO_MIN,SERVO_MAX));
//  }
  for (int i = 0; i < NUM_SERVO; i++) {
//    pwm_driver.setPWM(servo_pin[i], 0, constrain(map(pkt_rx.servo[i],FLEX_MIN,FLEX_MAX,servo_min[i],servo_max[i]),servo_min[i],servo_max[i]));
    pwm_driver.setPWM(servo_pin[i], 0,map(pkt_rx.servo[i],FLEX_MIN,FLEX_MAX,servo_min[i],servo_max[i]),servo_min[i],servo_max[i]);
    servo[i].actuate(SERVO_CENTER);
  }
}



void test_servo() {
  for (int j = SERVO_MIN; j < SERVO_MAX; j++) {
    for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {  
      pwm_driver.setPWM(i, 0, j);
    }
    delay(1);
  }
  delay(500);
  for (int j = SERVO_MAX; j > SERVO_MIN; j--) {
    for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {  
      pwm_driver.setPWM(i, 0, j);
    }
    delay(1);
  }
  delay(500);
  for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {
    pwm_driver.setPWM(i, 0, SERVO_CENTER);
  }
  delay(500);
}



#ifdef DEBUG
  void print_fsr_sensors() {
    Serial.print("TX: (fsr0,fsr1,fsr2,fsr3,fsr4,fsr5)=(");
    for (byte i = 0 ; i < NUM_FSR ; i++) {
      Serial.print(fsr[i].read());
      if (i < NUM_FSR - 1)
        Serial.print(",");
    }  
    Serial.println(")");
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

  void print_tx_pkt() {
    Serial.print("TX: (fsr_pinky,fsr_ring,fsr_middle,fsr_index,fsr_thumb,temp_pinky,temp_ring,temp_middle,temp_index,temp_thumb)=(");
    for(int i = 0; i < NUM_FSR; i++) {
      Serial.print(pkt_tx.fsr[i]); Serial.print(",");  
    }
    for(int i = 0; i < NUM_TEMP; i++) {
      Serial.print(pkt_tx.temp[i]); Serial.print(",");  
    }
    
    Serial.print(pkt_tx.checksum); 
    Serial.println(")");
  }
  
  void print_rx_pkt() {
    Serial.print("RX: (servo_pinky,servo_ring,servo_middle,servo_index,servo_thumb)=(");
    for(int i = 0; i < NUM_SERVO; i++) {
      Serial.print(pkt_rx.servo[i]); Serial.print(",");      
    }
    Serial.print(pkt_rx.checksum); 
    Serial.println(")");
  }
#endif
