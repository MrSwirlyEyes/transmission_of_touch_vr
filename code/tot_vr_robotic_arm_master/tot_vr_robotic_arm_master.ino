#include "radio.h"
#include "CD74HC4067.h"
#include "TMP36.h"
#include "Adafruit_PWMServoDriver.h"

//#define DEBUG

#define RF_CHANNEL 11
#define BAUDRATE 9600

#define s0 4
#define s1 3
#define s2 6
#define s3 5
#define sig_pin 2

CD74HC4067 demux(s0, s1, s2, s3, sig_pin);

#define NUM_FSR 5

int fsr[NUM_FSR] = {0,};

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





#define NUM_SERVO 5
#define SERVO_INITIAL 8
#define SERVO_FINAL (SERVO_INITIAL + NUM_SERVO)

#define FLEX_MIN_PINKY  119
#define FLEX_MIN_RING   190
#define FLEX_MIN_MIDDLE 236
#define FLEX_MIN_INDEX  236
#define FLEX_MIN_THUMB   90

#define FLEX_MAX_PINKY  276
#define FLEX_MAX_RING   300
#define FLEX_MAX_MIDDLE 300
#define FLEX_MAX_INDEX  308
#define FLEX_MAX_THUMB  235

int flex_min[NUM_SERVO] = {
                            FLEX_MIN_PINKY,
                            FLEX_MIN_RING,
                            FLEX_MIN_MIDDLE,
                            FLEX_MIN_INDEX,
                            FLEX_MIN_THUMB,
                          };

int flex_max[NUM_SERVO] = {
                            FLEX_MAX_PINKY,
                            FLEX_MAX_RING,
                            FLEX_MAX_MIDDLE,
                            FLEX_MAX_INDEX,
                            FLEX_MAX_THUMB,
                          };

#define SERVO_MIN 185 //225 (light settings)
#define SERVO_MAX 450 //350 (light settings)
#define SERVO_CENTER ((SERVO_MAX + SERVO_MIN) / 2.0)

#define PWM_FREQUENCY 60
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

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

void setup() {
  #ifdef DEBUG
    Serial.begin(BAUDRATE);
  #endif

  pwm.begin();
  pwm.setPWMFreq(PWM_FREQUENCY);

  for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {
    pwm.setPWM(i, 0, SERVO_CENTER);
  }

  rfBegin(RF_CHANNEL);

  delay(10000);
  test_servo();
  delay(5000);
}

void loop() {

  read_fsr_sensors();
//  #ifdef DEBUG
//    print_fsr_sensors();
//  #endif
  
  read_temp_sensors();
//  #ifdef DEBUG
//    print_temp_sensors();
//  #endif

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

void read_fsr_sensors() {
  for (int i = 0; i < NUM_FSR; i++) {
//    fsr[i] = demux.read_channel(i);
    pkt_tx.fsr[i] = demux.read_channel(i);
//    pkt_tx.fsr[i] = constrain(map(pkt_rx.servo[i],flex_max[i],flex_min[i],SERVO_MIN,SERVO_MAX),SERVO_MIN,SERVO_MAX);
  }
}

void read_temp_sensors() {
  for (int i = 0; i < NUM_TEMP; i++) {
    temp[i].demux_read(demux.read_channel(temp_pin[i]));
    pkt_tx.temp[i]=temp[i].get_temperature_C();
  }
}

void actuate_servos() {
  
  for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {
//    pwm.setPWM(i, 0, SERVO_CENTER);
    pwm.setPWM(i, 0, constrain(map(pkt_rx.servo[i - SERVO_INITIAL],flex_max[i - SERVO_INITIAL],flex_min[i - SERVO_INITIAL],SERVO_MIN,SERVO_MAX),SERVO_MIN,SERVO_MAX));
    
  }
}





#ifdef DEBUG
  void print_fsr_sensors() {
    Serial.print("TX: (fsr0,fsr1,fsr2,fsr3,fsr4,fsr5)=(");
    for (byte i = 0 ; i < NUM_FSR ; i++) {
      Serial.print(fsr[i]);
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




void test_servo() {
  for (int j = SERVO_MIN; j < SERVO_MAX; j++) {
    for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {  
      pwm.setPWM(i, 0, j);
    }
    delay(1);
  }
  delay(500);
  for (int j = SERVO_MAX; j > SERVO_MIN; j--) {
    for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {  
      pwm.setPWM(i, 0, j);
    }
    delay(1);
  }
  delay(500);
  for (int i = SERVO_INITIAL; i < SERVO_FINAL; i++) {
    pwm.setPWM(i, 0, SERVO_CENTER);
  }
  delay(500);
}
