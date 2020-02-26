/*!
 * @file ST7558.cpp
 *
 * @mainpage Arduino library for monochrome LCD (from the phone Motorola C115) based on ST7558 drivers.
 *
 * @section Introduction
 *
 *  ST7558 supports I²C, SPI or 8-bit parallel interface(8080 and 6800) to communicate. 
 *  I2C requires 2 pins (SCL and SDA) and RESET pin. SPI requires 4 pins and RESET pin 
 *  (optionaly, software RESET can be work with SPI).
 *  My LCD version (from the phone Motorola C115) doesn't have SPI and 8-bit parallel interface outputs, only I²C.
 *
 *  ST7558:
 *      - resolution: 102x66, but version from the phone Motorola C115 have 96x65
 *      - on-chip display data RAM (102x66 = 6732 bit)
 *      - pinout:
 * 
 *          1 Vcc +3.3v           ________________________
 *          2 empty             /                          \
 *          3 I²C SDA           |   ST7558 LCD from C115   |
 *          4 I²C SCL           |                          |
 *          5 A0                |             96x65        |
 *          6 GND               \ ________________________ /
 *          7 VLCD                 |  |  |  |  |  |  |  |      
 *          8 RESET                1  2  3  4  5  6  7  8
 * 
 * @section Dependencies
 *
 *  no dependencies, yet
 *
 * @section Author
 *
 *  Written by Yaroslav @kashapovd Kashapov just for fun, feb 2020
 *  Thanks for Tapia @kr4fty Favio, https://github.com/kr4fty
 *
 * @section License
 *  
 *  GNU GENERAL PUBLIC LICENSE ver. 3
 * 
 */

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

#define ST7558_BYTES_CAPACITY ST7558_WIDTH * (ST7558_HEIGHT + 7) / 8
#define I2C_MAX 32

// Constructor for ST7558 object
ST7558::ST7558(uint8_t rst_pin)
{
    _buffer = nullptr;
    _rst_pin = rst_pin;
}

// Destructor for ST7558 object
ST7558::~ST7558(void) 
{
    if (_buffer) 
       free(_buffer);
}

void ST7558::_i2cwrite_cmd(const uint8_t *data, uint8_t n)
{
    Wire.beginTransmission(ST7558_I2C_ADDRESS);     // start a transmition session
    Wire.write(CONTROL_BYTE);                       // <- the Co byte, see datasheet
    Wire.write(data, n);                            // data transfer 
    Wire.endTransmission();                         // close a transmition session
}

void ST7558::_i2cwrite_data(const uint8_t *data, uint8_t n)  
{
    Wire.beginTransmission(ST7558_I2C_ADDRESS);
    // the Co byte need for instruction, but not for the graphics data
    Wire.write(data, n);
    Wire.endTransmission();
}

void ST7558::_hardreset(void)
/* ST7558 support a soft reset, but it is not working with I²C */ 
{
    pinMode(_rst_pin, OUTPUT);
    digitalWrite(_rst_pin, HIGH);
    _delay_ms(1);                   // VDD goes high at start, pause for 1 ms
    digitalWrite(_rst_pin, LOW);    // Bring reset low
    _delay_ms(5);                   // Wait 5ms
    digitalWrite(_rst_pin, HIGH);   // Bring out of reset
}

void ST7558::_setXY(uint8_t x, uint8_t y)
{
    uint8_t cmd_setxy[] PROGMEM = 
    {
        //CONTROL_BYTE
        ST7558_FUNCTIONSET|BASIC,           // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_XADDR|x,
        ST7558_YADDR|y
    };
    this -> _i2cwrite_cmd(cmd_setxy, sizeof(cmd_setxy));
}

