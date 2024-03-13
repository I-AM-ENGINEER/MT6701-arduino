# Arduino driver for MT6701

The MT6701 is an easy to program magnetic rotary position sensor with a high-resolution 12-bit analog, PWM, UVW, ABZ. With SPI and I2C output is 14 bit. They should provide magnetic button functionality, but datasheet is messy and i cant get this mode work.

This library provide simple API for configure module with I2C and flash EEPROM. Also lib provide interface for read angle and magnetic pole status (too strong/too low/nornal) throught SPI and I2C (with I2C only angle read possible)

This is arduino wrapper for my [MT6701 lib](https://github.com/I-AM-ENGINEER/MT6701-driver), implement all chip config functions, exclude button and fail detection (because they broken?).

## Install
1) Download ZIP archive. Just click [here](https://github.com/I-AM-ENGINEER/MT6701-arduino/archive/refs/heads/main.zip)
2) In Arduino IDE select tab Sketch->Include Library->Add .ZIP Library
3) Thats all, you can open examples (maybe need restart Arduino IDE) in tab File->Examples->MT6701

## Available interfaces

### SSI
SSI is fast, you can connect many encoders, setting different CSN for each one. You can only read angle and pole strength, write settings in SSI impossible due chip limitations.
Lib support only SSI over hardware SPI, this pinout for arduino UNO/nano, i didnt test on others boards.
If you have problems with reading stability, you can adjust connection speed by redefing SSI speed (default 1MHz, max 8MHz), BEFORE (important!) include library.

#### Wiring
```
 Arduino     |      MT6701
  PIN13   <----->    CLK
  PIN12   <----->    DO/SDA
  PINx    <----->    CSN
```

For CSN you can use any free pin, expect 0, 1, 11, in examples i use pin 9.

You can connect other SPI sensors, they should functional fine.

#### Initialization
```c
#define MT6701_SSI_CLOCK    100000    // Define 100kHz SSI speed before <MT6701.h> include
#include <SPI.h>
#include <MT6701.h>
...
#define CS_PIN  9  // CSN pin of NT6701

MT6701 encoder;

void setup() {
  ...
  SPI.begin();
  encoder.initializeSSI(CS_PIN);
  ...
}
```

### I2C
I2C uses mainly for one time configuration. You can configure every option of encoder and save settings in chip EEPROM. I dont recomend use this interface, if you only need read angle, use SSI or any other options from datasheet.
Because MT6701 cant change I2C address, only one encoder could be connected at same time.

#### Wiring
```
 Arduino    |      MT6701
   A5    <----->    CLK
   A4    <----->    DO/SDA
   5V    <----->    CSN
```

#### Initialization
```c
#include <Wire.h>
#include <MT6701.h>
...

MT6701 encoder;

void setup() {
  ...
  Wire.begin();
  encoder.initializeI2C();
  ...
}
```

## Usage

For both (SSI and I2C) interfaces available function for read angle, they return 0...360 float value:
```c
float angle = encoder.angleRead();
```

One SSI only function - read field status (see example 02_spi_read for details): 
```c
mt6701_status_t field_status = encoder.fieldStatusRead();
```

Other functions are I2C only and used for configure chip for imitate other encoders types. Full list:

```c
  void uvwModeSet( uint8_t pole_pairs );
  void abzModeSet( uint16_t pulses_per_round, mt6701_pulse_width_t z_pulse_width, mt6701_hyst_t hysteresis );
  void nanbnzEnable( bool nanbnz_enable ); // Only for QFN version
  // Analog/PWM pin mode select
  void analogModeSet( float start, float stop );
  void pwmModeSet( mt6701_pwm_freq_t frequency, mt6701_pwm_pol_t polarity );
  // General tracking options
  void offsetSet( float offset );
  void directionSet( mt6701_direction_t direction );
  // Save settings as default for chip
  void programmEEPROM( void );
```

If you need more information for this functions, read examples they include all function and arguments description. For further information, read datasheet
