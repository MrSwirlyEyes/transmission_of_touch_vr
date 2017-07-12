/*  
 *    TOUCH GLOVE
 * 
 */


#define BTSerial Serial3 // Bluetooth

#define led 13 // LED onboard Teensy

bool debug=true;

#define BT_delay 50
#define sensor_delay 10
#define actuator_delay 10





/*****     STRAIN SENSORS     *****/

#define num_strain_sensors 5 // Total # of stain sensors

// Strain sensors on each of the glove fingers
//#define pinky_strain A1
//#define ring_strain A2
//#define middle_strain A3
//#define index_strain A4
//#define thumb_strain A5

// PCB
#define pinky_strain A0
#define ring_strain A1
#define middle_strain A2
#define index_strain A3
#define thumb_strain A4

// Value array for strain sensors
int strain_val_arr[num_strain_sensors]={0,};

// Strain sensor pin array
byte strain_pin_arr[num_strain_sensors]={
                                            pinky_strain,
                                            ring_strain,
                                            middle_strain,
                                            index_strain,
                                            thumb_strain
                                         };            

// Max value to map to for strain sensors (relative to Robotic Arm servos)
#define pinky_max_strain 75
#define ring_max_strain 100
#define middle_max_strain 115
#define index_max_strain 115
#define thumb_max_strain 85

byte strain_max_val[num_strain_sensors]={
                                      pinky_max_strain,
                                      ring_max_strain,
                                      middle_max_strain,
                                      index_max_strain,
                                      thumb_max_strain
                                    };





/*****     TEMPERATURE (Thermal Electric Coupler) ACTUATORS     *****/

#define num_tec_acts 5 // Total # of temperature actuators

// Temperature actuators on each of the glove finger tips
//#define pinky_tec 10
//#define ring_tec 11
//#define middle_tec 12
//#define index_tec 13
//#define thumb_tec 14

//#define pinky_tec_A1 10 // PWM 
//#define pinky_tec_A2 11 // NOT PWM (always 0)
//#define ring_tec_B1 23 // PWM
//#define ring_tec_B2 12 // NOT PWM (always 0)
//#define middle_tec_A1 22 // PWM
//#define middle_tec_A2 2 // NOT PWM (always 0)
//#define index_tec_B1 21 // PWM
//#define index_tec_B2 1 // NOT PWM (always 0)
//#define thumb_tec_A1 20 // PWM
//#define thumb_tec_A2 0 // NOT PWM (always 0)

// PCB
#define pinky_tec_A1 10 // PWM
#define pinky_tec_A2 11 // NO PWM

#define ring_tec_B1 19 // NO PWM
#define ring_tec_B2 20 // PWM

#define middle_tec_A1 21 // PWM
#define middle_tec_A2 22 // PWM

#define index_tec_B1 23 // PWM
#define index_tec_B2 24 // NO PWM

#define thumb_tec_A1 27 // NO PWM
#define thumb_tec_A2 36 // NO PWM

// Value array for temperature actuators
int tec_val_arr[num_tec_acts]={0,};

// Temperature actuator pin array
byte tec_pin_arr[num_tec_acts]={
                                  pinky_tec_A1,
                                  ring_tec_B1,
                                  middle_tec_A1,
                                  index_tec_B1,
                                  thumb_tec_A1
                               };
                               
// Temperature actuator pin array
byte tec_pin_arr2[num_tec_acts]={
                                  pinky_tec_A2,
                                  ring_tec_B2,
                                  middle_tec_A2,
                                  index_tec_B2,
                                  thumb_tec_A2
                               };                          

// Max value to map to for tec actuators (relative to Robotic Arm temp sensors)
#define pinky_max_tec 255
#define ring_max_tec 255
#define middle_max_tec 255
#define index_max_tec 255
#define thumb_max_tec 255

byte tec_max_val[num_strain_sensors]={
                                      pinky_max_tec,
                                      ring_max_tec,
                                      middle_max_tec,
                                      index_max_tec,
                                      thumb_max_tec
                                    };





