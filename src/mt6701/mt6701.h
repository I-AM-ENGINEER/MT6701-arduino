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

#ifndef MT6701_H__
#define MT6701_H__

#include <stdint.h>
#include <stdbool.h>

#define MT6701_DEFAULT_ADDRESS				0x06

#define MT6701_OK							0
#define MT6701_ERR_GENERAL					1
#define MT6701_ERR_HANDLER_NULL				2
#define MT6701_ERR_CONFIG_UNAVAILABLE		3
#define MT6701_ERR_IO						4
#define MT6701_ERR_OUT_OF_RANGE				5
#define MT6701_ERR_UNINITITIALIZED			6

#define MT6701_REG_ANGLE0					0x04
#define MT6701_REG_ANGLE6					0x03
#define MT6701_REG_UVM_MUX					0x25
#define MT6701_REG_ABZ_MUX					0x29
#define MT6701_REG_DIR						0x29
#define MT6701_REG_UVW_RES0					0x30
#define MT6701_REG_ABZ_RES8					0x30
#define MT6701_REG_ABZ_RES0					0x31
#define MT6701_REG_ZERO8					0x32
#define MT6701_REG_PULSE_WIDTH				0x32
#define MT6701_REG_HYST2					0x32
#define MT6701_REG_ZERO0					0x33
#define MT6701_REG_HYST0					0x34
#define MT6701_REG_PWM_FREQ					0x38
#define MT6701_REG_PWM_POL					0x38
#define MT6701_REG_OUT_MODE					0x38
#define MT6701_REG_A_STOP8					0x3E
#define MT6701_REG_A_START8					0x3E
#define MT6701_REG_A_START0					0x3F
#define MT6701_REG_A_STOP0					0x40


#define MT6701_REG_ANGLE0_POS				2
#define MT6701_REG_ANGLE6_POS				0
#define MT6701_REG_UVM_MUX_POS				7
#define MT6701_REG_ABZ_MUX_POS				6
#define MT6701_REG_DIR_POS					1
#define MT6701_REG_UVW_RES0_POS				4
#define MT6701_REG_ABZ_RES8_POS				0
#define MT6701_REG_ABZ_RES0_POS				0
#define MT6701_REG_ZERO8_POS				0
#define MT6701_REG_PULSE_WIDTH_POS			4
#define MT6701_REG_HYST2_POS				7
#define MT6701_REG_ZERO0_POS				0
#define MT6701_REG_HYST0_POS				6
#define MT6701_REG_PWM_FREQ_POS				7
#define MT6701_REG_PWM_POL_POS				6
#define MT6701_REG_OUT_MODE_POS				5
#define MT6701_REG_A_STOP8_POS				4
#define MT6701_REG_A_START8_POS				0
#define MT6701_REG_A_START0_POS				0
#define MT6701_REG_A_STOP0_POS				0


#define MT6701_REG_ANGLE0_MASK				(0x3F << MT6701_REG_ANGLE0_POS)
#define MT6701_REG_ANGLE6_MASK				(0xFF << MT6701_REG_ANGLE6_POS)
#define MT6701_REG_UVM_MUX_MASK				(0x01 << MT6701_REG_UVM_MUX_POS)
#define MT6701_REG_ABZ_MUX_MASK				(0x01 << MT6701_REG_ABZ_MUX_POS)
#define MT6701_REG_DIR_MASK					(0x01 << MT6701_REG_DIR_POS)
#define MT6701_REG_UVW_RES0_MASK			(0x0F << MT6701_REG_UVW_RES0_POS)
#define MT6701_REG_ABZ_RES8_MASK			(0x03 << MT6701_REG_ABZ_RES8_POS)
#define MT6701_REG_ABZ_RES0_MASK			(0xFF << MT6701_REG_ABZ_RES0_POS)
#define MT6701_REG_ZERO8_MASK				(0x0F << MT6701_REG_ZERO8_POS)
#define MT6701_REG_PULSE_WIDTH_MASK			(0x07 << MT6701_REG_PULSE_WIDTH_POS)
#define MT6701_REG_HYST2_MASK				(0x01 << MT6701_REG_HYST2_POS)
#define MT6701_REG_ZERO0_MASK				(0xFF << MT6701_REG_ZERO0_POS)
#define MT6701_REG_HYST0_MASK				(0x03 << MT6701_REG_HYST0_POS)
#define MT6701_REG_PWM_FREQ_MASK			(0x01 << MT6701_REG_PWM_FREQ_POS)
#define MT6701_REG_PWM_POL_MASK				(0x01 << MT6701_REG_PWM_POL_POS)
#define MT6701_REG_OUT_MODE_MASK			(0x01 << MT6701_REG_OUT_MODE_POS)
#define MT6701_REG_A_STOP8_MASK				(0x0F << MT6701_REG_A_STOP8_POS)
#define MT6701_REG_A_START8_MASK			(0x0F << MT6701_REG_A_START8_POS)
#define MT6701_REG_A_START0_MASK			(0xFF << MT6701_REG_A_START0_POS)
#define MT6701_REG_A_STOP0_MASK				(0xFF << MT6701_REG_A_STOP0_POS)		 

