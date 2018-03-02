#include <SoftwareSerial.h>
#include "SerialCommand.h"

#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4

#define L1 5
#define L2 6
#define L3 9
#define L4 10
#define L5 11

SerialCommand sCmd;

struct SensorPacket {
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  int s4 = 0;
  int s5 = 0;
} pkt;

//byte [] 

void pingHandler () {
//  Serial.write((byte *) & pkt, sizeof(SensorPacket));
  String data = "";
  data = (String) pkt.s1 + "," + pkt.s2 + "," + pkt.s3 + "," + pkt.s4 + "," + pkt.s5;
  Serial.println(data);
}

void echoHandler() {
  char * arg;
  int pwm1, pwm2, pwm3, pwm4, pwm5;
  
  arg = sCmd.next();
  if (arg != NULL) {
    if (sscanf(arg, "%d,%d,%d,%d,%d", &pwm1, &pwm2, &pwm3, &pwm4, &pwm5) == 5) {
      analogWrite(L1, pwm1);
      analogWrite(L2, pwm2);
      analogWrite(L3, pwm3);
      analogWrite(L4, pwm4);
      analogWrite(L5, pwm5);
    } else {
      analogWrite(L1, 0);
      analogWrite(L2, 255);
      analogWrite(L3, 0);
      analogWrite(L4, 255);
      analogWrite(L5, 0);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);

  sCmd.addCommand("PING", pingHandler);
  sCmd.addCommand("ECHO", echoHandler);
//  sCmd.setDefaultHandler(errorHandler);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    sCmd.readSerial();
  }

  delay(5);
  pkt.s1 = analogRead(S1);
  delay(5);
  pkt.s1 = analogRead(S1);
  delay(5);
  pkt.s2 = analogRead(S2);
  delay(5);
  pkt.s2 = analogRead(S2);
  delay(5);
  pkt.s3 = analogRead(S3);
  delay(5);
  pkt.s3 = analogRead(S3);
  delay(5);
  pkt.s4 = analogRead(S4);
  delay(5);
  pkt.s4 = analogRead(S4);
  delay(5);
  pkt.s5 = analogRead(S5);
  delay(5);
  pkt.s5 = analogRead(S5);

}