/*****     PRESSURE (VIBRO TACTILE) ACTUATORS     *****/

#define num_vibe_acts 5 // Total # of pressure actuators

// Pressure actuators on each of the glove finger tips
//#define pinky_vibe 3
//#define ring_vibe 4
//#define middle_vibe 5
//#define index_vibe 6
//#define thumb_vibe 9

// PCB
#define pinky_vibe 6
#define ring_vibe 5
#define middle_vibe 4
#define index_vibe 3
#define thumb_vibe 25

// Value array for pressure actuators
int vibe_val_arr[num_vibe_acts]={0,};

// Value array for pressure actuators
byte vibe_pin_arr[num_vibe_acts]={
                                    pinky_vibe,
                                    ring_vibe,
                                    middle_vibe,
                                    index_vibe,
                                    thumb_vibe
                                 };

// Max value to map to for strain sensors (relative to Robotic Arm servos)
#define pinky_max_vibe 255
#define ring_max_vibe 255
#define middle_max_vibe 255
#define index_max_vibe 255
#define thumb_max_vibe 255

byte vibe_max_val[num_vibe_acts]={
                                      pinky_max_vibe,
                                      ring_max_vibe,
                                      middle_max_vibe,
                                      index_max_vibe,
                                      thumb_max_vibe
                                    };                               





/*****     BLUETOOTH TX/RX VARIABLES     *****/                               

const byte numChars=40; // 3 (# of digits) * 10 (num readings: 5 temp, 5 pressure) + 9 (semicolons) + 1 (end marker)
char receivedChars[numChars]; // Char array of our received data
bool newData = false;  // 





/*****     SETUP     *****/

void setup() {

    open_serial_COM();

//    vibe_start_seq();
    tec_start_seq();

    pinMode(led, OUTPUT); // Configure LED (pin) as OUTPUT

    for(unsigned int i=0;i<num_tec_acts;i++) {
      digitalWrite(tec_pin_arr2[i],LOW);
    }

    digitalWrite(led, HIGH);


    //TEC 
    pinMode(pinky_tec_A2,OUTPUT);
    digitalWrite(pinky_tec_A2,LOW);
}


/*****     LOOP     *****/

void loop() {    
    digitalWrite(led, HIGH);
    transmitBT();    
    delay(BT_delay);
    digitalWrite(led, LOW);
    receiveBT();
    delay(BT_delay);
}





/*****     FUNCTION DEFINITIONS     *****/



/***   BT TRANSMIT HELPER FUNCTIONS   ***/

void transmitBT() {
    if(debug) {Serial.println("Touch Glove - \"Strain\"");}

    print_start_marker();    
    
    for(int i=0;i<num_strain_sensors;i++) {

      read_strain_sensor(i);      

      send_data(i);      

      print_delimiter(i);      
    }  

    print_end_marker();
    
    BTSerial.flush();
}



void print_start_marker() {
    if(debug) {Serial.print("<");}
    BTSerial.print("<");
}



void print_delimiter(int i) {
    if(i<num_strain_sensors-1) {
      if(debug) {Serial.print(";");}
      BTSerial.print(";");
    }
}



void print_end_marker() {
    if(debug) {Serial.println(">");}
    BTSerial.print(">");
    
    if(debug) {Serial.println();}
}



void send_data(int i) {
  if(strain_val_arr[i] <= 5) {
      if(debug) {Serial.print(0);}
      BTSerial.print(0);        
  } else {      
      BTSerial.print(strain_val_arr[i]);
      if(debug) {Serial.print(strain_val_arr[i]);}
  }
  delay(sensor_delay);
}

void read_strain_sensor(int i) {
    int raw=analogRead(strain_pin_arr[i]);
    Serial.print("raw");
    Serial.print(i);
    Serial.print("= ");
    Serial.println(raw);
    strain_val_arr[i]=map(raw,0,1023,0,strain_max_val[i]);
    delay(sensor_delay);
}





