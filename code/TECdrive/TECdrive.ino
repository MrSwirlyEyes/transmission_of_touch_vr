#define strength_pin 3
#define direction_pin 5

int i = 0;
int d = 255;
//int strength = 0;
  
void setup() {
 
  Serial.begin(9600);
}

void loop() {
  
  Serial.print("Direction: ");
  Serial.println(d);
  analogWrite(direction_pin, d);

  for(i = 0;i <= 255; i++){
    analogWrite(strength_pin, i);
    Serial.println(i);
    delay(30);
  }
  for(i=255; i>=0; i--){
    analogWrite(strength_pin, i);
    Serial.println(i);
    delay(30);
  }
  if(d==255){ d= 0;}
  else if(d==0){d=255;}
}
