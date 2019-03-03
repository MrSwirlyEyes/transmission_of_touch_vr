#include "radio.h"
#include "CD74HC4067.h"
#include "Adafruit_PWMServoDriver.h"
#include "TEC.h"



//#define DEBUG

#define RF_CHANNEL 11
#define BAUDRATE 9600



////////////////////////////////
//     ANALOG MULTIPLEXER     //
////////////////////////////////
#define s0 22
#define s1 23
#define s2 25
#define s3 24
#define sig_pin 0

CD74HC4067 demux(s0, s1, s2, s3, sig_pin);



//////////////////////////
//     FLEX SENSORS     //
//////////////////////////
#define NUM_FLEX 5

#define FLEX_PINKY 8
#define FLEX_RING 9
#define FLEX_MIDDLE 0
#define FLEX_INDEX 1
#define FLEX_THUMB 2

//pinky  119 276
//ring   190 300
//middle 236 300
//index  236 308
//thumb   90 235

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

int flex_min[NUM_FLEX] = {
                            FLEX_MIN_PINKY,
                            FLEX_MIN_RING,
                            FLEX_MIN_MIDDLE,
                            FLEX_MIN_INDEX,
                            FLEX_MIN_THUMB,
                          };

int flex_max[NUM_FLEX] = {
                            FLEX_MAX_PINKY,
                            FLEX_MAX_RING,
                            FLEX_MAX_MIDDLE,
                            FLEX_MAX_INDEX,
                            FLEX_MAX_THUMB,
                          };

byte flex_pin[NUM_FLEX] = {
                            FLEX_PINKY,
                            FLEX_RING,
                            FLEX_MIDDLE,
                            FLEX_INDEX,
                            FLEX_THUMB,
                          };

int flex[NUM_FLEX] = {0,};



////////////////////////
//     PWM DRIVER     //
////////////////////////
#define PWM_FREQUENCY 60

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();



///////////////////////////
//     VIBROTACTILES     //
///////////////////////////
#define NUM_VIBE 5
#define VIBE_INITIAL 0
#define VIBE_FINAL VIBE_INITIAL + NUM_VIBE

//defined vib motor values
#define FSR_MIN 0
#define FSR_MAX 1023
#define VIBE_MIN 0
#define VIBE_MAX 4095



/////////////////////////////
//     THERMOELECTRICS     //
/////////////////////////////
#define PHASE_COLD 0
#define PHASE_HOT 1

#define NUM_TEC 5

#define TECMIN 0
#define TECMAX 4095

#define TECMAX_HOT 4095
#define TECMAX_COLD 4095

#define TEC_PINKY_HOT 6
#define TEC_PINKY_COLD 7

#define TEC_RING_HOT 15
#define TEC_RING_COLD 14

#define TEC_MIDDLE_HOT 13
#define TEC_MIDDLE_COLD 12

#define TEC_INDEX_HOT 11
#define TEC_INDEX_COLD 10

#define TEC_THUMB_HOT 9
#define TEC_THUMB_COLD 8



////////////////////////////
//     COMMUNICATIONS     //
////////////////////////////
struct PacketTX {    
  int flex[NUM_FLEX] = {0,};
  int checksum = 0;
} pkt_tx;

struct PacketRX {    
  int vibe[NUM_VIBE] = {0,};
  float tec[NUM_TEC] = {0.0,};
  float checksum = 0.0;
} pkt_rx;

float checksum = 0.0;





///////////////////
//     SETUP     //
///////////////////
void setup() {  
  #ifdef DEBUG
    Serial.begin(BAUDRATE);
  #endif

  pwm.begin();
  pwm.setPWMFreq(PWM_FREQUENCY);

  rfBegin(RF_CHANNEL);

  delay(10000);
  test_vibe();
  test_thermoelectrics();
  delay(5000);
}



