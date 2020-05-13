/**
 * @file ST7558.cpp
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
 *      - resolution: 102x66, but version from the phone Motorola C115 has 96x65 visible pixels
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

#include "ST7558.h"
#include <Wire.h>
#include <SPI.h>  // just for adafruit gfx lib, don't pay attention
#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#endif

#define ST7558_BYTES_CAPACITY   ST7558_WIDTH * (ST7558_HEIGHT + 7) / 8
#define COLUMNS                 ST7558_WIDTH
#define PAGES                   ST7558_BYTES_CAPACITY / COLUMNS

#define WIRE_BEGIN              Wire.begin() 
#define WIRE_START              Wire.beginTransmission(ST7558_I2C_ADDRESS)
#define WIRE_END                Wire.endTransmission()
#define WIRE_WRITE(data)        Wire.write(data)  


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//                  CONSTRUCTOR & DESTRUCTOR                    //   
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/**************************************************************/
/** @brief Common constructor
*/
/**************************************************************/
ST7558::ST7558(uint8_t rst_pin) : Adafruit_GFX (ST7558_WIDTH, ST7558_HEIGHT) {
    
    _buffer = nullptr;
    _rst_pin = rst_pin;
    clock = 100000;     // standart frequency for most arduinos.
                        // estimated maximum is 9 fps :( 
}

/**************************************************************/
/** @brief Constructor with i2c clock frequency control
*/
/**************************************************************/
ST7558::ST7558(uint8_t rst_pin, uint32_t clock) : Adafruit_GFX (ST7558_WIDTH, ST7558_HEIGHT) {
    
    _buffer = nullptr;
    _rst_pin = rst_pin;
    if (clock > 300000) {
        clock = 300000;     // on 300kHz estimated maximum is 23-22 fps
    }
    if (clock < 100000) {
        clock = 100000;
    }
    this->clock = clock;
}

