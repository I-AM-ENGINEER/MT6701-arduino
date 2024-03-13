#include "MT6701.h"
#include <Wire.h>
#include <SPI.h>

int MT6701::selected_cs = 9; 

uint8_t MT6701::ssi_read( uint8_t* data, uint8_t len ){
	if(len != 3){
		return 1;
	}
	// Yes, this shit work
	uint32_t tmp;
	SPI.beginTransaction(SPISettings(MT6701_SSI_CLOCK, MSBFIRST, SPI_MODE2));
	digitalWrite(selected_cs, LOW);
	for(uint32_t i = 0; i < 4; i++){
		((uint8_t*)&tmp)[i] = SPI.transfer(0xff);
	}
	tmp <<= 1;
	memcpy(data, &tmp, 3);
	digitalWrite(selected_cs, HIGH);
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

MT6701::MT6701( void ) {
	handle = {0};
	handle.delay = mt6701_delay;
}

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

float MT6701::angleRead( void ){
	float angle = 0.0f;
	this->selected_cs = this->cs_pin;
	mt6701_read(&this->handle, &angle, NULL, NULL, NULL);
	return angle;
}

bool MT6701::_buttonRead( void ){
	bool btn_state = false;
	this->selected_cs = this->cs_pin;
	mt6701_read(&this->handle, NULL, NULL, &btn_state, NULL);
	return btn_state;
}

mt6701_status_t MT6701::fieldStatusRead( void ){
	mt6701_status_t field_status = MT6701_STATUS_FIELD_ERROR;
	this->selected_cs = this->cs_pin;
	mt6701_read(&this->handle, NULL, &field_status, NULL, NULL);
	return field_status;
}

bool MT6701::_trackLossCheck( void ){
	bool loss = false;
	this->selected_cs = this->cs_pin;
	mt6701_read(&this->handle, NULL, NULL, NULL, &loss);
	return loss;
}

void MT6701::uvwModeSet( uint8_t pole_pairs ){
	mt6701_uvw_pole_pair_set(&this->handle, pole_pairs);
	mt6701_mode_set(&this->handle, MT6701_MODE_UVW);
}

void MT6701::abzModeSet( uint16_t pulses_per_round, mt6701_pulse_width_t z_pulse_width, mt6701_hyst_t hysteresis ){
	mt6701_pulse_width_set(&this->handle, z_pulse_width);
	mt6701_hyst_set(&this->handle, hysteresis);
	mt6701_abz_pulse_per_round_set(&this->handle, pulses_per_round);
	mt6701_mode_set(&this->handle, MT6701_MODE_ABZ);
}

void MT6701::nanbnzEnable( bool nanbnz_enable ){
	mt6701_nanbnz_enable(&this->handle, nanbnz_enable);
}

void MT6701::analogModeSet( float start, float stop ){
	mt6701_a_start_stop_set(&this->handle, start, stop);
	mt6701_out_mode_set(&this->handle, MT6701_OUT_MODE_ANALOG);
}

void MT6701::pwmModeSet( mt6701_pwm_freq_t frequency, mt6701_pwm_pol_t polarity ){
	mt6701_out_mode_set(&this->handle, MT6701_OUT_MODE_PWM);
	mt6701_pwm_freq_set(&this->handle, frequency);
	mt6701_pwm_polarity_set(&this->handle, polarity);
}

void MT6701::offsetSet( float offset ){
	mt6701_zero_set(&this->handle, offset);
}

void MT6701::directionSet( mt6701_direction_t direction ){
	mt6701_direction_set(&this->handle, direction );
}

void MT6701::programmEEPROM( void ){
	mt6701_programm_eeprom(&this->handle);
}
