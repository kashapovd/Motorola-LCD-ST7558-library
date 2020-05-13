#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <Adafruit_GFX.h>
#include <ST7558.h>
#include <logo.h>

#define DEBUG 0             // 1 - enable, 0 - disable
#define ENABLE_FPS 1

/* set pins

    Display connections:
        VCC -> +3.3v arduino pin
        GND -> +0v arduino pin 
        SCL -> A5 arduino pin
        SDA -> A4 arduino pin
        RST -> A3 arduino pin

    Joystick connections:
        VCC -> +5v arduino pin
        GND -> +0v arduino pin
        URX -> A2 arduino pin
        URY -> A1 arduino pin
        SW -> 3 arduino pin via 10k pullup resistor

    speaker -> 4 arduino pin
*/

#define RST_PIN A3
#define STICK_X A2
#define STICK_Y A1
#define SW 3
#define SPK 4

// lcd resolution
#define width 96
#define height 65

// gamefield
#define field_x 95
#define field_y 56
#define a 3

// functions
void
    draw_menu(),
    draw_options(),
    draw_gameover_menu(),
    draw_game(),
    default_set(),
    on_click(),
    get_dir(),
    logic(),
    no_tone(uint8_t);

uint8_t menu_switcher(uint8_t, uint8_t);

// main struct
struct snake {
    signed
        x ,                  // snake horizontally position
        y,                  // snake vertical position
        prev_dir,           // previos direction of snake. need for correct game experience
        n_eat;              // the number of eaten food

    struct food {
        unsigned
            x,              // food horizontally position
            y;              // food vertical position
    } food;
    
    //directions
    enum dir { 
        STAY,
        LEFT,
        RIGHT,
        UP,
        DOWN
    } dir;

    // structs for snake's tail
    struct tail {
        signed 
            x,
            y;
    } tail[50];

    
} snake;

struct item {
    unsigned
        main,
        gameover,
        options;

} menu_item;

struct states {
    unsigned 
        gameover : 1,
        options : 1, 
        menu : 1,
        borders : 1,
        flicker : 1,
        inverter : 1,
        spk : 1;
} state;

uint8_t contrast;                 // lcd contrast parameter
uint32_t prev_millis;

#if(ENABLE_FPS)
uint32_t fps_millis;
uint8_t fps;
#endif

ST7558 lcd = ST7558(RST_PIN);

void setup() {

#if(DEBUG)
    Serial.begin(9600); // for debug
#endif

    randomSeed(analogRead(A7)); // rand init

    pinMode(SW, INPUT); // for switch on a joystick

    // lcd init
    lcd.begin();
    lcd.setContrast(70);
    lcd.clearDisplay();

    // game init
    default_set();
    state.borders = false;
    state.inverter = true;
    state.menu = true;
    prev_millis = 0;
    state.flicker = true;
    state.spk = false;
}

// main loop
void loop() {
    
    lcd.invertDisplay(state.inverter);

    if (state.menu)
        draw_menu();
    else if (state.options)
        draw_options();
    else {

        if (!state.gameover)
            draw_game();
        else
            draw_gameover_menu();
    }

#if(DEBUG)
    Serial.println("Snake: [" + String(snake.x) + ',' + String(snake.y) + ']');
#endif

    lcd.display();
    lcd.clearDisplay();
}

// set default game options
void default_set() {
    
    state.gameover = false;
    // start point for snake
    snake.x = 46;
    snake.y = 34;
    // start point for food
    snake.food.x = random(0, 30) * 3 + 1;
    snake.food.y = random(3, 17) * 3 + 1;

    snake.n_eat = 0;
    snake.dir = snake::STAY;
    menu_item.main = 1;
    menu_item.gameover = 1;
}

