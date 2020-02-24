#include "ST7558.h"
#define RST A3

ST7558 lcd = ST7558(A3);

void setup() {
    lcd.begin();
    lcd.setContrast(70);
}


void loop() {
    
    lcd.display();
    //lcd.invert();
    //lcd.on();
    //lcd.off();
}