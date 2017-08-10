#include <radio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //library needs to be downloaded

#define CHANNEL 11
#define SERVOMIN  150 //This is the 'minimum' pulse length count (out of 4096).
#define SERVOMAX  600 //This is the 'maximum' pulse length count (out of 4096).

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Radio radio(CHANNEL);  // Initialize ATmega128RFA1 radio on channel CHANNEL (can be 11-26)

int servoNum, angle, i;

struct Packet {

  int start_check = 0;

  int sensordat[16];
  
//  int test1= 0;
//
//  int test2= 0;
//
//  int test3= 0;

  int end_check = 0;
  
};

Packet pkt;




void setup()
{
  Serial.begin(9600);

   //Set up PWM.
  pwm.begin();
  pwm.setPWMFreq(60);

  
}

void loop()
{
  if(radio.rfAvailable()) {           // If data receievd on radio...
    radio.rfRead((uint8_t *) & pkt,sizeof(Packet)); // ... send it out serial.

    if(valid_pkt_check(pkt)) {
      //Loop through and read all 16 values
      //Reports back Value at channel 6 is: 346
      for(int j = 0; j < 5 ; j++) {
        Serial.print("Raw Flex vall");
        Serial.print(j);
        Serial.print(": ");
        Serial.println(pkt.sensordat[j]);
        angle = map(pkt.sensordat[j],0,1000,0,180);
        turnTo(j, angle);
      }
      for(int j = 5; j<10; j++){
        Serial.print("Raw Pressure vall");
        Serial.print(j-5);
        Serial.print(": ");
        Serial.println(pkt.sensordat[j]);
        angle = map(pkt.sensordat[j],800,200,0,180);
        turnTo(j, angle);
      }
    
//      Serial.print("Test1: ");
//      Serial.println(pkt.test1);
//      Serial.print("Test2: ");
//      Serial.println(pkt.test2);
//      Serial.print("Test3: ");
//      Serial.println(pkt.test3);
    }
    else { Serial.print("Error");
    }
  }
}

bool valid_pkt_check(Packet & pkt) {
  if(pkt.start_check != 0xFEEAED){
    return false;
  }
  else if(pkt.end_check != 0xDAAABD){
    return false;
  }
  return true;
}

void turnTo(int servoNum, int angle)
{
  angle = map(angle, 0, 180*4, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoNum, 0, angle);
}


  
