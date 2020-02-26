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
 * My LCD version (from the phone Motorola C115) doesn't have SPI and 8-bit parallel interface outputs, only I²C.
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
 * @section License
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

#define ST7558_I2C_ADDRESS              0b00111100  // 0x3c <- see datasheet
#define NOP                             0b00000000  // 0x00 <- see datasheet
#define CONTROL_BYTE                    0b00000000  // 0x00 <- see datasheet
#define ST7558_FUNCTIONSET              0b00100000  // 0x20 <- see datasheet

    #define POWER_DOWN_MODE             0b00000100  // 0x04 <- see datasheet
    #define CHIP_ACTIVE                 0b00000000  // 0x00 <- see datasheet

    #define BASIC                       0b00000000  // 0x00 <- see datasheet
    #define EXTENDED                    0b00000001  // 0x01 <- see datashee
    
    #define VERTICAL_ADDRESSING         0b00000010  // 0x02 <- see datasheet
    #define HORIZONTAL_ADDRESSING       0b00000000  // 0x00 <- see datasheet

#define ST7558_EXTENDED_DISPAY_CONTROL  0b00101110  // 0x2e <- see datasheet

    #define MIRROR_X                    0b00000010  // 0x02 <- see datasheet
    #define MIRROR_Y                    0b00000100  // 0x04 <- see datasheet

/* H="0", basic mode */ 
#define ST7558_VLCD                     0b00010000  // 0x10 <- see datasheet

    #define VLCD_HIGH                   0b00000001  // 0x01 <- see datasheet
    #define VLCD_LOW                    0b00000000  // 0x00 <- see datasheet

#define ST7558_DISPLAY_CONROL           0b00001000  // 0x08 <- see datasheet

    #define OFF                         0b00000000  // 0x00 <- see datasheet
    #define ON                          0b00001100  // 0x0c <- see datasheet
    #define INVERT                      0b00001101  // 0x0b <- see datasheet
    #define ALL_SEG_ON                  0b00001001  // 0x09 <- see datasheet

#define ST7558_YADDR                    0b01000000  // 0x40 <- see datasheet
#define ST7558_XADDR                    0b10000000  // 0x80 <- see datasheet
//#define ST7558_LOADING_CONTROL        0b00001111  // 0x0f <- see datasheet
//#define ST7558_LOADING_CONTROL        0b00001001  // 0x09 <- see datasheet

/* H="1", ext. mode */
#define ST7558_BOOST                    0b00001000  // 0x08 <- see datasheet
    #define VDD2x2                      0b00000000  // 0x00 <- see datasheet
    #define VDD2x3                      0b00000001  // 0x01 <- see datasheet
    #define VDD2x4                      0b00000010  // 0x02 <- see datasheet
    #define VDD2x5                      0b00000011  // 0x03 <- see datasheet
#define ST7558_SYSTEM_BIAS              0b00010010  // 0x12 <- see datasheet
#define ST7558_VOP                      0b10000000  // 0x80 <- see datasheet
    #define DEFAULT_VOP                 0b01000000  // 0x40 <- see datasheet
//#define SW_INTERNAL_RGISTER_INIT1     0b00001110  // 0x0e <- see datasheet
//#define SW_INTERNAL_RGISTER_INIT2     0b00001001  // 0x09 <- see datasheet

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

        ST7558 (uint8_t rst_pin);
        ~ST7558(void);
        
        /**************************************************************/
        /*!
            @brief This method makes initial display setup
        */
        /**************************************************************/
        void begin(void);
        
        
        /**************************************************************/
        /*!
            @brief Just display off, not power down
        */
        /**************************************************************/
        void off(void);


        /**************************************************************/
        /*!
            @brief Display all RAM bits. Normal mode
        */
        /**************************************************************/
        void on(void);


        /**************************************************************/
        /*!
            @brief This method sets all framebuffer bits to zero
        */
        /**************************************************************/
        void clear(void);

        
        /**************************************************************/
        /*!
            @brief This method writes all framebuffer to the ST7558 RAM 
        */
        /**************************************************************/
        void display(void);

        /**************************************************************/
        /*!
            @brief Get memory pointer to the framebuffer
            @return Pointer to the first framebuffer's element 
        */
        /**************************************************************/
        uint8_t *getBuffer(void);


        /**************************************************************/
        /*!
            @brief Get size of the framebuffer in bytes
            @return Size in bytes
        */
        /**************************************************************/
        uint16_t getBufferSize(void);


        /**************************************************************/
        /*!
            @brief Get width of the display
            @return Width in pixels
        */
        /**************************************************************/
        uint8_t width(void);

        
        /***************************************************************/
        /*!
            @brief Get height of the display
            @return Height in pixels
        */
        /***************************************************************/
        uint8_t height(void);

        
        /***************************************************************/
        /*!  
            @brief  Specified contrast level. It drives a voltage 
            operating by software. See 32 page of the datasheet
            @param  value   contrast level [0...127]. I recommended 70.
        */
        /****************************************************************/
        void setContrast(uint8_t value);
        
        
        /****************************************************************/
        /*!  
            @brief  Activate or deactivate the inverse video mode. 
            If true, your pixel will be black, if bit in RAM is zero. 
            And vice versa
            @param  state   true - mode is on, false - off
        */
        /****************************************************************/
        void invert(bool state);

        
        /****************************************************************/
        /*!  
            @brief  Draw one pixel to the framebuffer
            @param  x   x coordinate
            @param  y   y coordinate
            @param  color 
        */
        /****************************************************************/
        void drawPixel(uint8_t x, uint8_t y, uint8_t color);

};
#endif
