// created 22 feb 2020

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#ifdef __AVR__
    #include <util/delay.h>
    #include <avr/pgmspace.h>
#endif

#include <Wire.h>
#include "ST7558.h"


#define ST7558_WIDTH    96  
#define ST7558_HEIGHT   65

// Constructor fot ST7558 object

ST7558::ST7558(uint8_t rst_pin) 
{
    _buffer = nullptr;
    _rst_pin = rst_pin;
}

// Destructor for ST7558 object

ST7558::~ST7558(void) 
{
    if (_buffer) 
    {
        delete [] _buffer;
        _buffer = nullptr;
    }
}

void ST7558::_i2cwrite_cmdList(const uint8_t *data, uint8_t n)
{
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.write(data, n);
    Wire.endTransmission();
}

void ST7558::_i2cwrite_cmd1(const uint8_t cmd) {
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write((uint8_t)0x00);
    Wire.write(cmd);
    Wire.endTransmission();
}

void ST7558::_hardreset(void) {
    pinMode(_rst_pin, OUTPUT);
    digitalWrite(_rst_pin, HIGH);
    _delay_ms(1);                   // VDD goes high at start, pause for 1 ms
    digitalWrite(_rst_pin, LOW);    // Bring reset low
    _delay_ms(5);                   // Wait 10ms
    digitalWrite(_rst_pin, HIGH);   // Bring out of reset
}

void ST7558::_setXY(uint8_t x, uint8_t y)
{
    uint8_t cmd_setxy[] PROGMEM = 
    {
        //CMD_RST,
        FUNCTION_SET|BASIC,
        SET_XADDR|(x & 0b00001111),
        SET_YADDR|(y & 0b01111111)
    };
    this->_i2cwrite_cmdList(cmd_setxy, sizeof(cmd_setxy));
}

void ST7558::begin(void) 
{
    Wire.begin();
    _buffer = (uint8_t *)malloc(ST7558_WIDTH * (ST7558_HEIGHT + 3) / 8);
    this->clear();
    this->_hardreset();

    uint8_t cmd_init[] PROGMEM = 
    {
        //CMD_RST,
        EXTENDED_DISPAY_CONTROL,        // Ext. display control | MX = 1, MY = 1
        FUNCTION_SET|EXTENDED,          // Function set PD = 0, V = 0, H = 1 (extended instruction set)
        //0b00001110,                   // S/W initial Internal register(first byte)
        //0b00010010,                   // S/W initial Internal register(second byte)
        SYSTEM_BIAS,
        SET_VOP|DEFAULT_VOP,
        SET_BOOST|VDD2x5,
        FUNCTION_SET|BASIC,             // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        //0b00101100,                   // Ext. display control | MX = 1, MY = 0 
        VLCD_RANGE|VLCD_HIGH,           // PRS
        DISPLAY_CONROL|ON,              // Display control D = 1, E = 0 (Normal mode)
        //0b0,                          // nop
        SET_YADDR,                         
        SET_XADDR                          
    };

    this->_i2cwrite_cmdList(cmd_init, sizeof(cmd_init)); 
}

void ST7558::invert(bool state) 
{
    uint8_t  cmd_invert[] PROGMEM = 
    {
        //CMD_RST,
        FUNCTION_SET|BASIC,                             // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        DISPLAY_CONROL| (state) ? INVERT : ON,          // Display control D = 1, E = 1 (Invert video mode)
    };

    this->_i2cwrite_cmdList(cmd_invert, sizeof(cmd_invert));
}

void ST7558::off(void) 
{
    uint8_t  cmd_off[] PROGMEM = 
    {
        //CMD_RST,
        FUNCTION_SET|BASIC,            // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        DISPLAY_CONROL|OFF,            // Display control D = 0, E = 0 (Display off)
    };
    this->_i2cwrite_cmdList(cmd_off, sizeof(cmd_off));
}

void ST7558::on(void) 
{
    uint8_t  cmd_on[] PROGMEM = 
    {
        //CMD_RST,
        FUNCTION_SET|BASIC,             // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        DISPLAY_CONROL|ON,              // Display control D = 1, E = 0 (Normal mode)
    };

    this->_i2cwrite_cmdList(cmd_on, sizeof(cmd_on));
}

void ST7558::rotate(bool state)
{

}

void ST7558::setContrast(byte value) 
{
    uint8_t  cmd_set_contrast[] PROGMEM = 
    {
        //CMD_RST,
        FUNCTION_SET|EXTENDED,
        SET_VOP|(value & 0b01111111)    // value always be less than 128
    };

    this->_i2cwrite_cmdList(cmd_set_contrast, sizeof(cmd_set_contrast));
}

void ST7558::clear(void) {
    uint16_t len = (ST7558_WIDTH * (ST7558_HEIGHT + 3) / 8);
    memset(_buffer, 1, len);
}

uint8_t *ST7558::getBuffer(void) {
    return _buffer;
}

void ST7558::display(void) {
    //_setXY(0,0);
    _i2cwrite_cmd1((uint8_t)0x40);
    //Wire.beginTransmission(I2C_ADDRESS);
    //Wire.write(_buffer, sizeof(_buffer));
    //Wire.endTransmission();
    _i2cwrite_cmd1((uint8_t)0xff);
    //_i2cwrite_cmdList(_buffer, sizeof(_buffer));
}