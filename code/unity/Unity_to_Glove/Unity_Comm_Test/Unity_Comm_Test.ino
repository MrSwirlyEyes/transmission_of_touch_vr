#include "Adafruit_PWMServoDriver.h" //download library from adafruit website
#include "CD74HC4067.h" //include this library from github



#define BAUDRATE 9600


// Type of Touch Glove
// Cols - how do I make an enumeration?
// 0 for no electrotactiles, 1 for has electrotactile
#define HAS_ET 0

struct SensorPacket {
  int flexThumb = 0;
  int flexIndex = 0;
  int flexMiddle = 0;
  int flexRing = 0;
  int flexPinky = 0;
} outpkt;

struct ActuatorPacket {
  int vibeThumb = 0;
  int vibeIndex = 0;
  int vibeMiddle = 0;
  int vibeRing = 0;
  int vibePinky = 0;

  int tecThumb = 0;
  int tecIndex = 0;
  int tecMiddle = 0;
  int tecRing = 0;
  int tecPinky = 0;

  /*
  int dirThumb = -1;
  int dirIndex = -1;
  int dirMiddle = -1;
  int dirRing = -1;
  int dirPinky = -1;
  int dirWrist = -1;
  */
  
} inpkt;

byte numRead;



////////////////////////
//     PWM DRIVER     //
////////////////////////
#define PWM_FREQUENCY 60
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();



///////////////////////////
//     VIBROTACTILES     //
///////////////////////////

#define NUM_VIBE 5

#define VIBMIN 0
#define VIBMAX 4095

#define thumbVibe 4
#define indexVibe 3
#define middleVibe 2
#define ringVibe 1
#define pinkyVibe 0


/////////////////////////////
//     THERMOELECTRICS     //
/////////////////////////////

//#define PHASE_COLD 0
//#define PHASE_HOT 1

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

int flex[NUM_FLEX] = {0,};

byte flex_pin[NUM_FLEX] = {
                            FLEX_PINKY,
                            FLEX_RING,
                            FLEX_MIDDLE,
                            FLEX_INDEX,
                            FLEX_THUMB,
                          };

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
                            FLEX_MIN_THUMB,
                            FLEX_MIN_INDEX,
                            FLEX_MIN_MIDDLE,
                            FLEX_MIN_RING,
                            FLEX_MIN_PINKY,
                          };

int flex_max[NUM_FLEX] = {
                            FLEX_MAX_THUMB,
                            FLEX_MAX_INDEX,
                            FLEX_MAX_MIDDLE,
                            FLEX_MAX_RING,
                            FLEX_MAX_PINKY,
                          };



///////////////////
//     SETUP     //
///////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
//  while (!Serial);
//  Serial.println("ON");

  pwm.begin();
  pwm.setPWMFreq(PWM_FREQUENCY);

  for (int i = 0; i < 16; i++) {
    pwm.setPin(i, 0); 
  }
}



//////////////////
//     LOOP     //
//////////////////
void loop() {
  updateSensors();
//  // put your main code here, to run repeatedly:
  if (Serial.available() > sizeof(inpkt)) {
    numRead = Serial.readBytes((byte *) &inpkt, sizeof(inpkt));
    sendFingers();
    updateActuators();
  }  

//read_flex_sensors();
////
//  print_flex_sensors();
}



///////////////////////
//     FUNCTIONS     //
///////////////////////
void sendFingers() {
    String data = (String) outpkt.flexThumb + "," + outpkt.flexIndex + "," + outpkt.flexMiddle + "," + outpkt.flexRing + "," + outpkt.flexPinky;
//    String data = (String) numRead;
    Serial.println(data);
}



void updateSensors() {
  int i = 0;
  // Op amp (row "2") flex channels (vert row)
  outpkt.flexThumb = constrain(demux.read_channel(flex_pin[i]),flex_min[i],flex_max[i]);
  i++;
  outpkt.flexIndex = constrain(demux.read_channel(flex_pin[i]),flex_min[i],flex_max[i]);
  i++;
  outpkt.flexMiddle = constrain(demux.read_channel(flex_pin[i]),flex_min[i],flex_max[i]);
  i++;
  outpkt.flexRing = constrain(demux.read_channel(flex_pin[i]),flex_min[i],flex_max[i]);
  i++;
  outpkt.flexPinky = constrain(demux.read_channel(flex_pin[i]),flex_min[i],flex_max[i]);

  // Non-op amp (row "1") flex channels (horiz row)
//  outpkt.s1 = demux.read_channel(7);
//  outpkt.s2 = demux.read_channel(6);
//  outpkt.s3 = demux.read_channel(5);
//  outpkt.s4 = demux.read_channel(4);
//  outpkt.s5 = demux.read_channel(3);
}



