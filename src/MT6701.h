/*

This is a library for MT6701 encoder IC sensor.

MIT License

Copyright (c) 2024 I_AM_ENGINEER

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MT6701_ARDUINO_WRAPPER_H__
#define MT6701_ARDUINO_WRAPPER_H__

#include <Arduino.h>
#include "mt6701/mt6701.h"

#ifndef MT6701_SSI_CLOCK
#define MT6701_SSI_CLOCK    1000000
#endif

/*!
 *  @brief  Class that stores state and functions for MT6701
 */
class MT6701 {
public:
	MT6701( void );
	// Only hardware I2C and SPI support
	bool initializeI2C( void );
	bool initializeSSI( int cs_pin );

	// SPI & I2C functions
	float angleRead( void );

	// SPI only functions
	mt6701_status_t fieldStatusRead( void );

	// I2C only functions
	void uvwModeSet( uint8_t pole_pairs );
	void abzModeSet( uint16_t pulses_per_round, mt6701_pulse_width_t z_pulse_width = MT6701_PULSE_WIDTH_180, mt6701_hyst_t hysteresis = MT6701_HYST_1 );
	void nanbnzEnable( bool nanbnz_enable ); // Only for QFN version
	// Analog/PWM pin mode select
	void analogModeSet( float start = 0.0f, float stop = 360.0f );
	void pwmModeSet( mt6701_pwm_freq_t frequency = MT6701_PWM_FREQ_497_2, mt6701_pwm_pol_t polarity = MT6701_PWM_POL_HIGH );
	// General tracking options
	void offsetSet( float offset );
	void directionSet( mt6701_direction_t direction );
	// Save settings as default
	void programmEEPROM( void );

private:
	int cs_pin;
	mt6701_handle_t handle;

	// Internal c function for driver
	static uint8_t ssi_read( uint8_t* data, uint8_t len );
	static uint8_t i2c_write( uint8_t reg, uint8_t data );
	static uint8_t i2c_read( uint8_t reg, uint8_t *data );
};

#endif // MT6701_ARDUINO_WRAPPER_H__
