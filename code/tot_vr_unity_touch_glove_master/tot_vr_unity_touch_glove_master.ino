#include "PCA9685.h" //download library from adafruit website
#include "CD74HC4067.h" //include this library from github
#include "Thermoelectric.h"
#include "FSR.h"
#include "Vibrotactile.h"


#define BAUDRATE 9600


// Type of Touch Glove
// Cols - how do I make an enumeration?
// 0 for no electrotactiles, 1 for has electrotactile
#define HAS_ET 1

byte numRead;



////////////////////////////////
//     ANALOG MULTIPLEXER     //
////////////////////////////////

// Select pins
#define s0 22  // 2
#define s1 23  // 6
#define s2 25  // 7
#define s3 24 // 10

// Signal pin
#define sig_pin 0

CD74HC4067 multiplexer(s0, s1, s2, s3, sig_pin);
//#define DEBUG 0


//////////////////////////
//     FLEX SENSORS     //
//////////////////////////
#define NUM_FLEX 5

int flex_mapped[NUM_FLEX] = {0,};

#define FLEX_MIN 0
#define FLEX_MAX 90

#define FLEX_PINKY 3
#define FLEX_RING 4
#define FLEX_MIDDLE 5
#define FLEX_INDEX 6
#define FLEX_THUMB 7

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

FSR flex[NUM_FLEX] = {
                      FSR(multiplexer,FLEX_THUMB,FLEX_MIN_THUMB,FLEX_MAX_THUMB,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_INDEX,FLEX_MIN_INDEX,FLEX_MAX_INDEX,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_MIDDLE,FLEX_MIN_MIDDLE,FLEX_MAX_MIDDLE,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_RING,FLEX_MIN_RING,FLEX_MAX_RING,FLEX_MIN,FLEX_MAX),
                      FSR(multiplexer,FLEX_PINKY,FLEX_MIN_PINKY,FLEX_MAX_PINKY,FLEX_MIN,FLEX_MAX),
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

#define VIBMIN 0
#define VIBMAX 4095

#define VIBE_PINKY 0
#define VIBE_RING 1
#define VIBE_MIDDLE 2
#define VIBE_INDEX 3
#define VIBE_THUMB 4
#define VIBE_PALM 5

#define VIBE_MIN 0
#define VIBE_MAX 4095

Vibrotactile vibrotactile[NUM_VIBE] = {
                                          Vibrotactile(pwm_driver,VIBE_THUMB,VIBE_MIN,VIBE_MAX),
                                          Vibrotactile(pwm_driver,VIBE_INDEX,VIBE_MIN,VIBE_MAX),
                                          Vibrotactile(pwm_driver,VIBE_MIDDLE,VIBE_MIN,VIBE_MAX),
                                          Vibrotactile(pwm_driver,VIBE_RING,VIBE_MIN,VIBE_MAX),
                                          Vibrotactile(pwm_driver,VIBE_PINKY,VIBE_MIN,VIBE_MAX),
                                          Vibrotactile(pwm_driver,VIBE_PALM,VIBE_MIN,VIBE_MAX),
                                        };



/////////////////////////////
//     THERMOELECTRICS     //
/////////////////////////////

#define NUM_TEC 5

#define PHASE_COLD 1
#define PHASE_HOT 0

#define TEC_MIN 0
#define TEC_MAX 4095

#define TEC_MAX_HOT 1024
#define TEC_MAX_COLD 4095

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
                                Thermoelectric(pwm_driver,TEC_THUMB_HOT,TEC_THUMB_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_INDEX_HOT,TEC_INDEX_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_MIDDLE_HOT,TEC_MIDDLE_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_RING_HOT,TEC_RING_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                                Thermoelectric(pwm_driver,TEC_PINKY_HOT,TEC_PINKY_COLD,TEC_MIN,TEC_MAX_COLD,TEC_MIN,TEC_MAX_HOT,PHASE_HOT),
                              };



/////////////////////////////
//     ELECTROTACTILES     //
/////////////////////////////
#define NUM_ET_PIXELS 5
#define NUM_ET 5

#define ET_1   1
#define ET_2   0
#define ET_3   2
#define ET_4   3
#define ET_5   4
#define ET_6   5
#define ET_7   6
#define ET_8   7
#define ET_9   8
#define ET_10  9
#define ET_11 10
#define ET_12 11
#define ET_13 12
#define ET_14 13
#define ET_15 14
#define ET_16 15

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


struct SensorPacket {
  int flex[NUM_FLEX] = {0, 0, 0, 0, 0};
} pkt_tx;

struct ActuatorPacket {
  int msgType = 0;

//  int vibes[NUM_VIBE] = {0,};
//  int heats[NUM_TEC] = {0,};
//  int dires[5] = {0,};
  
  int vibeThumb   = 0;
  int vibeIndex   = 0;
  int vibeMiddle  = 0;
  int vibeRing    = 0;
  int vibePinky   = 0;
  int vibePalm    = 0;
  
  int tecThumb    = 0;
  int tecIndex    = 0;
  int tecMiddle   = 0;
  int tecRing     = 0;
  int tecPinky    = 0;

  int directions  = 0;

/*  int dirThumb = -1;
  int dirIndex = -1;
  int dirMiddle = -1;
  int dirRing = -1;
  int dirPinky = -1;*/

} pkt_rx;

///////////////////
//     SETUP     //
///////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
//  while (!Serial);
//  Serial.println("ON");

  pwm_driver.begin();
  pwm_driver.set_pwm_freq(PWM_FREQUENCY);

  pwm_driver_et_1.begin();
  pwm_driver_et_1.set_pwm_freq(PWM_FREQUENCY);

  for (int i = 0; i < 16; i++) {
    pwm_driver.set_pwm(i, 0, 0);
    pwm_driver_et_1.set_pwm(i,0,0);
  }

//  test_vibe();
//  test_thermoelectrics();
  test_electrotactiles();
}





