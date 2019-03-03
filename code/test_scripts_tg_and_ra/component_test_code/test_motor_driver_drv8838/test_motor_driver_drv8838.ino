#define TEC1 9
#define PHASE 2

#define MAX 255

#define DELAY 25

void setup() {
  digitalWrite(PHASE,LOW);
  // put your setup code here, to run once:
  analogWrite(TEC1,LOW);
  delay(10);  
  Serial.begin(9600);
  Serial.println("Waiting...");
  delay(5000);
  Serial.println("START!");
}

void loop() {
  
  // put your main code here, to run repeatedly:
  for(int i=0;i<MAX;i++) {
    analogWrite(TEC1,i);  
    delay(10);    
    Serial.println(i);
    delay(DELAY);
  }

  digitalWrite(PHASE,HIGH);
  for(int i=MAX;i>=0;i--) {
    analogWrite(TEC1,i);
    
    Serial.println(i);
    delay(DELAY);
  }
  digitalWrite(PHASE,LOW);

  analogWrite(TEC1,LOW);  
  delay(500);
}
