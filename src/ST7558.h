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

    #define BLACK 1
    #define WHITE 0

    #define I2C_ADDRESS                 0b00111100
    #define NOP                         0b00000000
    #define CMD_RST                     0b00000000
    #define FUNCTION_SET                0b00100000

        /*  POWER_DOWN_MODE: All LCD outputs at VSS (display off), bias generator and VLCD generator off, VLCD can be
        disconnected, oscillator off (external clock possible), RAM contents not cleared; RAM data
        can be written */
        #define POWER_DOWN_MODE         0b00100100
        #define CHIP_ACTIVE             0b00100000
    
        /*  When H = 0(BASIC) the commands "display control" , "set Y address" and "set X address"
        can be performed, when H = 1(EXTENDED) the others can be executed. The commands "write data"
        and ‘function set’ can be executed in both cases. For more you should see the datasheet */
        #define BASIC                   0b00000000
        #define EXTENDED                0b00000001
        
        #define VERTICAL_ADDRESSING     0b00000010 // <- page 22, figure 11 of the datasheet
        #define HORIZONTAL_ADDRESSING   0b00000000 // <- page 22, figure 12 of the datasheet

    #define EXTENDED_DISPAY_CONTROL     0b00101110
        #define MIRROR_X                0b00000010
        #define MIRROR_Y                0b00000100

    /* H="0", basic mode */ 
    #define VLCD_RANGE                  0b00010000
        #define VLCD_HIGH               0b00000001
        #define VLCD_LOW                0b00000000
    #define DISPLAY_CONROL              0b00001000
        #define OFF                     0b00000000
        #define ON                      0b00001100
        #define INVERT                  0b00001101
        #define ALL_SEG_ON              0b00001001
    #define SET_YADDR                   0b01000000
    #define SET_XADDR                   0b10000000
    //#define LOADING_CONTROL             0b00001111  
    //#define LOADING_CONTROL             0b00001001  

    /* H="1", ext. mode */
    #define SET_BOOST                   0b00001000
        #define VDD2x2                  0b00000000
        #define VDD2x3                  0b00000001
        #define VDD2x4                  0b00000010
        #define VDD2x5                  0b00000011
    #define SYSTEM_BIAS                 0b00010010
    #define SET_VOP                     0b10000000
        #define DEFAULT_VOP             0b01000000
    //#define SW_INTERNAL_RGISTER_INIT1 0b00001110
    //#define SW_INTERNAL_RGISTER_INIT2 0b00001001

    class ST7558
    {

    private:

        void 
            _i2cwrite_cmd1(const uint8_t c),
            _i2cwrite_cmdList(const uint8_t *data, uint8_t n),
            _hardreset(void),
            _setXY(uint8_t x, uint8_t y);
        uint8_t 
            _rst_pin,
            *_buffer;

    public:

        ST7558 (uint8_t rst_pin = -1);
        ~ST7558(void);
        void
            begin(),
            setContrast(byte value),
            invert(bool state),
            off(void),
            on(void),
            writechar(uint8_t x, uint8_t y, char symbol),
            drawPixel(uint8_t x, uint8_t y, bool color),
            clear(void),
            rotate(bool state),
            display(void);

        uint8_t
            *getBuffer(void);
    };

#endif