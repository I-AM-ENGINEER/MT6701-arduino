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

#include "mt6701.h"
#include <stdlib.h>

static uint8_t mt6701_check_config_mode( mt6701_handle_t *handle ){
	if(handle == NULL){
		return MT6701_ERR_HANDLER_NULL;
	}

	if(handle->initialized != true){
		return MT6701_ERR_UNINITITIALIZED;
	}

	if(handle->interface != MT6701_INTERFACE_I2C){
		return MT6701_ERR_CONFIG_UNAVAILABLE;
	}

	return MT6701_OK;
}

uint8_t mt6701_nanbnz_enable( mt6701_handle_t *handle, bool nanbnz_enable ){	
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	res = handle->i2c_read(MT6701_REG_UVM_MUX, &data);

	if(nanbnz_enable){
		data |= MT6701_REG_UVM_MUX_MASK;
	}else{
		data &= ~MT6701_REG_UVM_MUX_MASK;
	}
	
	res = handle->i2c_write(MT6701_REG_UVM_MUX, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_abz_pulse_per_round_set( mt6701_handle_t *handle, uint16_t resolution ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	resolution--;
	if(resolution >= 1024){
		return MT6701_ERR_OUT_OF_RANGE;
	}

	res = handle->i2c_read(MT6701_REG_ABZ_RES8, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	res = handle->i2c_write(MT6701_REG_ABZ_RES0, (uint8_t)resolution);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	resolution >>= 8;
	data &= ~MT6701_REG_ABZ_RES8_MASK;
	data |= (uint8_t)(resolution << MT6701_REG_ZERO8_POS);

	res = handle->i2c_write(MT6701_REG_ABZ_RES8, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_uvw_pole_pair_set( mt6701_handle_t *handle, uint8_t pole_pairs ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	pole_pairs--;
	if(pole_pairs >= 16){
		return MT6701_ERR_OUT_OF_RANGE;
	}

	res = handle->i2c_read(MT6701_REG_UVW_RES0, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	data &= ~MT6701_REG_UVW_RES0_MASK;
	data |= (MT6701_REG_UVW_RES0_POS << pole_pairs);

	res = handle->i2c_write(MT6701_REG_UVW_RES0, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	return MT6701_OK;
}

uint8_t mt6701_mode_set( mt6701_handle_t *handle, mt6701_mode_t mode ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	res = handle->i2c_read(MT6701_REG_ABZ_MUX, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	if(mode == MT6701_MODE_UVW){
		data |=  MT6701_REG_ABZ_MUX_MASK;
	}else if(mode == MT6701_MODE_ABZ){
		data &= ~MT6701_REG_ABZ_MUX_MASK;
	}

	res = handle->i2c_write(MT6701_REG_ABZ_MUX, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_interface_set( mt6701_handle_t *handle, mt6701_interface_t interface ){
	if(handle == NULL){
		return MT6701_ERR_HANDLER_NULL;
	}

	handle->initialized = false;
	handle->interface = interface;

	return MT6701_OK;
}

uint8_t mt6701_zero_set_raw( mt6701_handle_t *handle, uint16_t zero_angle ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}
	
	zero_angle &= 0x0FFF;	

	res = handle->i2c_write(MT6701_REG_ZERO0, (uint8_t)zero_angle);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	res = handle->i2c_read(MT6701_REG_ZERO8, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	zero_angle >>= 8;
	data &= ~MT6701_REG_ZERO8_MASK;
	data |= (zero_angle << MT6701_REG_ZERO8_POS);

	res = handle->i2c_write(MT6701_REG_ZERO8, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	return MT6701_OK;
}

uint8_t mt6701_zero_set( mt6701_handle_t *handle, float zero_angle ){
	uint8_t res;
	uint16_t data;

	data = (uint16_t)(zero_angle * (4096.0f/360.0f));
	res = mt6701_zero_set_raw(handle, data);

	return res;
}

uint8_t mt6701_hyst_set( mt6701_handle_t *handle, mt6701_hyst_t hysteresis ){
	uint8_t res;
	uint8_t data;
	uint8_t hyst_lo;
	uint8_t hyst_hi;
	
	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return 1;
	}

	res = handle->i2c_read(MT6701_REG_HYST0, &data);
	if(res != 0){
		return 1;
	}

	hyst_lo = hysteresis & 0x03;
	data &= ~MT6701_REG_HYST0_MASK;
	data |= (hyst_lo << MT6701_REG_HYST0_POS);

	res = handle->i2c_write(MT6701_REG_HYST0, data);
	if(res != 0){
		return 1;
	}

	res = handle->i2c_read(MT6701_REG_HYST2, &data);
	if(res != 0){
		return 1;
	}

	hyst_hi = hysteresis >> 2;
	data &= ~MT6701_REG_HYST2_MASK;
	data |= (hyst_hi << MT6701_REG_HYST2_POS);

	res = handle->i2c_write(MT6701_REG_HYST2, data);
	if(res != 0){
		return 1;
	}

	return MT6701_OK;
}

uint8_t mt6701_a_start_stop_set_raw( mt6701_handle_t *handle, uint16_t start, uint16_t stop ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	if(start >= 4096){
		return MT6701_ERR_OUT_OF_RANGE;
	}

	if(stop >= 4096){
		return MT6701_ERR_OUT_OF_RANGE;
	}

	res = handle->i2c_write(MT6701_REG_A_START0, (uint8_t)start);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	res = handle->i2c_write(MT6701_REG_A_STOP0, (uint8_t)stop);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	start >>= 8;
	stop  >>= 8;
	data = (start << MT6701_REG_A_START8_POS) | (stop << MT6701_REG_A_STOP8_POS);

	res = handle->i2c_write(MT6701_REG_A_START8, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_a_start_stop_set( mt6701_handle_t *handle, float start, float stop ){
	uint8_t res;
	uint16_t start_u16;
	uint16_t stop_u16;

	start_u16 = (uint16_t)(start * (4096.0f/360.0f));
	stop_u16  = (uint16_t)(stop * (4096.0f/360.0f));
	if(start_u16 >= 4096){
		start_u16 = 0;
	}
	if(stop_u16 >= 4096){
		stop_u16 = 4095;
	}
	res = mt6701_a_start_stop_set_raw(handle, start_u16, stop_u16);
	return res;
}

uint8_t mt6701_direction_set( mt6701_handle_t *handle, mt6701_direction_t direction ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return MT6701_ERR_HANDLER_NULL;
	}

	res = handle->i2c_read(MT6701_REG_DIR, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	if(direction == MT6701_DIRECTION_CW){
		data &= ~MT6701_REG_DIR_MASK;
	}else if(direction == MT6701_DIRECTION_CCW){
		data |=  MT6701_REG_DIR_MASK;
	}else{
		return MT6701_ERR_GENERAL;
	}

	res = handle->i2c_write(MT6701_REG_DIR, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	return MT6701_OK;
}

uint8_t mt6701_pulse_width_set( mt6701_handle_t *handle, mt6701_pulse_width_t pulse_width ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	res = handle->i2c_read(MT6701_REG_PULSE_WIDTH, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	data &= ~MT6701_REG_PULSE_WIDTH_MASK;
	data |= (pulse_width << MT6701_REG_PULSE_WIDTH_POS);

	res = handle->i2c_write(MT6701_REG_PULSE_WIDTH, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_pwm_freq_set( mt6701_handle_t *handle, mt6701_pwm_freq_t pwm_freq ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	res = handle->i2c_read(MT6701_REG_PWM_FREQ, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	if(pwm_freq == MT6701_PWM_FREQ_994_4){
		data &= ~MT6701_REG_PWM_FREQ_MASK;
	}else if(pwm_freq == MT6701_PWM_FREQ_497_2){
		data |=  MT6701_REG_PWM_FREQ_MASK;
	}else{
		return MT6701_ERR_GENERAL;
	}

	res = handle->i2c_write(MT6701_REG_PWM_FREQ, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_pwm_polarity_set( mt6701_handle_t *handle, mt6701_pwm_pol_t pwm_polarity ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	res = handle->i2c_read(MT6701_REG_PWM_POL, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	if(pwm_polarity == MT6701_PWM_POL_HIGH){
		data &= ~MT6701_REG_PWM_POL_MASK;
	}else if(pwm_polarity == MT6701_PWM_POL_LOW){
		data |=  MT6701_REG_PWM_POL_MASK;
	}else{
		return MT6701_ERR_GENERAL;
	}

	res = handle->i2c_write(MT6701_REG_PWM_POL, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_out_mode_set( mt6701_handle_t *handle, mt6701_out_mode_t out_mode ){
	uint8_t res;
	uint8_t data;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	res = handle->i2c_read(MT6701_REG_PWM_POL, &data);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	
	if(out_mode == MT6701_OUT_MODE_ANALOG){
		data &= ~MT6701_REG_OUT_MODE_MASK;
	}else if(out_mode == MT6701_OUT_MODE_PWM){
		data |=  MT6701_REG_OUT_MODE_MASK;
	}else{
		return MT6701_ERR_GENERAL;
	}

	res = handle->i2c_write(MT6701_REG_OUT_MODE, data);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	return MT6701_OK;
}

uint8_t mt6701_init( mt6701_handle_t *handle ){
	if(handle == NULL){
		return MT6701_ERR_HANDLER_NULL;
	}

	handle->initialized = false;

	if(handle->interface == MT6701_INTERFACE_I2C){
		if(handle->i2c_read == NULL){
			return MT6701_ERR_GENERAL;
		}

		if(handle->i2c_write == NULL){
			return MT6701_ERR_GENERAL;
		}
	}else if(handle->interface == MT6701_INTERFACE_SSI){
		if(handle->ssi_read == NULL){
			return MT6701_ERR_GENERAL;
		}
	}

	if(handle->delay == NULL){
		return MT6701_ERR_GENERAL;
	}

	handle->initialized = true;

	return MT6701_OK;
}

uint8_t mt6701_programm_eeprom( mt6701_handle_t *handle ){
	uint8_t res;

	res = mt6701_check_config_mode(handle);
	if(res != 0){
		return res;
	}

	if(handle->delay == NULL){
		return MT6701_ERR_GENERAL;
	}

	res = handle->i2c_write(0x09, 0xB3);
	if(res != 0){
		return MT6701_ERR_IO;
	}

	res = handle->i2c_write(0x0A, 0x05);
	if(res != 0){
		return MT6701_ERR_IO;
	}
	handle->initialized = false;
	handle->delay(600);
	handle->initialized = true;

	return MT6701_OK;
}

uint8_t mt6701_read_raw( mt6701_handle_t *handle, uint16_t *angle_raw, mt6701_status_t *field_status, bool *button_pushed, bool *track_loss ){
	uint8_t res;
	uint8_t data[3];
	uint8_t status;
	uint16_t angle_u16;

	if(handle == NULL){
		return MT6701_ERR_HANDLER_NULL;
	}

	if(handle->initialized != true){
		return MT6701_ERR_UNINITITIALIZED;
	}

	if(handle->interface == MT6701_INTERFACE_I2C){
		res = handle->i2c_read(MT6701_REG_ANGLE6, &data[1]);
		if(res != 0){
			return MT6701_ERR_IO;
		}
		
		res = handle->i2c_read(MT6701_REG_ANGLE0, &data[0]);
		if(res != 0){
			return MT6701_ERR_IO;
		}

		angle_u16  = (uint16_t)(data[0] >> MT6701_REG_ANGLE0_POS);
  		angle_u16 |= ((uint16_t)data[1] << (8-MT6701_REG_ANGLE0_POS));
	}else{
		res = handle->ssi_read(data, 3);
		if(res != 0){
			return MT6701_ERR_IO;
		}

		angle_u16  = (uint16_t)(data[1] >> 2);
		angle_u16 |= ((uint16_t)data[0] << 6);
		
		status  = (data[2] >> 6);
		status |= (data[1] & 0x03) << 2;

		// I dont check CRC6, becouse i soo stupid to implement this rare shit

		if(field_status != NULL){
			*field_status = status & 0x03;
		}

		if(button_pushed != NULL){
			if(status & 0x04){
				*button_pushed = true; 
			}else{
				*button_pushed = false;
			}
		}
		
		if(track_loss != NULL){
			if(status & 0x08){
				*track_loss = true;
			}else{
				*track_loss = false;
			}
		}
	}

	if(angle_raw != NULL){
		*angle_raw = angle_u16; 
	}
	return MT6701_OK;
}

uint8_t mt6701_read( mt6701_handle_t *handle, float *angle, mt6701_status_t *field_status, bool *button_pushed, bool *track_loss ){
	uint8_t res;
	uint16_t angle_u16;
	float angle_f;

	res = mt6701_read_raw(handle, &angle_u16, field_status, button_pushed, track_loss);
	if(res != 0){
		return res;
	}

	if(angle != NULL){
		angle_f = (float)angle_u16 * (360.0f/16384.0f);
		*angle = angle_f;
	}

	return MT6701_OK;
}
