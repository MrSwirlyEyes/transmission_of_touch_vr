#include "radio.h"
#include "CD74HC4067.h"
#include "PCA9685.h"
#include "Thermoelectric.h"
#include "FSR.h"
#include "Vibrotactile.h"



#define DEBUG





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

CD74HC4067 multiplexer(s0, s1, s2, s3, sig_pin);



//////////////////////////
//     FLEX SENSORS     //
//////////////////////////
#define CALIBRATION_SAMPLES 15

#define NUM_FLEX 5

#define FLEX_MIN 0
#define FLEX_MAX 1023

#define FLEX_PINKY 3
#define FLEX_RING 4
#define FLEX_MIDDLE 5
#define FLEX_INDEX 6
#define FLEX_THUMB 7

//pinky  119 276
//ring   190 300
//middle 236 300
//index  236 308
//thumb   90 235

// extension
int FLEX_MIN_PINKY  = 225; //276 
int FLEX_MIN_RING   = 250; //300 // not work
int FLEX_MIN_MIDDLE = 255; //300
int FLEX_MIN_INDEX  = 260; //308 // not work
int FLEX_MIN_THUMB  = 215; //235 

// flexion
int FLEX_MAX_PINKY  = 120; //119
int FLEX_MAX_RING   = 120; //190
int FLEX_MAX_MIDDLE = 135; //236
int FLEX_MAX_INDEX  = 177; //236
int FLEX_MAX_THUMB  =  144; //90



FSR flex[NUM_FLEX] = {
                      FSR(multiplexer,FLEX_PINKY,FLEX_MIN_PINKY,FLEX_MAX_PINKY,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_RING,FLEX_MIN_RING,FLEX_MAX_RING,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_MIDDLE,FLEX_MIN_MIDDLE,FLEX_MAX_MIDDLE,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_INDEX,FLEX_MIN_INDEX,FLEX_MAX_INDEX,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_THUMB,FLEX_MIN_THUMB,FLEX_MAX_THUMB,FLEX_MIN,FLEX_MAX),
                    };



////////////////////////
//     PWM DRIVER     //
////////////////////////
#define PWM_FREQUENCY 60

PCA9685 pwm_driver = PCA9685(0x40);

PCA9685 pwm_driver_et_1 = PCA9685(0x42);



///////////////////////////
//     VIBROTACTILES     //
///////////////////////////
#define NUM_VIBE 6

#ifdef BLACK
  #define VIBE_PINKY 0
  #define VIBE_RING 1
  #define VIBE_MIDDLE 2
  #define VIBE_INDEX 3
  #define VIBE_THUMB 4
  #define VIBE_PALM 5
#else
  #define VIBE_PINKY 5
  #define VIBE_RING 1
  #define VIBE_MIDDLE 2
  #define VIBE_INDEX 3
  #define VIBE_THUMB 4
  #define VIBE_PALM 0

#endif

#define VIBE_MIN 0
#define VIBE_MAX 4095

Vibrotactile vibrotactile[NUM_VIBE] = {
                                        Vibrotactile(pwm_driver,VIBE_PINKY,VIBE_MIN,VIBE_MAX),
                                        Vibrotactile(pwm_driver,VIBE_RING,VIBE_MIN,VIBE_MAX),
                                        Vibrotactile(pwm_driver,VIBE_MIDDLE,VIBE_MIN,VIBE_MAX),
                                        Vibrotactile(pwm_driver,VIBE_INDEX,VIBE_MIN,VIBE_MAX),
                                        Vibrotactile(pwm_driver,VIBE_THUMB,VIBE_MIN,VIBE_MAX),
                                        Vibrotactile(pwm_driver,VIBE_PALM,VIBE_MIN,VIBE_MAX),
                                      };



/////////////////////////////
//     THERMOELECTRICS     //
/////////////////////////////
#define PHASE_COLD 1
#define PHASE_HOT 0

#define NUM_TEC 5

#define TEC_MIN 0
#define TEC_MAX 4095

#define TEC_MAX_HOT 500
#define TEC_MAX_COLD -2048

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