/***   BT RECEIVE HELPER FUNCTIONS   ***/

void receiveBT() {
    recvWithStartEndMarkers();  // Retrieve data over BT
    showNewData();  // Manipulate the retrieved data    
}



void recvWithStartEndMarkers() {
    static bool recvInProgress=false;
    static byte ndx=0;
    char startMarker='<';
    char endMarker='>';
    char rc;
 
    while (BTSerial.available() > 0 && newData == false) {
        rc=BTSerial.read(); // Read from the BT buffer

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            } else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        } else if (rc == startMarker) {
            recvInProgress = true;     
        }   
    }
}



void showNewData() {
    if (newData == true) {
        if(debug) {Serial.println();}
        if(debug) {Serial.print("RECEIVED: ");}
        String sensor_data=receivedChars;

        parse_sensor_readings(sensor_data);
        write_to_actuators();
        newData = false;
    }
}



void parse_sensor_readings(String sensor_data) {
  if(debug) {Serial.println(sensor_data);}
  int s_idx=0;
  String tmp="";
  char c;
  int i=0;
  
  while(c=sensor_data[s_idx]) {    

     if(c == ';') {
        //Serial.print(tmp);
        //Serial.print(';');
        if(i<num_vibe_acts)
            vibe_val_arr[i]=tmp.toInt();
        else if (i >= num_vibe_acts && i < num_vibe_acts + num_tec_acts)
            tec_val_arr[i-num_vibe_acts]=tmp.toInt();        
        tmp="";
        i++;
     } else
        tmp+=c;   
        
    s_idx++;
  }
  
  if(i<num_vibe_acts)
      vibe_val_arr[i]=tmp.toInt();
  else if (i >= num_vibe_acts && i < num_vibe_acts + num_tec_acts)
      tec_pin_arr[i-num_vibe_acts]=tmp.toInt();        
  tmp="";
//  Serial.print("last=");
//  Serial.print(tmp);
}



void write_to_actuators() {    
    if(debug) {Serial.print("TEC: ");}
    for(int i=0;i<num_tec_acts;i++) {
        analogWrite(tec_pin_arr[i],tec_val_arr[i]);    
//        analogWrite(tec_pin_arr[i],tec_val_arr[0]);    
        if(debug) {Serial.print(tec_val_arr[i]);}
        if(debug) {Serial.print(",");}
        delay(actuator_delay); 
    }
    if(debug) {Serial.println();}
    if(debug) {Serial.print("VIBE: ");}
    for(int i=0;i<num_vibe_acts;i++) {
        analogWrite(vibe_pin_arr[i],vibe_val_arr[i]);    
//        analogWrite(vibe_pin_arr[i],vibe_val_arr[0]);    
        if(debug) {Serial.print(vibe_val_arr[i]);}
        if(debug) {Serial.print(",");}
        delay(actuator_delay); 
    }
    if(debug) {Serial.println();}      
}



/*****     MISC HELPER FUNCTIONS     *****/

void vibe_start_seq() {
    for(unsigned int j=0;j<num_vibe_acts;j++) {
        for(int i=0;i<vibe_max_val[j];i++) {
            analogWrite(vibe_pin_arr[j],i);
            delay(5);
        }
        for(int i=vibe_max_val[j];i>=0;i--) {
            analogWrite(vibe_pin_arr[j],i);
            delay(5);
        }      
    }
}

void tec_start_seq() {
    for(unsigned int j=0;j<num_tec_acts;j++) {
        for(int i=0;i<tec_max_val[j];i++) {
            analogWrite(tec_pin_arr[j],i);
            delay(5);
        }
        for(int i=tec_max_val[j];i>=0;i--) {
            analogWrite(tec_pin_arr[j],i);
            delay(5);
        }      
    }
}



void open_serial_COM() {    
    Serial.begin(9600); // Start Serial Monitor (Console output)
    delay(500);
    BTSerial.begin(9600); // Start BT COMM port
    delay(500);
}