//////////////////
//     LOOP     //
//////////////////
void loop() {
  updateSensors();
//  print_flex_sensors();
  if (Serial.available() > 0) {
    numRead = Serial.readBytes((byte *) &pkt_rx, sizeof(ActuatorPacket));

    String range;
    switch(pkt_rx.msgType) {
      case (64):
        sendFingers();
        updateActuators();
        break;
      case (256):
        // max, flat
        delay(2000);
        range = calibrate(0);
        Serial.println("Max " + range);
        break;
       case (128):
        // min, fist
        delay(2000);
        range = calibrate(1);
        Serial.println("Min " + range);
        break;
    }
  }
}





///////////////////////
//     FUNCTIONS     //
///////////////////////
void sendFingers() {
    String data = (String) pkt_tx.flex[0] + "," + pkt_tx.flex[1] + "," + pkt_tx.flex[2] + "," + pkt_tx.flex[3] + "," + pkt_tx.flex[4];
//  String data = (String) pkt_rx.tecThumb + "," + pkt_rx.tecIndex + "," + pkt_rx.tecMiddle + "," + pkt_rx.tecRing + "," + pkt_rx.tecPinky;
//    String data = (String) pkt_rx.directions + ": " + get_et(0) + "," + get_et(1) + "," + get_et(2) + "," + get_et(3) + "," + get_et(4);
//    String data = (String) pkt_rx.directions;
    Serial.println(data);
}



void updateSensors() {

  for( int i = 0; i < NUM_FLEX; i++) {
    pkt_tx.flex[i] = flex[i].read();
    delay(1);
  }

//  pkt_tx.flexThumb = flex[i++].read();
//  pkt_tx.flexIndex = flex[i++].read();
//  pkt_tx.flexMiddle = flex[i++].read();
//  pkt_tx.flexRing = flex[i++].read();
//  pkt_tx.flexPinky = flex[i].read();
}