typedef enum{
	MT6701_MODE_NONE,
	MT6701_MODE_UVW,
	MT6701_MODE_ABZ,
} mt6701_mode_t;

typedef enum{
	MT6701_HYST_1		= 0x0,
	MT6701_HYST_2		= 0x1,
	MT6701_HYST_4		= 0x2,
	MT6701_HYST_8		= 0x3,
	MT6701_HYST_0_25	= 0x5,
	MT6701_HYST_0_5		= 0x6,
} mt6701_hyst_t;

typedef enum{
    MT6701_INTERFACE_NONE,
	MT6701_INTERFACE_I2C,
	MT6701_INTERFACE_SSI,
} mt6701_interface_t;

typedef enum{
	MT6701_DIRECTION_CW		= 0x0,
	MT6701_DIRECTION_CCW	= 0x1,
} mt6701_direction_t;

typedef enum{
	MT6701_PWM_FREQ_994_4	= 0x0,
	MT6701_PWM_FREQ_497_2	= 0x1,
} mt6701_pwm_freq_t;

typedef enum{
	MT6701_PWM_POL_HIGH		= 0x0,
	MT6701_PWM_POL_LOW		= 0x1,
} mt6701_pwm_pol_t;

typedef enum{
	MT6701_OUT_MODE_ANALOG	= 0x0,
	MT6701_OUT_MODE_PWM		= 0x1,
} mt6701_out_mode_t;

typedef enum{
	MT6701_PULSE_WIDTH_1LSB 	= 0x0,
	MT6701_PULSE_WIDTH_2LSB 	= 0x1,
	MT6701_PULSE_WIDTH_4LSB 	= 0x2,
	MT6701_PULSE_WIDTH_8LSB 	= 0x3,
	MT6701_PULSE_WIDTH_12LSB 	= 0x4,
	MT6701_PULSE_WIDTH_16LSB 	= 0x5,
	MT6701_PULSE_WIDTH_180 		= 0x6,
} mt6701_pulse_width_t;

typedef enum{
	MT6701_STATUS_NORM			= 0x0,
	MT6701_STATUS_FIELD_STRONG	= 0x1,
	MT6701_STATUS_FIELD_WEAK	= 0x2,
	MT6701_STATUS_FIELD_ERROR	= 0x3,
} mt6701_status_t;

typedef struct{
	uint8_t (*i2c_read)( uint8_t reg, uint8_t *data );
	uint8_t (*i2c_write)( uint8_t reg, uint8_t data );
	uint8_t (*ssi_read)( uint8_t *data, uint8_t len );
	void (*delay)( uint32_t ms );
	mt6701_interface_t interface;
	mt6701_mode_t mode;
	bool initialized;
} mt6701_handle_t;

