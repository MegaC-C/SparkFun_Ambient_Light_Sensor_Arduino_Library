#ifndef _SPARKFUN_VEML6030_H_
#define _SPARKFUN_VEML6030_H_

#include <Wire.h>
#include <Arduino.h>

#define ENABLE        0x01
#define DISABLE       0x00
#define SHUTDOWN      0x01
#define POWER         0x00
#define NO_INT        0x00
#define INT_HIGH      0x01
#define INT_LOW       0x02
#define UNKNOWN_ERROR 0xFF

// 7-Bit address options
const uint8_t defAddr = 0x48;
const uint8_t altAddr = 0x10;

enum VEML6030_16BIT_REGISTERS {

  SETTING_REG            = 0x00,
  H_THRESH_REG,
  L_THRESH_REG,
  POWER_SAVE_REG, 
  AMBIENT_LIGHT_DATA_REG,
  WHITE_LIGHT_DATA_REG,
  INTERRUPT_REG

};

enum VEML6030_16BIT_REG_MASKS {
 
  THRESH_MASK            = 0x0,
  GAIN_MASK              = 0xE7FF,
  INTEG_MASK             = 0xFC3F,
  PERS_PROT_MASK         = 0xFFCF,
  INT_EN_MASK            = 0xFFFD,
  SD_MASK                = 0xFFFE,
  POW_SAVE_EN_MASK       = 0x06, // Most of this register is reserved
  POW_SAVE_MASK          = 0x01, // Most of this register is reserved
  INT_MASK               = 0xC000    
  
};

enum REGISTER_BIT_POSITIONS {

  NO_SHIFT               = 0x00,
  INT_EN_POS             = 0x01,
  PSM_POS                = 0x01,
  PERS_PROT_POS          = 0x04,
  INTEG_POS              = 0x06,
  GAIN_POS               = 0xB,
  INT_POS                = 0xE

};

// Table of lux conversion values depending on the integration time and gain. 
// The arrays represent the all possible integration times and the index of the
// arrays represent the register's gain settings, which is directly analgous to
// their bit representations. 
const float eightHIt[]     = {.0036, .0072, .0288, .0576};
const float fourHIt[]      = {.0072, .0144, .0576, .1152};
const float twoHIt[]       = {.0144, .0288, .1152, .2304};
const float oneHIt[]       = {.0288, .0576, .2304, .4608};
const float fiftyIt[]      = {.0576, .1152, .4608, .9216};
const float twentyFiveIt[] = {.1152, .2304, .9216, 1.8432};

class SparkFun_Ambient_Light_A
{  
  public:

    SparkFun_Ambient_Light_A(uint8_t address_A); // I2C Constructor

    bool begin(TwoWire &wirePort = Wire); // begin function

    // REG0x00, bits [12:11]
    // This function sets the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    void setGain(float gainVal);

    // REG0x00, bits [12:11]
    // This function reads the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    float readGain();

    // REG0x00, bits[9:6]
    // This function sets the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower sensor refresh times. 
    void setIntegTime(uint16_t time);

    // REG0x00, bits[9:6]
    // This function reads the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower sensor refresh times. 
    uint16_t readIntegTime();

    // REG0x00, bit[0]
    // This function powers up the Ambient Light Sensor. The last value that was
    // read during shut down will be overwritten on the sensor's subsequent read.
    // After power up, a small 4ms delay is applied to give time for the internal
    // osciallator and signal processor to power up.   
    void powerOn();

    // REG[0x04], bits[15:0]
    // This function gets the sensor's ambient light's lux value. The lux value is
    // determined based on current gain and integration time settings. If the lux
    // value exceeds 1000 then a compensation formula is applied to it. 
    uint32_t readLight_A();


  private:

    uint8_t _address_A;
    
    // This function compensates for lux values over 1000. From datasheet:
    // "Illumination values higher than 1000 lx show non-linearity. This
    // non-linearity is the same for all sensors, so a compensation forumla..."
    // etc. etc. 
    uint32_t _luxCompensation(uint32_t _luxVal_A);

    // The lux value of the Ambient Light sensor depends on both the gain and the
    // integration time settings. This function determines which conversion value
    // to use by using the bit representation of the gain as an index to look up
    // the conversion value in the correct integration time array. It then converts 
    // the value and returns it.  
    uint32_t _calculateLux(uint16_t _lightBits_A);

    // This function writes to a 16 bit register. Paramaters include the register's address, a mask 
    // for bits that are ignored, the bits to write, and the bits' starting
    // position.
    void _writeRegister(uint8_t _wReg, uint16_t _mask, uint16_t _bits, uint8_t _startPosition);

    // This function reads a 16 bit register. It takes the register's
    // address as its' parameter.
    uint16_t _readRegister(uint8_t _reg);

    TwoWire *_i2cPort;
};


class SparkFun_Ambient_Light_B
{  
  public:

    SparkFun_Ambient_Light_B(uint8_t address_B); // I2C Constructor

    bool begin(TwoWire &wirePort = Wire); // begin function

    // REG0x00, bits [12:11]
    // This function sets the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    void setGain(float gainVal);

    // REG0x00, bits [12:11]
    // This function reads the gain for the Ambient Light Sensor. Possible values
    // are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
    // sensors is behind dark glass, where as the lowest setting should be used in
    // dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
    float readGain();

    // REG0x00, bits[9:6]
    // This function sets the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower sensor refresh times. 
    void setIntegTime(uint16_t time);

    // REG0x00, bits[9:6]
    // This function reads the integration time (the saturation time of light on the
    // sensor) of the ambient light sensor. Higher integration time leads to better
    // resolution but slower sensor refresh times. 
    uint16_t readIntegTime();

    // REG0x00, bit[0]
    // This function powers up the Ambient Light Sensor. The last value that was
    // read during shut down will be overwritten on the sensor's subsequent read.
    // After power up, a small 4ms delay is applied to give time for the internal
    // osciallator and signal processor to power up.   
    void powerOn();

    // REG[0x04], bits[15:0]
    // This function gets the sensor's ambient light's lux value. The lux value is
    // determined based on current gain and integration time settings. If the lux
    // value exceeds 1000 then a compensation formula is applied to it. 
    uint32_t readLight_B();


  private:

    uint8_t _address_B;
    
    // This function compensates for lux values over 1000. From datasheet:
    // "Illumination values higher than 1000 lx show non-linearity. This
    // non-linearity is the same for all sensors, so a compensation forumla..."
    // etc. etc. 
    uint32_t _luxCompensation(uint32_t _luxVal_B);

    // The lux value of the Ambient Light sensor depends on both the gain and the
    // integration time settings. This function determines which conversion value
    // to use by using the bit representation of the gain as an index to look up
    // the conversion value in the correct integration time array. It then converts 
    // the value and returns it.  
    uint32_t _calculateLux(uint16_t _lightBits_B);

    // This function writes to a 16 bit register. Paramaters include the register's address, a mask 
    // for bits that are ignored, the bits to write, and the bits' starting
    // position.
    void _writeRegister(uint8_t _wReg, uint16_t _mask, uint16_t _bits, uint8_t _startPosition);

    // This function reads a 16 bit register. It takes the register's
    // address as its' parameter.
    uint16_t _readRegister(uint8_t _reg);

    TwoWire *_i2cPort;
};


#endif
