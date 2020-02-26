/*!
 * @file ST7558.h
 *
 * @mainpage Arduino library for monochrome LCD (from the phone Motorola C115) based on ST7558 drivers.
 *
 * @section Introduction
 *
 * ST7558 supports I²C, SPI or 8-bit parallel interface(8080 and 6800) to communicate. 
 * I2C requires 2 pins (SCL and SDA) and RESET pin. SPI requires 4 pins and RESET pin 
 * (optionaly, software RESET can be work with SPI).
 * My LCD version (from the phone Motorola C115) don't have a SPI and 8-bit parallel interface outputs, only I²C.
 *
 * ST7558:
 *      - resolution: 102x66, but version from the phone Motorola C115 have 96x65
 *      - on-chip display data RAM (102x66 = 6732 bit)
 *      - pinout:
 * 
 *          1 Vcc +3.3v           ________________________
 *          2 empty             /                          \
 *          3 I²C SDA           |   ST7558 LCD from C115   |
 *          4 I²C SCL           |                          |
 *          5 A0                |          96x65           |
 *          6 GND               \ ________________________ /
 *          7 VLCD                 |  |  |  |  |  |  |  |      
 *          8 RESET                1  2  3  4  5  6  7  8
 * 
 * @section Dependencies
 *
 * no dependencies, yet
 *
 * @section Author
 *
 * Written by Yaroslav @kashapovd Kashapov just for fun, feb 2020
 * Thanks for Tapia @kr4fty Favio, https://github.com/kr4fty
 *
 * @section license License
 *
 * GNU GENERAL PUBLIC LICENSE ver. 3
 * 
 */

#ifndef ST7558_H
#define ST7558_H

#include <Arduino.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

#define ST7558_WIDTH    96  
#define ST7558_HEIGHT   65

#define ST7558_BLACK 1
#define ST7558_WHITE 0

#define BLACK ST7558_BLACK
#define WHITE ST7558_WHITE

#define I2C_MAX 32

#define ST7558_I2C_ADDRESS              0b00111100  // 0x3c
#define NOP                             0b00000000  // 0
#define CONTROL_BYTE                    0b00000000  // 0
#define ST7558_FUNCTIONSET              0b00100000  // 0x20

    #define POWER_DOWN_MODE             0b00000100  // 0x04 
    #define CHIP_ACTIVE                 0b00000000  // 0

    #define BASIC                       0b00000000  // 0x00, h=0 
    #define EXTENDED                    0b00000001  // 0x01, h=1
    
    #define VERTICAL_ADDRESSING         0b00000010  // 0x02 <- page 22, figure 11 of the datasheet
    #define HORIZONTAL_ADDRESSING       0b00000000  // 0x00 <- page 22, figure 12 of the datasheet

#define ST7558_EXTENDED_DISPAY_CONTROL  0b00101110  // 0x2e
    #define MIRROR_X                    0b00000010  // 0x02
    #define MIRROR_Y                    0b00000100  // 0x04

/* H="0", basic mode */ 
#define ST7558_VLCD                     0b00010000  // 0x10
    #define VLCD_HIGH                   0b00000001  // 0x01
    #define VLCD_LOW                    0b00000000  // 0x00
#define ST7558_DISPLAY_CONROL           0b00001000  // 0x08
    #define OFF                         0b00000000  // 0x00
    #define ON                          0b00001100  // 0x0c
    #define INVERT                      0b00001101  // 0x0b
    #define ALL_SEG_ON                  0b00001001  // 0x09
#define ST7558_YADDR                    0b01000000  // 0x40
#define ST7558_XADDR                    0b10000000  // 0x80
//#define ST7558_LOADING_CONTROL        0b00001111  // 0x0f
//#define ST7558_LOADING_CONTROL        0b00001001  // 0x09

/* H="1", ext. mode */
#define ST7558_BOOST                    0b00001000  // 0x08
    #define VDD2x2                      0b00000000  // 0x00
    #define VDD2x3                      0b00000001  // 0x01
    #define VDD2x4                      0b00000010  // 0x02
    #define VDD2x5                      0b00000011  // 0x03
#define ST7558_SYSTEM_BIAS              0b00010010  // 0x12
#define ST7558_VOP                      0b10000000  // 0x80
    #define DEFAULT_VOP                 0b01000000  // 0x40
//#define SW_INTERNAL_RGISTER_INIT1     0b00001110  // 0x0e
//#define SW_INTERNAL_RGISTER_INIT2     0b00001001  // 0x09

class ST7558
{

private:

    void 
            _i2cwrite_cmd(const uint8_t *data, uint8_t n),
            _i2cwrite_data(const uint8_t *data, uint8_t n),
            _hardreset(void),
            _setXY(uint8_t x, uint8_t y);
    uint8_t 
            _rst_pin,
            *_buffer;
public:

    ST7558 (uint8_t rst_pin = -1);
    ~ST7558(void);
    void
            begin(void),
            setContrast(byte value),
            invert(bool state),
            off(void),
            on(void),
            drawPixel(uint8_t x, uint8_t y, uint8_t color),
            clear(void),
            display(void);

    uint8_t
            *getBuffer(void),
            width(void),
            height(void);
};

#endif