#ifdef __cplusplus
extern "C"{
#endif 

/// @brief Enable -a-b-z UVW mode (only for QFN package)
/// @param handle mt6701 handler
/// @param nanbnz_enable 
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_nanbnz_enable( mt6701_handle_t *handle, bool nanbnz_enable );

/// @brief Set resolution for ABZ mode
/// @param handle mt6701 handler
/// @param pulses pulses per round [1...1024]
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_abz_pulse_per_round_set( mt6701_handle_t *handle, uint16_t pulses );

/// @brief Set resolution for UVW mode
/// @param handle mt6701 handler
/// @param pole_pairs pole_pairs [1...16]
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_uvw_pole_pair_set( mt6701_handle_t *handle, uint8_t pole_pairs );

/// @brief Set mode (UVW or ABZ)
/// @param handle mt6701 handler
/// @param mode MT6701_MODE_UVW or MT6701_MODE_ABZ
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_mode_set( mt6701_handle_t *handle, mt6701_mode_t mode );

/// @brief Set communication interface, after set interface, sensor must be reinit
/// @param handle mt6701 handler
/// @param interface MT6701_INTERFACE_SSI or MT6701_INTERFACE_I2C
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_interface_set( mt6701_handle_t *handle, mt6701_interface_t interface );

/// @brief Set zero offset in 12 bit position value
/// @param handle mt6701 handler
/// @param zero_angle Raw angle [0...4096]
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_zero_set_raw( mt6701_handle_t *handle, uint16_t zero_angle );

/// @brief Set zero offset in float value
/// @param handle mt6701 handler
/// @param zero_angle Angle [0...360.0)
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_zero_set( mt6701_handle_t *handle, float zero_angle );

/// @brief Set hysteresis
/// @param handle mt6701 handler
/// @param hysteresis MT6701_HYST_x, where x can be 0_25, 0_5, 1, 2, 4, 8
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_hyst_set( mt6701_handle_t *handle, mt6701_hyst_t hysteresis );

/// @brief Read raw position and status values. If value isnt needed, pointer can be NULL. WARNING! field_status, button_pushed, track_loss READS ONLY WITH SSI INTERFACE!
/// @param handle mt6701 handler
/// @param angle_raw Raw angle [0...16383]
/// @param field_status Can be MT6701_STATUS_NORM, MT6701_STATUS_FIELD_STRONG, MT6701_STATUS_FIELD_WEAK
/// @param button_pushed True if button pushed, false if isnt 
/// @param track_loss True if loss detected, false if isnt
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_read_raw( mt6701_handle_t *handle, uint16_t *angle_raw, mt6701_status_t *field_status, bool *button_pushed, bool *track_loss );

/// @brief Read position value in degrees
/// @param handle mt6701 handler
/// @param angle Angle float [0...360.0)
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_read( mt6701_handle_t *handle, float *angle, mt6701_status_t *field_status, bool *button_pushed, bool *track_loss );

/// @brief Set start and stop angles raw
/// @param handle 
/// @param start [0...4096]
/// @param stop [0...4096]
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_a_start_stop_set_raw( mt6701_handle_t *handle, uint16_t start, uint16_t stop );

/// @brief Set start and stop angles raw
/// @param handle mt6701 handler
/// @param start [0...360.0)
/// @param stop [0...360.0)
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_a_start_stop_set( mt6701_handle_t *handle, float start, float stop );

/// @brief Set direction
/// @param handle mt6701 handler
/// @param mt6701_direction_t MT6701_DIRECTION_CW or MT6701_DIRECTION_CCW
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_direction_set( mt6701_handle_t *handle, mt6701_direction_t direction );

/// @brief Pulse width set
/// @param handle 
/// @param pulse_width 
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_pulse_width_set( mt6701_handle_t *handle, mt6701_pulse_width_t pulse_width );

/// @brief Set PWM frequency (994.4 Hz or 497.2 Hz)
/// @param handle mt6701 handler
/// @param pwm_freq MT6701_PWM_FREQ_994_4 or MT6701_PWM_FREQ_497_2
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_pwm_freq_set( mt6701_handle_t *handle, mt6701_pwm_freq_t pwm_freq );

/// @brief Set PWM polarity
/// @param handle mt6701 handler
/// @param pwm_freq MT6701_PWM_POL_HIGH or MT6701_PWM_POL_LOW
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_pwm_polarity_set( mt6701_handle_t *handle, mt6701_pwm_pol_t pwm_polarity );

/// @brief Set analog or PWM mode of ic output
/// @param handle mt6701 handler
/// @param out_mode MT6701_OUT_MODE_ANALOG or MT6701_OUT_MODE_PWM
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_out_mode_set( mt6701_handle_t *handle, mt6701_out_mode_t out_mode );

/// @brief Save current settings to EEPROM.
/// @param handle mt6701 handler
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_programm_eeprom( mt6701_handle_t *handle );

/// @brief Perfom init for mt6701. Before call interface MUST be selected and selected interface handlers MUST be defined, plus delay handler MUST be set
/// @param handle mt6701 handler
/// @return On OK return 0, else see MT6701_ERR codes
uint8_t mt6701_init( mt6701_handle_t *handle );

#ifdef __cplusplus
}
#endif

#endif // !MT6701_H__
