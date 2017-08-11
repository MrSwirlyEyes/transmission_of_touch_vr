/*  
 *    ROBOTIC ARM
 *    
 *    Written by:  Colin Keef
 * 
 */
 
#include <Servo.h>  // Servo library

#define BTSerial Serial3 // Bluetooth

#define led 13 // LED onboard Teensy

bool debug=false;





/*****     SERVO ACTUATORS     *****/

#define num_servo_acts 5 // Total # of servo actuators

// Servo actuators for each of the robotic arm fingers
#define pinky_servo 9
#define ring_servo 6
#define middle_servo 5
#define index_servo 4
#define thumb_servo 3

// Array of servo actuators
Servo servo_act_arr[num_servo_acts];

// Value array for strain sensors
int servo_val_arr[num_servo_acts]={0,};
int last_servo_val_arr[num_servo_acts]={0,};
int delta_servo_val_arr[num_servo_acts]={0,};

// Servo actuator pin array
byte servo_pin_arr[num_servo_acts]={
                                      pinky_servo,
                                      ring_servo,
                                      middle_servo,
                                      index_servo,
                                      thumb_servo
                                   };    

// Max value to map to write to strain sensors (relative to Robotic Arm servos)
#define pinky_max 75
#define ring_max 100
#define middle_max 115
#define index_max 115
#define thumb_max 85

byte servo_max_val[num_servo_acts]={
                                      pinky_max,
                                      ring_max,
                                      middle_max,
                                      index_max,
                                      thumb_max
                                    };





/*****     TEMPERATURE (Thermal Electric Coupler) ACTUATORS     *****/

#define num_temp_sensors 5 // Total # of temperature sensors

// Temperature sensors on each of the robotic arm finger tips
#define pinky_temp A5
#define ring_temp A6
#define middle_temp A7
#define index_temp A8
#define thumb_temp A9

// Value array for temperature sensors
byte temp_val_arr[num_temp_sensors]={0,0,0,0,0};

// Temperature sensor pin array
byte temp_pin_arr[num_temp_sensors]={
                                  pinky_temp,
                                  ring_temp,
                                  middle_temp,
                                  index_temp,
                                  thumb_temp
                               };





/*****     PRESSURE SENSORS     *****/

#define num_press_sensors 5 // Total # of pressure sensors

// Pressure sensors on each of the robotic arm finger tips
#define pinky_press A0
#define ring_press A1
#define middle_press A2
#define index_press A3
#define thumb_press A4

// Value array for pressure sensors
byte press_val_arr[num_press_sensors]={0,0,0,0,0};

// Value array for pressure actuators
byte press_pin_arr[num_press_sensors]={
                                    pinky_press,
                                    ring_press,
                                    middle_press,
                                    index_press,
                                    thumb_press
                                 };





/*****     BLUETOOTH TX/RX VARIABLES     *****/   
                            
const byte numChars=20; // 3 (# of digits) * 5 (num readings: 5 strain) + 4 (semicolons) + 1 (end marker)
char receivedChars[numChars]; // Char array of our received data
bool newData = false;  // 





/*****     SETUP     *****/

void setup() {
    open_serial_COM();    

    initialize_servos();   

    test_robo_arm();

    pinMode(led, OUTPUT); // Configure LED (pin) as OUTPUT

    analogReference(EXTERNAL);
    digitalWrite(led, HIGH);
}





/*****     LOOP     *****/

void loop() {    
    digitalWrite(led, HIGH);
    transmitBT();    
    delay(50);
    digitalWrite(led, LOW);
    
    receiveBT();
    delay(50);
}





/*****     FUNCTION DEFINITIONS     *****/





/***   BT TRANSMIT HELPER FUNCTIONS   ***/

void transmitBT() {
    if(debug) {Serial.println("Robotic Arm:  Temperature");}

    print_start_marker();

    send_press_data();
    
    if(debug) Serial.print(';');
    BTSerial.print(';');
    
    send_temp_data();
    
    print_end_marker();
  
    BTSerial.flush();
}



void send_press_data() {
     for(int i=0;i<num_press_sensors;i++) {        
        read_press_sensor(i);      
        
        send_data(i,"press");      
  
        print_delimiter(i,"press");
        
        delay(10);
    } 
}



void send_temp_data() {
    for(int i=0;i<num_temp_sensors;i++) {
        read_temp_sensor(i);      
        
        send_data(i,"temp");      
    
        print_delimiter(i,"temp");
        
        delay(10);
    }  
}



void print_start_marker() {
    if(debug) {Serial.print("<");}
    BTSerial.print("<");
}