void updateActuators() {
  // Writes to the vibe motors [0-4095]
//  pwm_driver.set_pwm(thumbVibe,0,pkt_rx.vibeThumb);

//  for (int i = 0; i < NUM_VIBE; i++) {
//    vibrotactile[i].actuate(pkt_rx.vibes[i]);
//    tec[i].actuate(pkt_rx.heats[i]);
//    if (HAS_ET) {
//      // set the electrotaciles to do their thing
//      // based on the direction of movement for each finger
//    }
//  }
  int i = 0;
  vibrotactile[i++].actuate(pkt_rx.vibeThumb);
  vibrotactile[i++].actuate(pkt_rx.vibeIndex);
  vibrotactile[i++].actuate(pkt_rx.vibeMiddle);
  vibrotactile[i++].actuate(pkt_rx.vibeRing);
  vibrotactile[i++].actuate(pkt_rx.vibePinky);
  vibrotactile[i].actuate(pkt_rx.vibePalm);

  i=0;

  // Writes to the thermoelectrics [-4095 - 4095]
  //  Where a (-) value denotes COLD; (+) value denotes HOT
  tec[i++].actuate(pkt_rx.tecThumb);
  tec[i++].actuate(pkt_rx.tecIndex);
  tec[i++].actuate(pkt_rx.tecMiddle);
  tec[i++].actuate(pkt_rx.tecRing);
  tec[i].actuate(pkt_rx.tecPinky);

  if (HAS_ET) {
    for (int i = 0; i < NUM_ET; i++) {
      electrotactile[i].actuate(get_et(i)); 
    } 
  }
}

int get_et(int i) {
//  if (i == 4) {
    return ((pkt_rx.directions & (0x01 << i)) >> i)*ON;
//  } else {
//    return OFF;
//  }
//  bool isOn = (pkt_rx.directionsOn & (0x01 << i)) >> i;
//  bool isRough = (pkt_rx.directionsMagnitude & (0x01 << i)) >> i;
//  if (!isOn) { // is off
//    return OFF;
//  } else if (isRough) { //is on and is rough
//    return 
//  } else { // is on and is not rough => is smooth
//    return ON;
//  }
}

void print_flex_sensors() {
    Serial.print("(flex0,flexThumb,flexIndex,flexMiddle,flexRing,flexPinky,flex6,flex7,flex8,flex9)=(");
  for (byte i = 0 ; i < NUM_FLEX ; i++) {
    Serial.print(flex[i].read_raw());
    if (i < NUM_FLEX - 1)
      Serial.print(",");
  }  
    Serial.println(")");
}



void read_flex_sensors() {  
  for (int i = 0; i < NUM_FLEX; i++) { 
    flex_mapped[i] = flex[i].read();
    #ifdef DEBUG
      Serial.println(flex[i].read_raw());
    #endif
  }
}


//----- TEST FUNCTIONS -----//
void test_vibe() {
  #ifdef DEBUG
    Serial.println("Testing vibrotactiles...");
    Serial.println("\tVibrotactiles... ON (increasing)");
  #endif

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
  delay(10000);

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
  delay(10000);

  #ifdef DEBUG
    Serial.println("\tTesting thermoelectrics...complete");
  #endif
}

void test_electrotactiles() {
  #ifdef DEBUG
    Serial.println("Testing electrotactiles...");
    Serial.println("\tElectrotactiles... ON");
  #endif

  electrotactile[4].actuate(ON);
  delay(1000);
  electrotactile[4].actuate(OFF);
  delay(1000);
//  for(int i = 0; i < NUM_ET_PIXELS; i++) {
//    electrotactile[i].actuate(ON);
//  }
//  delay(10000);
//
//  #ifdef DEBUG
//    Serial.println("\tElectrotactiles... OFF");
//  #endif
//
//  for(int i = 0; i < NUM_ET_PIXELS; i++) {
//    electrotactile[i].actuate(OFF);
//  }
//  delay(10000);
//
  #ifdef DEBUG
    Serial.println("\tTesting electrotactiles...complete");
  #endif
//
//  delay(1000);
}

String calibrate(byte _min) {
  int numReadings = 500;
  long flex_sums[NUM_FLEX] = {0,};
  for (int i = 0; i < numReadings; i++) {
    for (int j = 0; j < NUM_FLEX; j++) {
      flex_sums[j] += flex[j].read_raw();
    }
  }

  String range = "";
  for (int i = 0; i < NUM_FLEX; i++) {
    if (_min) {
      flex[i].set_min(flex_sums[i]/numReadings);
      range += flex[i].get_min();
    } else {
      flex[i].set_max(flex_sums[i]/numReadings);
      range += flex[i].get_max();
    }

    range += ',';
  }

  return range;
}
