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

#include "MT6701.h"
#include <Wire.h>
#include <SPI.h>

uint8_t MT6701::ssi_read( uint8_t* data, uint8_t len ){
	if(len != 3){
		return 1;
	}
	uint8_t tmp[3];
	SPI.beginTransaction(SPISettings(MT6701_SSI_CLOCK, MSBFIRST, SPI_MODE1));
	tmp[0] = SPI.transfer(0xff);
	tmp[1] = SPI.transfer(0xff);
	tmp[2] = SPI.transfer(0xff);
	memcpy(data, &tmp, 3);
	SPI.endTransaction();
	return 0;
}

uint8_t MT6701::i2c_read( uint8_t reg, uint8_t *data ){  
	Wire.beginTransmission(MT6701_DEFAULT_ADDRESS);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(MT6701_DEFAULT_ADDRESS, 1);
	if (Wire.available()) {
		*data = Wire.read();
	}else{
		return 1;
	}
	return 0;
}

uint8_t MT6701::i2c_write( uint8_t reg, uint8_t data ){
	Wire.beginTransmission(MT6701_DEFAULT_ADDRESS);
	Wire.write(reg);
	Wire.write(data);
	Wire.endTransmission();
	return 0;
}

static void mt6701_delay( uint32_t ms ){
	delay(ms);
}

/*!
 *  @brief  Instantiates a new MT6701 class
 */
MT6701::MT6701( void ) {
	handle = {0};
	handle.delay = mt6701_delay;
}

/*!
 *  @brief  Initialize MT6701 for work over I2C
 */
bool MT6701::initializeI2C( void ){
	uint8_t res;

	res = mt6701_interface_set(&this->handle, MT6701_INTERFACE_I2C);
	if(res != MT6701_OK){
		return false;
	}

	this->handle.i2c_read = this->i2c_read;
	this->handle.i2c_write = this->i2c_write;

	res = mt6701_init(&this->handle);
	if(res != MT6701_OK){
		return false;
	}

	return true;
}

/*!
 *  @brief  Initialize MT6701 for work over ISS over SPI
 *  @param  cs_pin CSN pin of MT6701
 */
bool MT6701::initializeSSI( int cs_pin ){
	uint8_t res;

	res = mt6701_interface_set(&this->handle, MT6701_INTERFACE_SSI);
	if(res != MT6701_OK){
		return false;
	}
	
	this->handle.ssi_read = this->ssi_read;
	res = mt6701_init(&this->handle);
	if(res != MT6701_OK){
		return false;
	}

	this->cs_pin = cs_pin;
	pinMode(this->cs_pin, OUTPUT);
	digitalWrite(this->cs_pin, HIGH);

	return true;
}

/*!
 *  @brief  Return position of encoder
 *  @return Angle value of encoder position
 */
float MT6701::angleRead( void ){
	float angle = 0.0f;
	
	if(this->handle.interface == MT6701_INTERFACE_SSI){
		digitalWrite(this->cs_pin, LOW);
	}

	mt6701_read(&this->handle, &angle, NULL, NULL, NULL);

	if(this->handle.interface == MT6701_INTERFACE_SSI){
		digitalWrite(this->cs_pin, HIGH);
	}
	return angle;
}

/*!
 *  @brief  Return magnetic field status
 *  @return Field status
 */
mt6701_status_t MT6701::fieldStatusRead( void ){
	mt6701_status_t field_status = MT6701_STATUS_FIELD_ERROR;

	if(this->handle.interface == MT6701_INTERFACE_SSI){
		digitalWrite(this->cs_pin, LOW);
	}

	mt6701_read(&this->handle, NULL, &field_status, NULL, NULL);

	if(this->handle.interface == MT6701_INTERFACE_SSI){
		digitalWrite(this->cs_pin, HIGH);
	}
	return field_status;
}

/*!
 *  @brief  Set encoder to UVW mode
 *  @param  pole_pairs Emulated poles count (1...16)
 */
void MT6701::uvwModeSet( uint8_t pole_pairs ){
	mt6701_uvw_pole_pair_set(&this->handle, pole_pairs);
	mt6701_mode_set(&this->handle, MT6701_MODE_UVW);
}

/*!
 *  @brief  Set encoder to ABZ mode
 *  @param  pulses_per_round Pulses per round (1...1024)
 *  @param  z_pulse_width Pin Z output waveform. Possible values: 
 *     MT6701_PULSE_WIDTH_180 or 
 *     MT6701_PULSE_WIDTH_xLSB, where x can be 
 *        1, 2, 4, 8, 12, 16
 *  @param  hysteresis Hysteresis. Possible values: 
 *     MT6701_HYST_x, where x can be 
 *        0_25, 0_5, 1, 2, 4, 8
 */
void MT6701::abzModeSet( uint16_t pulses_per_round, mt6701_pulse_width_t z_pulse_width, mt6701_hyst_t hysteresis ){
	mt6701_pulse_width_set(&this->handle, z_pulse_width);
	mt6701_hyst_set(&this->handle, hysteresis);
	mt6701_abz_pulse_per_round_set(&this->handle, pulses_per_round);
	mt6701_mode_set(&this->handle, MT6701_MODE_ABZ);
}

/*!
 *  @brief  Set nAnBnZ mode for QFN version chip
 *  @param  nanbnz_enable True - nAnBnZ, False - UVW mode
 */
void MT6701::nanbnzEnable( bool nanbnz_enable ){
	mt6701_nanbnz_enable(&this->handle, nanbnz_enable);
}

/*!
 *  @brief  Set analog mode for Analog/PWM output pin
 *  @param  start Min analog position in degrees
 *  @param  stop Max analog position in degrees
 */
void MT6701::analogModeSet( float start, float stop ){
	mt6701_a_start_stop_set(&this->handle, start, stop);
	mt6701_out_mode_set(&this->handle, MT6701_OUT_MODE_ANALOG);
}

/*!
 *  @brief  Set PWM mode for Analog/PWM output pin
 *  @param  mt6701_pwm_freq_t Frequency output. Possible values:
 *      MT6701_PWM_FREQ_994_4; 
 *      MT6701_PWM_FREQ_497_2
 *  @param  stop PWM polarity. Possible values:
 *      MT6701_PWM_POL_HIGH; 
 *      MT6701_PWM_POL_LOW
 */
void MT6701::pwmModeSet( mt6701_pwm_freq_t frequency, mt6701_pwm_pol_t polarity ){
	mt6701_out_mode_set(&this->handle, MT6701_OUT_MODE_PWM);
	mt6701_pwm_freq_set(&this->handle, frequency);
	mt6701_pwm_polarity_set(&this->handle, polarity);
}

/*!
 *  @brief  Set offset for zero point
 *  @param  offset Offset in degrees
 */
void MT6701::offsetSet( float offset ){
	mt6701_zero_set(&this->handle, offset);
}

/*!
 *  @brief  Set direction of encoder
 *  @param  direction Direction of encoder. Possible values:
 *      MT6701_DIRECTION_CW; 
 *      MT6701_DIRECTION_CCW
 */
void MT6701::directionSet( mt6701_direction_t direction ){
	mt6701_direction_set(&this->handle, direction );
}

/*!
 *  @brief  Save current settings to internal chip EEPROM
 */
void MT6701::programmEEPROM( void ){
	mt6701_programm_eeprom(&this->handle);
}
