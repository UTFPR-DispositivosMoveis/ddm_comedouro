/*------------------------------------------------------------------------------
             Outra Vers�o para Library de Manipula��o de LCDs
               para MSP430 - Compilador IAR versao 5.20
                 Adaptada por Francisco Fambrini - 9/10/2011
                              ffambrini@gmail.com
Baseada no seguinte original:
http://groups.google.com/group/hive76-discussion/browse_thread/thread/e0f340ed29a2acad
Derivada do ARDUINO
Agradecimento ao prof. Alessandro Cunha - TechTraining por indicar o original
Essa biblioteca � Freeware - use por sua conta e risco
                       VERS�O 1.0  - DATA: 9/10/2011
--------------------------------------------------------------------------------
Aterre o pino 3 do LCD (R/W aterrado)
Ligue o pino 2 do LCD em 5 volts externos ao LaunchPad, porque o MSP430 trabalha com 3,6V
LCD no Modo de 4 bits sem busyflag
PINAGEM DO LCD:
                   MSP430uC              LCD Display    
                    p1.7                    D7               
                    p1.6                    D6
                    p1.5                    D5
                    p1.4                    D4
                    p1.3                    EN
                    p1.2                    RS

// Para configurar os pinos do LCD use a seguinte fun�ao

//                  HD44780_init( &theHD44780, RS , E , d4, d5, d6, d7 );

// no caso acima:   HD44780_init(&theHD44780 , 2 , 3 ,   4,  5,  6, 7 );
-------------------------------------------------------------------------------*/

#include "msp430f5529.h"
#include "driverlib.h"


#define HD44780LIB_H_

#define uint8_t             unsigned int
#define uint16_t            unsigned int
#define WORD                unsigned int
#define byte                unsigned char
#define LOW                 0
#define HIGH                1
#define INPUT               0
#define OUTPUT              1

#define DISPLAY_PORT    GPIO_PORT_P3

//------------------------------------------------------------------------------
// Comando de digital I/O 
void pinMode(int pin, int isOutput);
//------------------------------------------------------------------------------
void digitalWrite(WORD pin, WORD setOrReset);
//------------------------------------------------------------------------------
inline int digitalRead(WORD pin);
//------------------------------------------------------------------------------

// Fun��es que fazem temporiza��es, baseadas nos Timers do MSP430
//------------------------------------------------------------------------------
void delayMicroseconds(unsigned int time);
//------------------------------------------------------------------------------
void delayMicrosecondszz(unsigned int time);
//------------------------------------------------------------------------------
inline void delayMilliseconds(unsigned int delay);
//------------------------------------------------------------------------------
void delayMillisecondszz(unsigned int time);
//------------------------------------------------------------------------------
// Comandos que manipulam o display LCD

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00


typedef struct _HD44780{

    uint8_t _rs_pin; // LOW: command.  HIGH: character.

    uint8_t _enable_pin; // activated by a HIGH pulse.
    uint8_t _data_pins[4];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _initialized;

    uint8_t _numlines,_currline;

} HD44780;

void HD44780_init(HD44780 *me, int rs, int enable, int d0, int d1, int d2, int d3);
void HD44780_begin(HD44780 *me, uint8_t cols, uint8_t lines);
void HD44780_send(HD44780 *me, uint8_t value, uint8_t mode);
void HD44780_command(HD44780 *me, uint8_t value);             //essa fun��o estava comentada na original
void HD44780_write4bits(HD44780 *me, uint8_t value);
void HD44780_pulseEnable(HD44780 *me);
void HD44780_clear(HD44780 *me);
void HD44780_display(HD44780 *me);
void HD44780_print_byte(HD44780 *me, uint8_t b);
void HD44780_print_string(HD44780 *me, const char c[]);
void HD44780_write(HD44780 *me, uint8_t value);

//-------------------------------------------------------------------------------------------------------------
// Posiciona o cursor para escrever na coluna e linha desejada do LCD
void HD44780_setCursor(HD44780 *me, uint8_t col, uint8_t row);
//--------------------------------------------------------------------------------------------------------------