void print_delimiter(int i,String sensor) {
    if(sensor.compareTo("temp") == 0) {
        if(i < num_temp_sensors-1) {
            if(debug) {Serial.print(";");}
            BTSerial.print(";");
        }
    } else if(sensor.compareTo("press") == 0) {
        if(i < num_press_sensors-1) {
            if(debug) {Serial.print(";");}
            BTSerial.print(";");
        }
    } else {
        if(debug) Serial.print("ERROR");  
         BTSerial.print("ERROR");
    }
}



void print_end_marker() {
    if(debug) {Serial.println(">");}
    BTSerial.print(">");
    if(debug) {Serial.println();}
}



void send_data(int i,String sensor) {
    if(sensor.compareTo("temp") == 0) {
        if(debug) {Serial.print(temp_val_arr[i]);}
        BTSerial.print(temp_val_arr[i]);
    } else if(sensor.compareTo("press") == 0) {
        if(press_val_arr[i] <= 10) {
            if(debug) {Serial.print(0);}
            BTSerial.print(0); 
        } else {
            if(debug) {Serial.print(press_val_arr[i]);}
            BTSerial.print(press_val_arr[i]);
        }
    } else {
        if(debug) {Serial.print("ERROR");}
        BTSerial.print("ERROR");
    }    
}



void read_temp_sensor(int i) {
    int raw=analogRead(temp_pin_arr[i]);
    float volt=raw * 3.3;
    volt/=1024.0;
    float temp=(volt - 0.5) * 100.0;
  
    if(temp < 20.0)
        temp=20.0;
    else if(temp > 50)
        temp=50.0;
    
    temp_val_arr[i]=map(temp,20,50,0,50);
  
    delay(10);
}



void read_press_sensor(int i) {
    press_val_arr[i]=map(analogRead(press_pin_arr[i]),0,550,0,255);
    delay(10);
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
        if(debug) Serial.println();
        if(debug) Serial.print("RECEIVED: ");
        
        String sensor_data=receivedChars;
        //Serial.println(sensor_data);
        parse_sensor_readings(sensor_data);
        write_to_actuators();
        newData = false;
    }
}



void parse_sensor_readings(String sensor_data) {
    if(debug) Serial.println(sensor_data);
    int s_idx=0;
    String tmp="";
    char c;
    int i=0;
  
    while(c=sensor_data[s_idx]) {
      
       if(c == ';') {
          if(debug) {Serial.print(tmp);}
          if(debug) {Serial.print(';');}
          if(i<num_servo_acts) {
              servo_val_arr[i]=tmp.toInt();
          }
  
          tmp="";
          i++;
       } else {
          tmp+=c;      
       }
      s_idx++;
    }   
    if(i<num_servo_acts) {
        servo_val_arr[i]=tmp.toInt();  
    }
    tmp="";
    
    if(debug) {Serial.print("last=");}
    if(debug) {Serial.print(tmp);}    
}



void write_to_actuators() {
    //for(int i=0;i<num_servo_acts;i++)
//      delta_servo_val_arr[i]=abs(servo_val_arr[i]-last_servo_val_arr[i]);
    if(debug) {Serial.print("SERVO: ");}
    for(int i=0;i<num_servo_acts;i++) {
//        if(delta_servo_val_arr[i] < 10)
          if(servo_val_arr[i] > servo_max_val[i]) {
            servo_act_arr[i].write(servo_max_val[i]);
          } else if(servo_val_arr[i] < 5) {
            servo_act_arr[i].write(0);
          } else {
            servo_act_arr[i].write(servo_val_arr[i]);
          }        
//        if(debug) {Serial.print(delta_servo_val_arr[i]);}
        if(debug) {Serial.print(",");}
    }
    if(debug) Serial.println();
//    for(int i=0;i<num_vibe_acts;i++)
//        analogWrite(vibe_pin_arr[i],vibe_val_arr[i]);    
//    for(int i=0;i<num_servo_acts;i++)
//      last_servo_val_arr[i]=servo_val_arr[i];
    
}





/*****     MISC HELPER FUNCTIONS     *****/

void test_robo_arm() {  
    for(int j=0;j<num_servo_acts;j++) {
        
        for(int i=0;i<servo_max_val[j];i++) {
            servo_act_arr[j].write(i);
            delay(5);
        }
        
        delay(500);
        
        for(int i=servo_max_val[j];i>=0;i--) {
            servo_act_arr[j].write(i);
            delay(5);
        }
        delay(500);
    }
}



void open_serial_COM() {    
    Serial.begin(9600); // Start Serial Monitor (Console output)
    delay(500);
    BTSerial.begin(9600); // Start BT COMM port
    delay(500);
}



void initialize_servos() {
    for(int i=0;i<num_servo_acts;i++) {
        servo_act_arr[i].attach(servo_pin_arr[i]); // Configure Servo
        delay(100);
        servo_act_arr[i].write(servo_val_arr[i]);  // Write servo to initial position
        delay(100);
    }
}