void draw_menu() {

    snake.dir = snake::STAY;

    lcd.drawBitmap(random(14, 16), random(2, 4), snake_logo, snake_logo_w, snake_logo_h, BLACK);
    lcd.drawRoundRect((width / 2) - 16, 35, 33, 11, 2, BLACK);
    lcd.setCursor((width / 2) - 14, 37);
    lcd.print(F("start"));
    lcd.drawRoundRect((width / 2) - 22, 49, 45, 12, 2, BLACK);
    lcd.setCursor((width / 2) - 20, 51);
    lcd.print(F("options"));

    const static uint8_t n_items = 2;
    menu_item.main = menu_switcher(menu_item.main, n_items);
    if (menu_item.main == 1)
        lcd.drawRoundRect((width / 2) - 17, 34, 35, 13, 2, state.flicker ? BLACK : WHITE);
    else if (menu_item.main == 2)
        lcd.drawRoundRect((width / 2) - 23, 48, 47, 14, 2, state.flicker ? BLACK : WHITE);
    state.flicker = !state.flicker;

    if (!digitalRead(SW)) {

        if (menu_item.main == 1)
            default_set(); 
        else {

            state.options = true;
            menu_item.options = 1;
        }
        state.menu = false;
        on_click();
    }
}

void draw_options() {
    
    snake.dir = snake::STAY;

    lcd.setCursor(8, 1);
    lcd.print(F("contrast:"));
    lcd.print(contrast);
    lcd.setCursor(8, 9);
    lcd.print(F("borders:"));
    lcd.print((state.borders ? F("true") : F("false")));
    lcd.setCursor(8, 17);
    lcd.print(F("theme:"));
    lcd.print((state.inverter ? F("dark") : F("light")));
    lcd.setCursor(8, 25);
    lcd.print(F("exit"));


    const static uint8_t n_items = 4;
    menu_item.options = menu_switcher(menu_item.options, n_items);

    if (menu_item.options == 1) {

        lcd.setCursor(0, 1);
        lcd.print(F(">"));

        get_dir();
        switch (snake.dir) {

            case snake::LEFT:
                contrast += 5;
                snake.dir = snake::STAY;
                break;
            case snake::RIGHT:
                contrast -= 5;
                snake.dir = snake::STAY;
            default:
                break;
        }
        lcd.setContrast(contrast);
    } else if (menu_item.options == 2) {

        lcd.setCursor(0, 9);
        lcd.print(F(">"));

        if (!digitalRead(SW)) {

            state.borders = !state.borders;
            on_click();
        }
    } else if (menu_item.options == 3) {

        lcd.setCursor(0, 17);
        lcd.print(F(">"));

        if (!digitalRead(SW)) {

            state.inverter = !state.inverter;
            on_click();
        }
    } else {

        lcd.setCursor(0, 25);
        lcd.print(F(">"));

        if (!digitalRead(SW)) {

            state.menu = true;
            state.options = false;
            on_click();
        }
    }
}

void draw_game() {

    (state.inverter) ? lcd.drawLine(0, 9, 95, 9, BLACK) : lcd.drawRect(0, 9, field_x, field_y, BLACK);
    lcd.setCursor(1, 1);
    lcd.print(F("Score:"));
    lcd.print(snake.n_eat);

#if(ENABLE_FPS)
    fps = 1000/(millis()-fps_millis);
    lcd.setCursor(72,1);
    lcd.setTextColor(BLACK);
#if(DEBUG)
    Serial.println(fps);
#endif
    lcd.print(fps);
    lcd.print("fps");
    fps_millis = millis();
#endif

    logic();

    // draw snake's head
    lcd.fillRect(snake.x, snake.y, a, a, BLACK);
    // draw snake's tail
    for (uint8_t i = 0; i < snake.n_eat; i++)
        lcd.fillRect(snake.tail[i].x, snake.tail[i].y, a, a, BLACK);
    // draw food
    lcd.drawRect(snake.food.x, snake.food.y, a, a, BLACK);
}

void draw_gameover_menu() {
    
    snake.dir = snake::STAY;

    lcd.drawBitmap(random(-1, 1), random(-1, 1), gameover_logo, gameover_logo_w, gameover_logo_h, BLACK);
    lcd.drawRoundRect((width / 2) - 22, 35, 45, 11, 2, BLACK);
    lcd.setCursor((width / 2) - 20, 37);
    lcd.print(F("restart"));

    lcd.drawRoundRect((width / 2) - 13, 49, 27, 12, 2, BLACK);
    lcd.setCursor((width / 2) - 11, 51);
    lcd.print(F("menu"));

    const static uint8_t n_items = 2;
    menu_item.gameover = menu_switcher(menu_item.gameover, n_items);

    if (menu_item.gameover == 1)
        lcd.drawRoundRect((width / 2) - 23, 34, 47, 13, 2, state.flicker ? BLACK : WHITE); 
    else if (menu_item.gameover == 2)
        lcd.drawRoundRect((width / 2) - 14, 48, 29, 14, 2, state.flicker ? BLACK : WHITE);
    state.flicker = !state.flicker;

    if (!digitalRead(SW)) {

        on_click();
        if (menu_item.gameover == 1)
            default_set();
        else
            state.menu = true;
    }
}