void ST7558::begin(void) 
{
    Wire.begin();

    _buffer = (uint8_t *)malloc(ST7558_BYTES_CAPACITY);

    this -> clear();
    this -> _hardreset();

    uint8_t cmd_init[] PROGMEM = 
    {
        //CONTROL_BYTE
        ST7558_EXTENDED_DISPAY_CONTROL,         // Ext. display control | MX = 1, MY = 1
        ST7558_FUNCTIONSET|EXTENDED,            // Function set PD = 0, V = 0, H = 1 (extended instruction set)
        ST7558_SYSTEM_BIAS,
        ST7558_VOP|DEFAULT_VOP,
        ST7558_BOOST|VDD2x5,
        ST7558_FUNCTIONSET|BASIC,               // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        //ST7558_EXTENDED_DISPAY_CONTROL,       // Ext. display control | MX = 1, MY = 0 
        ST7558_VLCD|VLCD_HIGH,                  // PRS
        ST7558_DISPLAY_CONROL|ON,               // Display control D = 1, E = 0 (Normal mode)
        //NOP,
        ST7558_YADDR,                         
        ST7558_XADDR                          
    };

    this -> _i2cwrite_cmd(cmd_init, sizeof(cmd_init)); 
}

void ST7558::invert(bool state) 
{
    uint8_t cmd_invert[] PROGMEM = 
    {
        //CONTROL_BYTE,
        ST7558_FUNCTIONSET|BASIC,                             // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_DISPLAY_CONROL| (state) ? INVERT : ON,         // Display control D = 1, E = 1 (Invert video mode)
    };

    this -> _i2cwrite_cmd(cmd_invert, sizeof(cmd_invert));
}

void ST7558::off(void) 
{
    uint8_t cmd_off[] PROGMEM = 
    {
        //CONTROL_BYTE,
        ST7558_FUNCTIONSET|BASIC,                       // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_DISPLAY_CONROL|OFF,                      // Display control D = 0, E = 0 (Display off)
    };
    this -> _i2cwrite_cmd(cmd_off, sizeof(cmd_off));
}

void ST7558::on(void) 
{
    uint8_t cmd_on[] PROGMEM = 
    {
        //CONTROL_BYTE,
        ST7558_FUNCTIONSET|BASIC,                       // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_DISPLAY_CONROL|ON,                       // Display control D = 1, E = 0 (Normal mode)
    };

    this -> _i2cwrite_cmd(cmd_on, sizeof(cmd_on));
}

void ST7558::setContrast(byte value) 
{
    uint8_t cmd_set_contrast[] PROGMEM = 
    {
        //CONTROL_BYTE
        ST7558_FUNCTIONSET|EXTENDED,
        ST7558_VOP|(value & 0b01111111)             // 127 is max contrast level
    };
    
    this -> _i2cwrite_cmd(cmd_set_contrast, sizeof(cmd_set_contrast));
}

void ST7558::clear(void) 
{
    memset(_buffer, 0, ST7558_BYTES_CAPACITY);      // set the whole bytes in the framebuffer to zero
}

void ST7558::drawPixel(uint8_t x, uint8_t y, uint8_t color) 
{
    if ((x >= 0 && x < ST7558_WIDTH) && (y >= 0 && y < ST7558_HEIGHT)) 
    {   
        color ? 
        _buffer[x + (y/8) * ST7558_WIDTH] |= (1 << y%8) 
        :
        _buffer[x + (y/8) * ST7558_WIDTH] &= ~(1 << y%8);
    }
}

void ST7558::display(void)
{   
    this -> _setXY(0,0);

    uint8_t maxPage = ceil(ST7558_HEIGHT / 8.0),
            column, 
            page,
            buff[25];   // 25 because 96(width) can be divide evenly by 24. 24(data) + 1(0x40, see down)

    for (page = 0; page < maxPage; page++) 
    {
        buff[0] = uint8_t(0x40);
        column = 0;
        this -> _setXY(column,page);

        while (column < ST7558_WIDTH)
        {
            for (size_t i = 1; i < sizeof(buff); i++, column++) 
            {
                buff[i] = _buffer[ST7558_WIDTH * page + column];
            }
            this -> _i2cwrite_data(buff, sizeof(buff));
        }
    }
    this -> _setXY(0,0);
}

uint8_t *ST7558::getBuffer(void)  { return _buffer; }

uint16_t ST7558::getBufferSize(void) { return ST7558_BYTES_CAPACITY; }

uint8_t ST7558::width() { return ST7558_WIDTH; }

uint8_t ST7558::height() { return ST7558_HEIGHT; }