ST7558::~ST7558(void) {
    
    if (_buffer) 
       free(_buffer);
    _buffer = nullptr;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//                          LOW-LEVEL UTILS                     //   
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/**************************************************************/
/** @brief This method writes instructions. 
           For one I²C transmission session can be transmitted 
           32 bytes. 
*/
/**************************************************************/
void ST7558::_i2cwrite(const uint8_t type, const uint8_t *data, uint8_t n) {
    
    uint8_t bytesOut;
    WIRE_START;
    WIRE_WRITE(type);                       // <- Co byte, see datasheet
    bytesOut = 1;
    while (n--) {

        if (bytesOut >= I2C_MAX) {

            WIRE_END;
            WIRE_START;
            WIRE_WRITE(type);
            bytesOut = 1; 
        }
        WIRE_WRITE(*data++);
        bytesOut++;
    } 
    WIRE_END;
}

/**************************************************************/
/** @brief This method makes hardware reset. ST7558 support a 
           software reset, but it doesn't work with I²C :(
*/
/**************************************************************/
inline void ST7558::_hardreset(void) {
    
    pinMode(_rst_pin, OUTPUT);
    digitalWrite(_rst_pin, LOW);            // Bring reset low
    delay(5);                           // Wait 5ms
    digitalWrite(_rst_pin, HIGH);           // Bring out of reset
    delay(5);                           // Wait 5ms
}

/**************************************************************/
/** @brief This method sets x[0...101](columns) and 
           y[0...9](pages) address of RAM.
*/
/**************************************************************/
void ST7558::_setXY(const uint8_t x, const uint8_t y) {
    
    uint8_t cmd_setxy[] = {

        //CONTROL_BYTE,
        ST7558_FUNCTIONSET | BASIC,         // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_XADDR + x,
        ST7558_YADDR + y
    };
    _i2cwrite(ST7558_CMD, cmd_setxy, sizeof(cmd_setxy));
}

/**************************************************************/
/** @brief This method makes initial display setup
*/
/**************************************************************/
void ST7558::begin(void) {
    
    WIRE_BEGIN;
    _buffer = (uint8_t *)malloc(ST7558_BYTES_CAPACITY);
    Wire.setClock(clock);
    clearDisplay();
    _hardreset();

    // main lcd initialization 
    uint8_t cmd_init[] = {

        //CONTROL_BYTE,
        ST7558_EXTENDED_DISPAY_CONTROL,         // Ext. display control | MX = 1, MY = 1
        ST7558_FUNCTIONSET | EXTENDED,          // Function set PD = 0, V = 0, H = 1 (extended instruction set)
        ST7558_SYSTEM_BIAS,
        ST7558_VOP | DEFAULT_VOP,
        ST7558_BOOST | VDD2x5,
        ST7558_FUNCTIONSET | BASIC,             // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        //ST7558_EXTENDED_DISPAY_CONTROL,       // Ext. display control | MX = 1, MY = 0 
        ST7558_VLCD | VLCD_HIGH,                // PRS
        ST7558_DISPLAY_CONROL | ON,             // Display control D = 1, E = 0 (Normal mode)
        //NOP,
        ST7558_YADDR,                         
        ST7558_XADDR                          
    };
    _i2cwrite(ST7558_CMD, cmd_init, sizeof(cmd_init)); 
}

/****************************************************************/
/** @brief  Activate or deactivate the inverse video mode. 
            If true, your pixel will be black, if bit in 
            RAM is zero. And vice versa
    @param  state   true - mode is on, false - off
*/
/****************************************************************/
void ST7558::invertDisplay(const bool state) {
    
    uint8_t cmd_invert[] = {

        //CONTROL_BYTE,
        ST7558_FUNCTIONSET | BASIC,                         // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_DISPLAY_CONROL | ON | state,      // Display control D = 1, E = 1 (Invert video mode)
    };
    _i2cwrite(ST7558_CMD, cmd_invert, sizeof(cmd_invert));
}

/**************************************************************/
/** @brief Just display off, not power down
*/
/**************************************************************/
void ST7558::displayOff(void) {
    
    uint8_t cmd_off[] = {

        //CONTROL_BYTE,
        ST7558_FUNCTIONSET | BASIC,                         // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_DISPLAY_CONROL | OFF,                        // Display control D = 0, E = 0 (Display off)
    };
    _i2cwrite(ST7558_CMD, cmd_off, sizeof(cmd_off));
}

/**************************************************************/
/** @brief This method displays all RAM bits. Normal mode
*/
/**************************************************************/
void ST7558::displayOn(void) {
    
    uint8_t cmd_on[] = {

        //CONTROL_BYTE,
        ST7558_FUNCTIONSET | BASIC,                         // Function set PD = 0, V = 0, H = 0 (basic instruction set)
        ST7558_DISPLAY_CONROL | ON,                         // Display control D = 1, E = 0 (Normal mode)
    };
    _i2cwrite(ST7558_CMD, cmd_on, sizeof(cmd_on));
}

/***************************************************************/
/** @brief  This method sets contrast level. It drives a voltage 
            operating by software. See 32 page of the datasheet
    @param  value   contrast level [0...127]. I recommended 70.
*/
/****************************************************************/
void ST7558::setContrast(const uint8_t value) {
    
    uint8_t cmd_set_contrast[] = {

        //CONTROL_BYTE
        ST7558_FUNCTIONSET | EXTENDED,
        ST7558_VOP + ( value & 0b01111111)
    };
    
    _i2cwrite(ST7558_CMD, cmd_set_contrast, sizeof(cmd_set_contrast));
}

/**************************************************************/
/** @brief  This method sets all framebuffer bits to zero
*/
/**************************************************************/
void ST7558::clearDisplay(void) { 
    memset(_buffer, 0x00, ST7558_BYTES_CAPACITY); 
}

/**************************************************************/
/** @brief This method writes all framebuffer to the ST7558 RAM 
*/
/**************************************************************/
void ST7558::display(void) {   
    
    _setXY(0,0);

    uint8_t column, 
            page,
            buff[96];

    for (page = 0; page < PAGES; page++) {
        column = 0;
        _setXY(column,page);

        while (column < COLUMNS) {

            buff[column] = _buffer[ST7558_WIDTH * page + column]; 
            column++;
        }
        _i2cwrite(ST7558_DATA, buff, sizeof(buff));
    }
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//                     FEEDBACK FUNCTIONS                       //   
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

uint8_t ST7558::getPixel(const uint8_t x, const uint8_t y) {

    if ((x >= 0 && x < ST7558_WIDTH) 
    && (y >= 0 && y < ST7558_HEIGHT)) {
        return ((1>>y%8) & _buffer[x + (y/8) * ST7558_WIDTH]);
    }  
}

/**************************************************************/
/** @brief Get memory pointer to the framebuffer
    @return Pointer to the first framebuffer's element 
*/
/**************************************************************/
uint8_t *ST7558::getBuffer(void) { 
    return _buffer; 
}

/**************************************************************/
/** @brief Get size of the framebuffer in bytes
    @return Size in bytes
*/
/**************************************************************/
uint16_t ST7558::getBufferSize(void) {
    return ST7558_BYTES_CAPACITY; 
}

/***************************************************************/
/** @brief Push another buffer
*/
/***************************************************************/
void ST7558::pushBuffer(uint8_t *buffer, const uint16_t size) { 
    memmove(_buffer, buffer, size); 
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//                      DRAWING FUNCTIONS                       //   
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/****************************************************************/
/** @brief  Draw one pixel to the framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  color 
*/
/****************************************************************/
void ST7558::drawPixel (int16_t x, int16_t y, 
                     uint16_t color) {

    if ((x >= 0 && x < ST7558_WIDTH) 
    && (y >= 0 && y < ST7558_HEIGHT)) {

        color ?
        _buffer[x + (y/8) * ST7558_WIDTH] |= (1 << y%8)
        :
        _buffer[x + (y/8) * ST7558_WIDTH] &= ~(1 << y%8);
    }
}

/****************************************************************/
/** @brief  Draw rectangle to the framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  w   rectangle width
    @param  h   rectangle height
    @param  color 
*/
/****************************************************************/
void ST7558::drawRect(int16_t x, int16_t y, 
                      int16_t w, int16_t h, 
                      uint16_t color) {
    
    for (int i = x; i < w+x; i++) {

        drawPixel(i, y, color);
        drawPixel(i, y+h-1, color);
    }
    for (int i = y; i < h+y; i++) {

        drawPixel(x, i, color);
        drawPixel(x+w-1, i, color);
    }
}

/****************************************************************/
/** @brief  Draw a filled rectangle to the framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  w   rectangle width
    @param  h   rectangle height
    @param  color 
*/
/****************************************************************/
void ST7558::fillRect(int16_t x, int16_t y, 
                      int16_t w, int16_t h, 
                      uint16_t color) {

    for (int i = y; i < h+y; i++) {

        for (int j = x; j < w+x; j++) {

            drawPixel(j, i, color);
        }
    }
}

/****************************************************************/
/** @brief  Draw a square to the framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  a   square width
    @param  color 
*/
/****************************************************************/
void ST7558::drawSquare(int16_t x, int16_t y, 
                      int16_t a, uint16_t color) {
    drawRect(x, y, a, a, color); 
}

/****************************************************************/
/** @brief  Draw a filled square to the framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  a   square width
    @param  color 
*/
/****************************************************************/
void ST7558::fillSquare(int16_t x, int16_t y, 
                      int16_t a, uint16_t color) {
     fillRect(x, y, a, a, color); 
}

/***************************************************************/
/** @brief Fill all framebuffer 
*/
/***************************************************************/
void ST7558::fillScreen(int16_t color) {
     memset(_buffer, color ? 0xFF : 0x00, ST7558_BYTES_CAPACITY); 
}


// old code
// /****************************************************************/
// /** @brief  Draw a line. Bresenham's algorithm
//     @param  x1  x coordinate of start point
//     @param  y1  y coordinate of start point
//     @param  x2  x coordinate of end point
//     @param  y2  y coordinate of end point
//     @param  color 
// */
// /****************************************************************/
// void ST7558::drawLine(const uint8_t x1, const uint8_t y1, 
//                       const uint8_t x2, const uint8_t y2, 
//                       const uint8_t color) {

//     const uint8_t dx = abs(x2 - x1);
//     const uint8_t dy = abs(y2 - y1);
//     const uint8_t sx = x1 < x2 ? 1 : -1;
//     const uint8_t sy = y1 < y2 ? 1 : -1;
//     int error = dx - dy;

//     uint8_t x = x1;
//     uint8_t y = y1;

//     drawPixel(x2, y2, color);
//     while(x != x2 || y != y2) {

//         const int16_t error2 = error * 2;
//         drawPixel(x, y, color);
//         if(error2 > -dy) {

//             error -= dy;
//             x += sx;
//         }
//         if(error2 < dx) {

//             error += dx;
//             y += sy;
//         }
//     }
// }

// /****************************************************************/
// /** @brief  Draw a line. DDA algorithm. 
//     @param  x1  x coordinate of start point
//     @param  y1  y coordinate of start point
//     @param  x2  x coordinate of end point
//     @param  y2  y coordinate of end point
//     @param  color 
// */
// /****************************************************************/
// void ST7558::drawLineDDA(const uint8_t x1, const uint8_t y1, 
//                          const uint8_t x2, const uint8_t y2, 
//                          const uint8_t color) {
    
//     if (x1 > x2) {

//         _swap(x1, x2);
//         _swap(y1,y2);
//     }
//     const uint8_t dx = x2 - x1;
//     const uint8_t dy = abs(y2 - y1);

//     uint8_t length = max(dy, dx);

//     const float dsx = float(x2 - x1) / float(length);
//     const float dsy = float(y2 - y1) / float(length);

//     float x = x1;
//     float y = y1;

//     while (length--) {

//         x += dsx;
//         y += dsy;
//         drawPixel(round(x), round(y), color);
//     }
// }

// /****************************************************************/
// /** @brief  Draw a triangle
//     @param  x1  x coordinate of first vertex
//     @param  y1  y coordinate of first vertex
//     @param  x2  x coordinate of second vertex
//     @param  y2  y coordinate of second vertex
//     @param  x3  x coordinate of third vertex
//     @param  y3  y coordinate of third vertex
//     @param  color 
// */
// /****************************************************************/
// void ST7558::drawTriangle(const uint8_t x1, const uint8_t y1, 
//                           const uint8_t x2, const uint8_t y2, 
//                           const uint8_t x3, const uint8_t y3, 
//                           const uint8_t color) {

//     drawLine(x1,y1,x2,y2,color);
//     drawLine(x2,y2,x3,y3,color);
//     drawLine(x3,y3,x1,y1,color);
// }

// /****************************************************************/
// /** @brief  Set custom text color(ha-ha)
//     @param  color   color. Black or white for this LCD
// /****************************************************************/
// void ST7558::setTextColor(const uint8_t color) {
//     textcolor = color;
// }

// /****************************************************************/
// /** @brief  Set custom cursor position
//     @param  x   x coordinate position
//     @param  y   y coordinate position
// */
// /****************************************************************/
// void ST7558::setCursor(const uint8_t x, const uint8_t y) {
    
//     cursor_x = x;
//     cursor_y = y;    
// }

// /****************************************************************/
// /** @brief  Draw a single char
//     @param  x1  x coordinate of first vertex
//     @param  y1  y coordinate of first vertex
//     @param  c   char what will drawed
//     @param  color 
// */
// /****************************************************************/
// void ST7558::drawChar(const uint8_t x, const uint8_t y, const char c, const uint8_t color) {

//     for (uint8_t i = 0; i < CHAR_WIDTH; i++) {

//         uint8_t ch_column = pgm_read_byte(&font[(c - 32)*CHAR_WIDTH + i]);
//         for (uint8_t j = 0; j < CHAR_HEIGHT; j++, ch_column >>=1){

//             drawPixel(x+i, y+j, (ch_column & 1) ? color : !color);
//         }
//     }   
// }

// /****************************************************************/
// /** @brief  Redefine a write method for Print.h
//     @param  c   input char
// */
// /****************************************************************/
// size_t ST7558::write(const uint8_t c) {
    
//     if (c == '\n') {

//         cursor_x = 0;
//         cursor_y += 8;
//     }
//     if (cursor_x > ST7558_WIDTH) {

//         cursor_x = 0;
//         cursor_y += 8;
//     }
//     drawChar(cursor_x, cursor_y, c, textcolor);
//     cursor_x+=6;
//     return 1; // for compatibility with Print.h
// }
