/*
TMP36.h - Library to obtain values from a TMP36Z Temperature Sensor
Created by Colin "MrSwirlyEyes"
For Project in a Box, University of California, San Diego.
*/

#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class TMP36 {
	public:
        TMP36(int _pin,float _vcc);

        void read();
        float get_temperature_C();
        float get_temperature_F();

    private:
        int _pin;    
        float _vcc;
        float _temperature_C;
        float _temperature_F;
};

#endif