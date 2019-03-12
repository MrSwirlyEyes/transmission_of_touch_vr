#include "PCA9685.h"
#include <Wire.h>

// Set to true to print some debug messages, or false to disable them.
// #define ENABLE_DEBUG_OUTPUT




PCA9685::PCA9685(uint8_t addr) {
  _i2caddr = addr;

	#if defined(ARDUINO_SAM_DUE)
		_i2c = &Wire1;
	#else
		_i2c = &Wire;
	#endif
}



void PCA9685::begin(void) {
  _i2c->begin();
  reset();
  // set a default frequency
  setPWMFreq(1000);
}



void PCA9685::reset(void) {
  write8(PCA9685_MODE1, 0x80);
  delay(10);
}



void PCA9685::setPWMFreq(float freq) {
#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Attempting to set freq ");
  Serial.println(freq);
#endif

  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;

#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Estimated pre-scale: "); Serial.println(prescaleval);
#endif

  uint8_t prescale = floor(prescaleval + 0.5);
#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Final pre-scale: "); Serial.println(prescale);
#endif
  
  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  write8(PCA9685_MODE1, newmode); // go to sleep
  write8(PCA9685_PRESCALE, prescale); // set the prescaler
  write8(PCA9685_MODE1, oldmode);
  delay(5);
  write8(PCA9685_MODE1, oldmode | 0xa0);  //  This sets the MODE1 register to turn on auto increment.

#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Mode now 0x"); Serial.println(read8(PCA9685_MODE1), HEX);
#endif
}



void PCA9685::setPWM(uint8_t num, uint16_t on, uint16_t off) {
#ifdef ENABLE_DEBUG_OUTPUT
  Serial.print("Setting PWM "); Serial.print(num); Serial.print(": "); Serial.print(on); Serial.print("->"); Serial.println(off);
#endif

  _i2c->beginTransmission(_i2caddr);
  _i2c->write(LED0_ON_L+4*num);
  _i2c->write(on);
  _i2c->write(on>>8);
  _i2c->write(off);
  _i2c->write(off>>8);
  _i2c->endTransmission();
}



///////////////////////////////
//     PRIVATE FUNCTIONS     //
///////////////////////////////

uint8_t PCA9685::read8(uint8_t addr) {
  _i2c->beginTransmission(_i2caddr);
  _i2c->write(addr);
  _i2c->endTransmission();

  _i2c->requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  return _i2c->read();
}



void PCA9685::write8(uint8_t addr, uint8_t d) {
  _i2c->beginTransmission(_i2caddr);
  _i2c->write(addr);
  _i2c->write(d);
  _i2c->endTransmission();
}