uint8_t menu_switcher(uint8_t menu_item, uint8_t n_items)
{
    get_dir();
    uint8_t cur_item = menu_item;
    if (millis() - prev_millis >= 250 && snake.dir != 0) {

        prev_millis = millis();
        switch (snake.dir) {

            case snake::DOWN:
                menu_item--;
                if (menu_item <= 0)
                    menu_item = n_items;
                break;
            case snake::UP:
                menu_item++;
                if (menu_item > n_items)
                    menu_item = 1;
                break;
            default:
                break;
        }
    }
    if (cur_item != menu_item) {

        tone(SPK, 80); // 80 Hz tone frequency
        state.spk = true;
    }
    no_tone(100);

    return menu_item;
}

void on_click() {
    while (!digitalRead(SW));
}

void no_tone(uint8_t duration) {

    if (millis() - prev_millis >= duration && state.spk) {

        prev_millis = 0;
        noTone(SPK);
        state.spk = false;
    }
}

void get_dir() {

    int stick_x = analogRead(STICK_X);
    int stick_y = analogRead(STICK_Y);

    if (stick_x > 700)
        snake.dir = snake::RIGHT;
    else if (stick_x < 100)
        snake.dir = snake::LEFT;
    if (stick_y > 700)
        snake.dir = snake::UP;
    else if (stick_y < 100)
        snake.dir = snake::DOWN;
}

void logic() {

    for (int i = snake.n_eat; i > 0; i--) {

        snake.tail[i].x = snake.tail[i - 1].x;
        snake.tail[i].y = snake.tail[i - 1].y;
    }
    snake.tail[0].x = snake.x;
    snake.tail[0].y = snake.y;

    get_dir();
    switch (snake.prev_dir) {

        case (snake::UP):
            if (snake.dir == snake::DOWN)
                snake.dir = snake::UP;
            break;
        case snake::DOWN:
            if (snake.dir == snake::UP)
                snake.dir = snake::DOWN;
            break;
        case snake::LEFT:
            if (snake.dir == snake::RIGHT)
                snake.dir = snake::LEFT;
            break;
        case snake::RIGHT:
            if (snake.dir == snake::LEFT)
                snake.dir = snake::RIGHT;
            break;
        default:
            break;
    }
    switch (snake.dir) {
        
        case snake::UP:
            if (snake.x != -2)
                snake.y += a;
            break;
        case snake::DOWN:
            if (snake.x != 94)
                snake.y -= a;
            break;
        case snake::LEFT:
            if (snake.y != 7)
                snake.x -= a;
            break;
        case snake::RIGHT:
            if (snake.y != 64)
                snake.x += a;
            break;
        case snake::STAY:
            break;
    }
    snake.prev_dir = snake.dir;

    if (!state.borders) {

        switch (snake.y) {

            case 64:
                snake.y = 10;
                break;
            case 7:
                snake.y = 64;
                break;
        }
        switch (snake.x) {

            case -2:
                snake.x = 91;
                break;
            case 94:
                snake.x = 1;
                break;
        }
    } 
    else
        // if u bump into state.borders
        if (snake.y == 7 || snake.y == 64 || snake.x == -2 || snake.x == 94)
            state.gameover = true;

    if (snake.x == snake.food.x && snake.y == snake.food.y) {
        
        // calculate food position 
        snake.food.x = random(0, 30) * 3 + 1;
        snake.food.y = random(3, 17) * 3 + 1;
        snake.n_eat++;
        tone(SPK, 150); // 150 Hz tone frequency
        state.spk = true;
        prev_millis = millis();
    }
    no_tone(100); // 100ms tone duration

    // check self-eating
    for (int k = 0; k < snake.n_eat; k++)
        if (snake.tail[k].x == snake.x && snake.tail[k].y == snake.y)
            state.gameover = true;
}