Thermoelectric tec[NUM_TEC] = {
                                Thermoelectric(pwm_driver,TEC_PINKY_HOT,TEC_PINKY_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_RING_HOT,TEC_RING_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_MIDDLE_HOT,TEC_MIDDLE_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_INDEX_HOT,TEC_INDEX_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_THUMB_HOT,TEC_THUMB_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                              };



/////////////////////////////
//     ELECTROTACTILES     //
/////////////////////////////
#define NUM_ET_PIXELS 5


  #define ET_1   4
  #define ET_2   3
  #define ET_3   2
  #define ET_4   1
  #define ET_5   0



#define OFF 0
#define ON 4095

Vibrotactile electrotactile[NUM_ET_PIXELS] = {
                                        Vibrotactile(pwm_driver_et_1,ET_1,OFF,ON),
                                        Vibrotactile(pwm_driver_et_1,ET_2,OFF,ON),
                                        Vibrotactile(pwm_driver_et_1,ET_3,OFF,ON),
                                        Vibrotactile(pwm_driver_et_1,ET_4,OFF,ON),
                                        Vibrotactile(pwm_driver_et_1,ET_5,OFF,ON),
                                        
//                                        Vibrotactile(pwm_driver_et_1,ET_6,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_7,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_8,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_9,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_10,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_11,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_12,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_13,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_14,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_15,OFF,ON),
//                                        Vibrotactile(pwm_driver_et_1,ET_16,OFF,ON),
                                      };



////////////////////////////
//     COMMUNICATIONS     //
////////////////////////////
struct TouchGlovePacket {    
  int flex[NUM_FLEX] = {0,};
  int checksum = 0;
} pkt_tx;

struct RoboticArmPacket {    
  int vibe[NUM_VIBE] = {0,};
  int tec[NUM_TEC] = {0,};
  int checksum = 0;
} pkt_rx;

int checksum = 0;





///////////////////
//     SETUP     //
///////////////////
void setup() {  
  #ifdef DEBUG
    Serial.begin(BAUDRATE);
  #endif

  pwm_driver.begin();
  pwm_driver.set_pwm_freq(PWM_FREQUENCY);

  for(int i = 0; i < 16; i++) {
    pwm_driver.set_pwm(i,0,0);
  }

  pwm_driver_et_1.begin();
  pwm_driver_et_1.set_pwm_freq(PWM_FREQUENCY);

  for(int i = 0; i < 16; i++) {
    pwm_driver_et_1.set_pwm(i,0,0);
  }

  rfBegin(RF_CHANNEL);

  delay(5000);  
//  test_vibe();
//  test_thermoelectrics();
//  test_electrotactiles();
//  calibrate_flex_sensors();
  delay(1000);
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
  
  rfWrite((uint8_t *) & pkt_tx,sizeof(TouchGlovePacket));

  #ifdef DEBUG
    print_tx_pkt();
  #endif

  if(rfAvailable() >= sizeof(RoboticArmPacket)) {   
    rfRead((uint8_t *) & pkt_rx,sizeof(RoboticArmPacket));
    
    checksum = 0;
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
  delay(10);  
}





///////////////////////
//     FUNCTIONS     //
///////////////////////
void read_flex_sensors() {  
  for (int i = 0; i < NUM_FLEX; i++) { 
    pkt_tx.flex[i] = flex[i].read();
//    #ifdef DEBUG
//      Serial.println(flex[i].read_raw());
//    #endif
  }
}



void actuate_vibrotactiles() {
  for (int i = 0; i < NUM_VIBE; i++) {    
    vibrotactile[i].actuate(pkt_rx.vibe[i]);
//    if (pkt_rx.vibe[i] > 2000) {
//      electrotactile[i].actuate(ON);
//    } else {
//      electrotactile[i].actuate(OFF);
//    }
    electrotactile[i].actuate(pkt_rx.vibe[i]);
  }
}



void actuate_thermoelectrics() {
  // TODO!!
//  for (int i = 0; i < NUM_VIBE; i++) {    
//    pwm.set_pwm(i, 0, map(j, 0, 255, VIBE_MIN, VIBE_MAX));    
//  }
//  for (int i = VIBE_INITIAL; i < VIBE_FINAL; i++) {    
//    pwm.set_pwm(i, 0, constrain(map(pkt_rx.vibe[i],FSR_MIN,FSR_MAX,VIBE_MIN,VIBE_MAX),VIBE_MIN,VIBE_MAX));
//  }
  for (int i = 0; i < NUM_TEC; i++) {
    tec[i].actuate(pkt_rx.tec[i]);
  }
}





//----- TEST FUNCTIONS -----//
void test_vibe() {
  #ifdef DEBUG
    Serial.println("Testing vibrotactiles...");
    Serial.println("\tVibrotactiles... ON (increasing)");
  #endif

  for (int i = 0; i < NUM_VIBE; i++) {    
    vibrotactile[i].actuate(4095);
    delay(500);
    vibrotactile[i].off();
    delay(500);
  }

  delay(500);

  for (int j = VIBE_MIN; j < VIBE_MAX; j+=3) {
    for (int i = 0; i < NUM_VIBE; i++) {    
      vibrotactile[i].actuate(j);
    }
    delay(1);
  }
  delay(500);

  #ifdef DEBUG
    Serial.println("\tVibrotactiles... ON (decreasing)");
  #endif

  for (int j = VIBE_MAX; j > VIBE_MIN; j-=3) {
    for (int i = 0; i < NUM_VIBE; i++) {    
      vibrotactile[i].actuate(j);
    }
    delay(1);
  }

  #ifdef DEBUG
    Serial.println("\tVibrotactiles... OFF");
  #endif

  for (int i = 0; i < NUM_VIBE; i++) {    
    vibrotactile[i].off();
  }
  delay(500);

  #ifdef DEBUG
    Serial.println("\tTesting vibrotactiles...complete");
  #endif
}



void test_thermoelectrics() {
  #ifdef DEBUG
    Serial.println("Testing thermoelectrics...");
    Serial.println("\tThermoelectrics... ON");
    Serial.println("\t\tThermoelectrics... COLD");
  #endif

  for (int i = 0; i < NUM_TEC; i++) {
    tec[i].actuate(TEC_MAX_COLD);
  }
  delay(10000);

  #ifdef DEBUG
    Serial.println("\tThermoelectrics... OFF");
  #endif

  for (int i = 0; i < NUM_TEC; i++) {    
    tec[i].off();
  }
  delay(5000);

  #ifdef DEBUG
    Serial.println("\tThermoelectrics... ON");
    Serial.println("\t\tThermoelectrics... HOT");
  #endif

  for (int i = 0; i < NUM_TEC; i++) {
    tec[i].actuate(TEC_MAX_HOT);
  }
  delay(10000);

  #ifdef DEBUG
    Serial.println("\tThermoelectrics... OFF");
  #endif

  for (int i = 0; i < NUM_TEC; i++) {    
    tec[i].off();
  }
  delay(5000);

  #ifdef DEBUG
    Serial.println("\tTesting thermoelectrics...complete");
  #endif
}

void test_electrotactiles() {
  #ifdef DEBUG
    Serial.println("Testing electrotactiles...");
    Serial.println("\tElectrotactiles... ON");
  #endif

  for(int i = 0; i < NUM_ET_PIXELS; i++) {
    electrotactile[i].actuate(ON);
    delay(100);
    electrotactile[i].actuate(OFF);
    delay(1000);
  }

  for(int i = 0; i < NUM_ET_PIXELS; i++) {
    electrotactile[i].actuate(ON);
  }
  delay(100);
  for(int i = 0; i < NUM_ET_PIXELS; i++) {
    electrotactile[i].actuate(OFF);
  }
//  delay(100);

  #ifdef DEBUG
    Serial.println("\tElectrotactiles... OFF");
  #endif

  for(int i = 0; i < NUM_ET_PIXELS; i++) {
    electrotactile[i].actuate(OFF);
  }
  delay(5000);

  #ifdef DEBUG
    Serial.println("\tTesting electrotactiles...complete");
  #endif

  delay(1000);
}




//----- CALIBRATION FUNCTIONS -----//
void calibrate_flex_sensors() {  
  unsigned int flex_avg[NUM_FLEX] = {0,};

  for(int k = 0; k < 2; k++) {
    vibe_signal();
    delay(3000);

    // extension
    for(int j = 0; j < CALIBRATION_SAMPLES; j++) {
      for (int i = 0; i < NUM_FLEX; i++) {    
        flex_avg[i] += flex[i].read_raw();
        delay(5);
      }
    }
    
    // set min
    for (int i = 0; i < NUM_FLEX; i++) {    
      Serial.print(flex_avg[i]); Serial.print("  -->  ");
      Serial.println(flex_avg[i]/CALIBRATION_SAMPLES);
      if(k == 0) {
        flex[i].set_min(flex_avg[i]/CALIBRATION_SAMPLES);
      } else { // k == 1
        flex[i].set_max(flex_avg[i]/CALIBRATION_SAMPLES);
      }
      flex_avg[i]=0;
    }
  }

  // signal end of calibration
  vibe_signal();
}

void vibe_signal() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < NUM_VIBE; i++) {    
      vibrotactile[i].actuate(VIBE_MAX);
    }
    delay(200);
    for (int i = 0; i < NUM_VIBE; i++) {    
      vibrotactile[i].off();
    }
    delay(200);
  }
}





//----- PRINT FUNCTIONS -----//
#ifdef DEBUG
  void print_tx_pkt() {
//    Serial.print("TX: (flex_pinky,flex_ring,flex_middle,flex_index,flex_thumb)=(");
    Serial.print("TX: (");
    for(int i = 0; i < NUM_FLEX; i++) {
      Serial.print(pkt_tx.flex[i]); Serial.print(",");  
    }    
    Serial.print(pkt_tx.checksum); 
    Serial.println(")");
  }
  
  void print_rx_pkt() {
    Serial.print("RX: (");
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
