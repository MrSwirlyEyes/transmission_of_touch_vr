/*
 * Communication between Microcontroller (e.g. Arduino UNO) and Unity
 *  over Serial UART interface.
 * 
 */
#define BAUDRATE 9600


struct PacketOutput {
  int data1 = 0;
  int data2 = 0;
  int data3 = 0;
  int data4 = 0;
  int data5 = 0;
} pkt_out;


struct PacketInput {
  int data1 = 0;
  int data2 = 0;
  int data3 = 0;
  int data4 = 0;
  int data5 = 0;
} pkt_in;



void setup() {
  Serial.begin(BAUDRATE);
}



void loop() {
  // Read from Sensors
  readSensors();
  
  if (Serial.available() >= sizeof(PacketInput)) {
      // Read from Unity into pkt_in
      Serial.readBytes((byte *) &pkt_in, sizeof(PacketInput));

      // Write from MCU to Unity
      transmitData();

      // Write to Actuators
      updateActuators();
  }
}



void transmitData() {
  // Build a string with a delimiter (e.g. ",") and handle parsing on Unity end.
  String data = (String) pkt_out.data1 + "," + pkt_out.data2 + "," + pkt_out.data3 + "," + pkt_out.data4 + "," + pkt_out.data5;
  Serial.println(data);
}



// Read/Update from MCU Sensors
void readSensors() {
  int data_dumb = 1000;
  pkt_out.data1 = data_dumb++;
  pkt_out.data2 = data_dumb++;
  pkt_out.data3 = data_dumb++;
  pkt_out.data4 = data_dumb++;
  pkt_out.data5 = data_dumb++;
}



void updateActuators() {
  // Write/Update Actuators here

  // No examples to put here since I am not including any hardware in the setup.
}