void updateActuators() {
  // Writes to the vibe motors [0-4095]
  pwm.setPWM(thumbVibe,0,inpkt.vibeThumb);
  pwm.setPWM(indexVibe,0,inpkt.vibeIndex);
  pwm.setPWM(middleVibe,0,inpkt.vibeMiddle);
  pwm.setPWM(ringVibe,0,inpkt.vibeRing);
  pwm.setPWM(pinkyVibe,0,inpkt.vibePinky);

  // Writes to the thermoelectrics [-4095 - 4095]
  //  Where a (-) value denotes COLD; (+) value denotes HOT
  if(inpkt.tecThumb < 0) {
    pwm.setPWM(TEC_THUMB_HOT,0,0);
    pwm.setPWM(TEC_THUMB_COLD,0,constrain(map(abs(inpkt.tecThumb),TECMIN,TECMAX,TECMIN,TECMAX_COLD),TECMIN,TECMAX_COLD));
  } else if(inpkt.tecThumb > 0) {
    pwm.setPWM(TEC_THUMB_COLD,0,0);
    pwm.setPWM(TEC_THUMB_HOT,0,constrain(map(abs(inpkt.tecThumb),TECMIN,TECMAX,TECMIN,TECMAX_HOT),TECMIN,TECMAX_HOT));
  } else {
    pwm.setPWM(TEC_THUMB_HOT,0,0);
    pwm.setPWM(TEC_THUMB_COLD,0,0);
  }
  
  if(inpkt.tecIndex < 0) {
    pwm.setPWM(TEC_INDEX_HOT,0,0);
    pwm.setPWM(TEC_INDEX_COLD,0,constrain(map(abs(inpkt.tecIndex),TECMIN,TECMAX,TECMIN,TECMAX_COLD),TECMIN,TECMAX_COLD));
  } else if(inpkt.tecIndex > 0) {
    pwm.setPWM(TEC_INDEX_COLD,0,0);
    pwm.setPWM(TEC_INDEX_HOT,0,constrain(map(abs(inpkt.tecIndex),TECMIN,TECMAX,TECMIN,TECMAX_HOT),TECMIN,TECMAX_HOT));
  } else {
    pwm.setPWM(TEC_INDEX_HOT,0,0);
    pwm.setPWM(TEC_INDEX_COLD,0,0);
  }
  
  if(inpkt.tecMiddle < 0) {
    pwm.setPWM(TEC_MIDDLE_HOT,0,0);
    pwm.setPWM(TEC_MIDDLE_COLD,0,constrain(map(abs(inpkt.tecMiddle),TECMIN,TECMAX,TECMIN,TECMAX_COLD),TECMIN,TECMAX_COLD));
  } else if(inpkt.tecMiddle > 0) {
    pwm.setPWM(TEC_MIDDLE_COLD,0,0);
    pwm.setPWM(TEC_MIDDLE_HOT,0,constrain(map(abs(inpkt.tecMiddle),TECMIN,TECMAX,TECMIN,TECMAX_HOT),TECMIN,TECMAX_HOT));
  } else {
    pwm.setPWM(TEC_MIDDLE_HOT,0,0);
    pwm.setPWM(TEC_MIDDLE_COLD,0,0);
  }
  
  if(inpkt.tecRing < 0) {
    pwm.setPWM(TEC_RING_HOT,0,0);
    pwm.setPWM(TEC_RING_COLD,0,constrain(map(abs(inpkt.tecRing),TECMIN,TECMAX,TECMIN,TECMAX_COLD),TECMIN,TECMAX_COLD));
  } else if(inpkt.tecRing > 0) {
    pwm.setPWM(TEC_RING_COLD,0,0);
    pwm.setPWM(TEC_RING_HOT,0,constrain(map(abs(inpkt.tecRing),TECMIN,TECMAX,TECMIN,TECMAX_HOT),TECMIN,TECMAX_HOT));
  } else {
    pwm.setPWM(TEC_RING_HOT,0,0);
    pwm.setPWM(TEC_RING_COLD,0,0);
  }
  
  if(inpkt.tecPinky < 0) {
    pwm.setPWM(TEC_PINKY_HOT,0,0);
    pwm.setPWM(TEC_PINKY_COLD,0,constrain(map(abs(inpkt.tecPinky),TECMIN,TECMAX,TECMIN,TECMAX_COLD),TECMIN,TECMAX_COLD));
  } else if(inpkt.tecPinky > 0) {
    pwm.setPWM(TEC_PINKY_COLD,0,0);
    pwm.setPWM(TEC_PINKY_HOT,0,constrain(map(abs(inpkt.tecPinky),TECMIN,TECMAX,TECMIN,TECMAX_HOT),TECMIN,TECMAX_HOT));
  } else {
    pwm.setPWM(TEC_PINKY_HOT,0,0);
    pwm.setPWM(TEC_PINKY_COLD,0,0);
  }

  if (HAS_ET) {
    // set the electrotaciles to do their thing
    // based on the direction of movement for each finger
  }
}



void print_flex_sensors() {
    Serial.print("(flex0,flexThumb,flexIndex,flexMiddle,flexRing,flexPinky,flex6,flex7,flex8,flex9)=(");
  for (byte i = 0 ; i < NUM_FLEX ; i++) {
    Serial.print(flex[i]);
    if (i < NUM_FLEX - 1)
      Serial.print(",");
  }  
    Serial.println(")");
}



//void read_flex_sensors() {  
//  for (int i = 0; i < NUM_FLEX; i++)
//    flex[i] = demux.read_channel(i);
//}
void read_flex_sensors() {  
  for (int i = 0; i < NUM_FLEX; i++) { 
    flex[i] = constrain(demux.read_channel(flex_pin[i]),flex_min[i],flex_max[i]);
  }
}
