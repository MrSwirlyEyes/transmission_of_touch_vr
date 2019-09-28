#define ON 4095
#define OFF 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int i = 4;
  int directions = 10;
  
  if (i == 4) {
    Serial.println(0x01 << i, BIN); // 16
    Serial.println(directions, BIN);
    Serial.println(directions & (0x01 << i), BIN); // 16
    Serial.println(((directions & (0x01 << i)) >> i)); // 1
    Serial.println ((directions & (0x01 << i)) >> i)*ON; // 4095
  } else {
    return OFF;
  }
  
//  int encoded = 1 + 2 + 4 + 8 + 16;
//  Serial.println(0x01); // 1
//  Serial.println(0x01 << 4); // 16
//  Serial.println(encoded & (0x01 << 4)); // 16
//  
//  int val = ((encoded & (0x01 << 4)) >> 4);
//  Serial.println(val); // 1
}

void loop() {
  // put your main code here, to run repeatedly:

}
