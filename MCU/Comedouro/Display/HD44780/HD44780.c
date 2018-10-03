/*
 * HD44780.c
 *
 *  Created on: 22 de set de 2018
 *      Author: raul
 */


#include    "HD44780.h"
#include    "port_HD44780.h"


void HD44780_write(HD44780 *me, uint8_t value){
    HD44780_send(me, value, HIGH);
}

inline void HD44780_command(HD44780 *me, uint8_t value) {
    HD44780_send(me, value, LOW);
}

void HD44780_send(HD44780 *me, uint8_t value, uint8_t mode) {
    digitalWrite(me->_display_port, me->_rs_pin, mode);
    HD44780_write4bits(me, value>>4);
    HD44780_write4bits(me, value);
}


//Fun��o que configura os pinos do LCD
void HD44780_init(HD44780 *me, WORD display_port, int rs, int enable, int d0, int d1, int d2, int d3){
    me->_display_port = display_port;
    me->_rs_pin = rs;
    me->_enable_pin = enable;

    me->_data_pins[0] = d0;
    me->_data_pins[1] = d1;
    me->_data_pins[2] = d2;
    me->_data_pins[3] = d3;

    pinMode(me->_display_port, rs, OUTPUT);
    pinMode(me->_display_port, enable, OUTPUT);

    me->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

        //configura o numero de linhas e de colunas do LCD
void HD44780_begin(HD44780 *me, uint8_t cols, uint8_t lines) {
    if (lines > 1) {
        me->_displayfunction |= LCD_2LINE;
    }
    me->_numlines = lines;
    me->_currline = 0;


    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    delayMicroseconds(50000);
    // Now we pull both RS and R/W low to begin commands
    digitalWrite(me->_display_port, me->_rs_pin, LOW);
    digitalWrite(me->_display_port, me->_enable_pin, LOW);

    //put the LCD into 4 bit or 8 bit mode
    if (! (me->_displayfunction & LCD_8BITMODE)) {
           // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        HD44780_write4bits(me, 0x03);
        delayMicroseconds(4500); // wait min 4.1ms

        // second try
        HD44780_write4bits(me, 0x03);
        delayMicroseconds(4500); // wait min 4.1ms

        // third go!
        HD44780_write4bits(me, 0x03);
        delayMicroseconds(150);

        // finally, set to 8-bit interface
        HD44780_write4bits(me, 0x02);
    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        HD44780_command(me, LCD_FUNCTIONSET | me->_displayfunction);
        delayMicroseconds(4500);  // wait more than 4.1ms

        // second try
        HD44780_command(me, LCD_FUNCTIONSET | me->_displayfunction);
        delayMicroseconds(150);

        // third go
        HD44780_command(me, LCD_FUNCTIONSET | me->_displayfunction);
    }

    // finally, set # lines, font size, etc.
    HD44780_command(me, LCD_FUNCTIONSET | me->_displayfunction);

    // turn the display on with no cursor or blinking default
    me->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    HD44780_display(me);

    // clear it off
    HD44780_clear(me);

    // Initialize to default text direction (for romance languages)
    me->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    HD44780_command(me, LCD_ENTRYMODESET | me->_displaymode);

}

void HD44780_write4bits(HD44780 *me, uint8_t value) {
    unsigned int i;
    for (i = 0; i < 4; i++) {
        pinMode(me->_display_port, me->_data_pins[i],OUTPUT);
        digitalWrite(me->_display_port, me->_data_pins[i], (value >> i) & 0x01);
    }

    HD44780_pulseEnable(me);
}

void HD44780_pulseEnable(HD44780 *me) {
    digitalWrite(me->_display_port, me->_enable_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(me->_display_port, me->_enable_pin, HIGH);
    delayMicroseconds(2);    // enable pulse must be >450ns
    digitalWrite(me->_display_port, me->_enable_pin, LOW);
    delayMicroseconds(100);   // commands need > 37us to settle
}

void HD44780_clear(HD44780 *me){
    HD44780_command(me, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    delayMicroseconds(2000);  // this command takes a long time!
}

void HD44780_display(HD44780 *me) {
    me->_displaycontrol |= LCD_DISPLAYON;
    HD44780_command(me, LCD_DISPLAYCONTROL | me->_displaycontrol);
}

void HD44780_print_byte(HD44780 *me, uint8_t b){
    HD44780_write(me, b);
}

void HD44780_print_string(HD44780 *me, const char c[]){
    while (*c)
    HD44780_print_byte(me, *c++);
}






void HD44780_setCursor(HD44780 *me, uint8_t col, uint8_t row){

    col--; //aceecentado por Francisco Fambrini em 9/0/2011
    row--;

    const int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

    if (row > me->_numlines )
        row = me->_numlines-1;    // we count rows starting w/0

    HD44780_command(me, LCD_SETDDRAMADDR | (col + row_offsets[row]));

}
