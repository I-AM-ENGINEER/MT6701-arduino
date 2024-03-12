#ifndef MT6701_ARDUINO_WRAPPER_H__
#define MT6701_ARDUINO_WRAPPER_H__
#include <Arduino.h>
#include "mt6701/mt6701.h"

#ifndef MT6701_SSI_CLOCK
#define MT6701_SSI_CLOCK    1000000
#endif

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
	bool _buttonRead( void ); // I think this isnt work, would be chip error
	bool _trackLossCheck( void ); // I think this isnt work, would be chip error

	// I2C only functions
	void uvwModeSet( uint8_t pole_pairs );
	void abzModeSet( uint16_t pulses_per_round );
	void nanbnzEnable( bool nanbnz_enable ); // Only for QFN version
	// Analog/PWM pin mode select
	void analogModeSet( float start, float stop );
	void pwmModeSet( mt6701_pwm_freq_t frequency, mt6701_pwm_pol_t polarity );
	// General tracking options
	void offsetSet( float offset );
	void hysteresisSet( mt6701_hyst_t hysteresis );
	void directionSet( mt6701_direction_t direction );
	// Save settings as default
	void programmEEPROM( void );

private:
	int cs_pin;
	mt6701_handle_t handle;

	static uint8_t ssi_read( uint8_t* data, uint8_t len );
	static uint8_t i2c_write( uint8_t reg, uint8_t data );
	static uint8_t i2c_read( uint8_t reg, uint8_t *data );
	static int selected_cs;
};
#endif // MT6701_ARDUINO_WRAPPER_H__
