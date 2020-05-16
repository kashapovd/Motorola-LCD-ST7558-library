/**
 * @file ST7558.h
 *
 * @mainpage Arduino library for monochrome LCD (from the phone Motorola C115) based on ST7558 driver.
 *
 * @section Introduction
 *
 *  ST7558 supports I²C, SPI or 8-bit parallel interface(8080 and 6800) to communicate. 
 *  I2C requires 2 pins (SCL and SDA) and RESET pin. SPI requires 4 pins and RESET pin.
 *  My LCD version (from the phone Motorola C115) doesn't have SPI and 8-bit parallel interface outputs, only I²C.
 *
 *  ST7558:
 *      - resolution: 102x66, but version from the phone Motorola C115 has 96x65 visible  pixels
 *      - on-chip display data RAM (102x66 = 6732 bit)
 *      - pinout:
 *                               ________________________
 *          1 Vcc +3.3v         / ______________________ \ 
 *          2 none             | |                      | |
 *          3 I²C SDA          | | ST7558 LCD from C115 | |
 *          4 I²C SCL          | |                      | |
 *          5 A0               | |        96x65         | |
 *          6 GND              | |______________________| |
 *          7 VLCD              \________________________/
 *          8 RESET               |  |  |  |  |  |  |  |
 *          .......               1  2  3  4  5  6  7  8
 * 
 * @section Dependencies
 *
 *  ADAFRUIT GRAPHIC LIB & arduino libraries
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

#ifndef ST7558_H
#define ST7558_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <Adafruit_GFX.h>

#define ST7558_WIDTH    96  
#define ST7558_HEIGHT   65

#define BLACK 1
#define WHITE 0

#define I2C_MAX 32

#define ST7558_CMD                      0x00
#define ST7558_DATA                     0x40

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

/* H="1", ext. mode */
#define ST7558_BOOST                    0b00001000  // 0x08 <- see datasheet
    #define VDD2x2                      0b00000000  // 0x00 <- see datasheet
    #define VDD2x3                      0b00000001  // 0x01 <- see datasheet
    #define VDD2x4                      0b00000010  // 0x02 <- see datasheet
    #define VDD2x5                      0b00000011  // 0x03 <- see datasheet

#define ST7558_SYSTEM_BIAS              0b00010010  // 0x12 <- see datasheet
#define ST7558_VOP                      0b10000000  // 0x80 <- see datasheet
    #define DEFAULT_VOP                 0b01000000  // 0x40 <- see datasheet


class ST7558 : public Adafruit_GFX {
            
    private:

        void _i2cwrite(const uint8_t type, const uint8_t *data, uint8_t n);
        void _hardreset(void);
        void _setXY (const uint8_t x, const uint8_t y);
        uint8_t _rst_pin;
        uint32_t clock;

    public:

        ST7558(uint8_t rst_pin);
        ST7558(uint8_t rst_pin, uint32_t clock);
        void begin(void);
        void displayOff(void);
        void displayOn(void);
        void setContrast(const uint8_t value);
        void invertDisplay(const bool state);
        void clearDisplay(void);
        void display(void);
        uint8_t *getBuffer(void);
        uint16_t getBufferSize(void);
        uint8_t getPixel(const uint8_t x, const uint8_t y);

        void drawPixel(int16_t x, int16_t y, 
                        uint16_t color);
        
        void drawRect(int16_t x, int16_t y, int16_t w, 
                      int16_t h, uint16_t color);

        void fillRect(int16_t x, int16_t y, int16_t w, 
                      int16_t h, uint16_t color);

        void drawSquare(int16_t x, int16_t y, int16_t a, 
                        uint16_t color);    

        void fillSquare(int16_t x, int16_t y, int16_t a, 
                        uint16_t color);   

        void fillScreen(int16_t color);
         
        void pushBuffer(uint8_t *buffer, 
                        const uint16_t size);


        // old code
        // void drawLine(int16_t x1, int16_t y1, int16_t x2, 
        //               int16_t y2, int16_t color);

        // void drawLineDDA(int16_t x1, int16_t y1, int16_t x2, 
        //                  int16_t y2, int16_t color);

        // void drawTriangle(int16_t x1, int16_t y1, int16_t x2, 
        //                   int16_t y2, int16_t x3, int16_t y3, 
        //                   int16_t color);

        // void setCursor(int16_t x, int16_t y);
        // void setTextColor(int16_t color);

        // void drawChar(int16_t x, int16_t y, const char c, int16_t color);

        // using Print::write;
        // #if ARDUINO >= 100
        //     virtual size_t write(int16_t c);
        // #else
        //     virtual void write(int16_t c);
        // #endif
};
#endif
