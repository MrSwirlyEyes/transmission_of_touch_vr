//Mux control pins
// ARDUINO PINOUT
//#define s0 8
//#define s1 9
//#define s2 10
//#define s3 11

// RED BOARD PINOUT
#define s0 2
#define s1 6
#define s2 7
#define s3 10

byte controlPin[]={s0,s1,s2,s3};

bool muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

//Mux in "SIG" pin
#define SIGpin 0


void setup(){
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 
//  pinMode(SIGpin, INPUT);

//  for(int i = 0; i < 4; i ++){
//    digitalWrite(controlPin[i], LOW);
//  }

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  

  Serial.begin(9600);

  while(1)
    test3();

}


void test3() {
  for(int j = 0; j < 10 ; j++) {
    for(int i = 0; i < 4; i++){
      digitalWrite(controlPin[i], muxChannel[j][i]);
    }
    //read the value at the SIG pin  
    Serial.println(analogRead(SIGpin));
  }

}


void test2() {
  Serial.println(analogRead(SIGpin));
}

//void test() {
//  for(int i=0;i<4;i++) {
//      digitalWrite(arr[i],HIGH);
//  }
//  delay(500);
//   for(int i=0;i<4;i++) {
//      digitalWrite(arr[i],LOW);
//  }
//  delay(500);
//    
//}


void loop(){

  //Loop through and read all 16 values
  //Reports back Value at channel 6 is: 346
  for(int i = 0; i < 10; i++){
    Serial.print("Value at channel ");
    Serial.print(i);
    Serial.print("is : ");
    Serial.println(readMux(i));
    
//    Serial.print("Mapped Value at channel ");
//    Serial.print(i);
//    Serial.print("is : ");
//    Serial.println(map(readMux(i),0,1000,0,255));
    
    delay(10);
  }
  Serial.print("---------------------------------- \n");
//delay(1000);
}

int readMux(int channel){
//  int controlPin[] = {s0, s1, s2, s3};

//  int muxChannel[16][4]={
//    {0,0,0,0}, //channel 0
//    {1,0,0,0}, //channel 1
//    {0,1,0,0}, //channel 2
//    {1,1,0,0}, //channel 3
//    {0,0,1,0}, //channel 4
//    {1,0,1,0}, //channel 5
//    {0,1,1,0}, //channel 6
//    {1,1,1,0}, //channel 7
//    {0,0,0,1}, //channel 8
//    {1,0,0,1}, //channel 9
//    {0,1,0,1}, //channel 10
//    {1,1,0,1}, //channel 11
//    {0,0,1,1}, //channel 12
//    {1,0,1,1}, //channel 13
//    {0,1,1,1}, //channel 14
//    {1,1,1,1}  //channel 15
//  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIGpin);
  Serial.println(val);

  //return the value
  return val;
}