//////////////////
//     LOOP     //
//////////////////
void loop() {  
  read_flex_sensors();
  
  pkt_tx.checksum = 0;
  for(int i = 0; i < NUM_FLEX; i++) {
    pkt_tx.checksum += pkt_tx.flex[i];
  }
  
  rfWrite((uint8_t *) & pkt_tx,sizeof(PacketTX));

  #ifdef DEBUG
    print_tx_pkt();
  #endif

  if(rfAvailable() >= sizeof(PacketRX)) {    
    rfRead((uint8_t *) & pkt_rx,sizeof(PacketRX));
    
    checksum = 0.0;
    for (int i=0; i < NUM_VIBE; i++) {
      checksum += pkt_rx.vibe[i];
    }
    for (int i=0; i < NUM_TEC; i++) {
      checksum += pkt_rx.tec[i];
    }

    if(checksum == pkt_rx.checksum) {

      #ifdef DEBUG
        print_rx_pkt();
      #endif
  
      actuate_vibrotactiles();
  
      actuate_thermoelectrics();

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
void read_flex_sensors() {  
  for (int i = 0; i < NUM_FLEX; i++) { 
    pkt_tx.flex[i] = constrain(demux.read_channel(flex_pin[i]),flex_min[i],flex_max[i]);
  }
}



void actuate_vibrotactiles() {
//  for (int i = 0; i < NUM_VIBE; i++) {    
//    pwm.setPWM(i, 0, map(j, 0, 255, VIBE_MIN, VIBE_MAX));    
//  }
  for (int i = VIBE_INITIAL; i < VIBE_FINAL; i++) {    
    pwm.setPWM(i, 0, constrain(map(pkt_rx.vibe[i - VIBE_INITIAL],FSR_MIN,FSR_MAX,VIBE_MIN,VIBE_MAX),VIBE_MIN,VIBE_MAX));
  }
}



void actuate_thermoelectrics() {
  // TODO!!
//  for (int i = 0; i < NUM_VIBE; i++) {    
//    pwm.setPWM(i, 0, map(j, 0, 255, VIBE_MIN, VIBE_MAX));    
//  }
//  for (int i = VIBE_INITIAL; i < VIBE_FINAL; i++) {    
//    pwm.setPWM(i, 0, constrain(map(pkt_rx.vibe[i],FSR_MIN,FSR_MAX,VIBE_MIN,VIBE_MAX),VIBE_MIN,VIBE_MAX));
//  }
}






void test_vibe() {
  for (int j = VIBE_MIN; j < VIBE_MAX; j+=3) {
    for (int i = VIBE_INITIAL; i < VIBE_FINAL; i++) {    
      pwm.setPWM(i, 0, j);
    }
//    delay(1);
  }
  delay(500);
  for (int j = VIBE_MAX; j > VIBE_MIN; j-=3) {
    for (int i = VIBE_INITIAL; i < VIBE_FINAL; i++) {    
      pwm.setPWM(i, 0, j);
    }
//    delay(1);
  }
  delay(500);
}

void test_thermoelectrics() {
//  tec_pinky.actuate(TECMAX_HOT,PHASE_HOT);
  pwm.setPWM(TEC_PINKY_HOT,0,0);
  pwm.setPWM(TEC_PINKY_COLD,0,TECMAX_HOT);
  pwm.setPWM(TEC_RING_HOT,0,0);
  pwm.setPWM(TEC_RING_COLD,0,TECMAX_HOT);
  pwm.setPWM(TEC_MIDDLE_HOT,0,0);
  pwm.setPWM(TEC_MIDDLE_COLD,0,TECMAX_HOT);
  pwm.setPWM(TEC_INDEX_HOT,0,0);
  pwm.setPWM(TEC_INDEX_COLD,0,TECMAX_HOT);
  pwm.setPWM(TEC_THUMB_HOT,0,0);
  pwm.setPWM(TEC_THUMB_COLD,0,TECMAX_HOT);
  delay(20000);
  pwm.setPWM(TEC_PINKY_HOT,0,0);
  pwm.setPWM(TEC_PINKY_COLD,0,0);
  pwm.setPWM(TEC_RING_HOT,0,0);
  pwm.setPWM(TEC_RING_COLD,0,0);
  pwm.setPWM(TEC_MIDDLE_HOT,0,0);
  pwm.setPWM(TEC_MIDDLE_COLD,0,0);
  pwm.setPWM(TEC_INDEX_HOT,0,0);
  pwm.setPWM(TEC_INDEX_COLD,0,0);
  pwm.setPWM(TEC_THUMB_HOT,0,0);
  pwm.setPWM(TEC_THUMB_COLD,0,0);
  delay(1000);
}


#ifdef DEBUG
//  void print_fsr_sensors() {
//    Serial.print("TX: (fsr0,fsr1,fsr2,fsr3,fsr4,fsr5)=(");
//    for (byte i = 0 ; i < NUM_FSR ; i++) {
//      Serial.print(fsr[i]);
//      if (i < NUM_FSR - 1)
//        Serial.print(",");
//    }  
//    Serial.println(")");
//  }
//  
//  void print_temp_sensors() {
//      Serial.print("TX: (temp0,temp1,temp2,temp3,temp4,temp5)=(");
//    for (byte i = 0 ; i < NUM_TEMP ; i++) {
//      Serial.print(temp[i].get_temperature_C());
//      if (i < NUM_TEMP - 1)
//        Serial.print(",");
//    }  
//      Serial.println(")");
//  }

  void print_tx_pkt() {
    Serial.print("TX: (flex_pinky,flex_ring,flex_middle,flex_index,flex_thumb)=(");
    for(int i = 0; i < NUM_FLEX; i++) {
      Serial.print(pkt_tx.flex[i]); Serial.print(",");  
    }    
    Serial.print(pkt_tx.checksum); 
    Serial.println(")");
  }
  
  void print_rx_pkt() {
    Serial.print("RX: (vibe_pinky,vibe_ring,vibe_middle,vibe_index,vibe_thumb,tec_pinky,tec_ring,tec_middle,tec_index,tec_thumb)=(");
    for(int i = 0; i < NUM_VIBE; i++) {
      Serial.print(pkt_rx.vibe[i]); Serial.print(",");      
    }
    for(int i = 0; i < NUM_TEC; i++) {
      Serial.print(pkt_rx.tec[i]); Serial.print(",");      
    }
    Serial.print(pkt_rx.checksum); 
    Serial.println(")");
  }
#endif
