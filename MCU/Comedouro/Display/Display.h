/*
 * Display.h
 *
 *  Created on: 21 de set de 2018
 *      Author: raul
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "HD44780/HD44780.h"



#define DISPLAY_PORT        GPIO_PORT_P6

#define DISPLAY_PIN_RS      GPIO_PIN6
#define DISPLAY_PIN_EN      GPIO_PIN4
#define DISPLAY_PIN_D4      GPIO_PIN3
#define DISPLAY_PIN_D5      GPIO_PIN2
#define DISPLAY_PIN_D6      GPIO_PIN1
#define DISPLAY_PIN_D7      GPIO_PIN0

#define LED_PIN             GPIO_PIN5




#define BUTTON_PORT         GPIO_PORT_P2
#define BUTTON_PORT_VECTOR  PORT2_VECTOR


#define NEXT_PIN            GPIO_PIN6
#define PREV_PIN            GPIO_PIN4
#define OK_PIN              GPIO_PIN3
#define INC_PIN             GPIO_PIN2
#define DEC_PIN             GPIO_PIN0



#define NEXT_BIT            (0x0001)
#define PREV_BIT            (0x0002)
#define INC_BIT             (0x0004)
#define DEC_BIT             (0x0008)
#define OK_BIT              (0x0010)



typedef void (*Action_t)(void *display, uint16_t flag);
//typedef void (*handler_t)(uint64_t *flag);

typedef enum {
    st_off,
    st_alarme1,
    st_alarme2,
    st_alarme3,
    st_tmpAberto,
    st_end
}   state_t;

typedef struct Display_Interface_t  {
    state_t state;
    Action_t action[st_end];
//    handler_t handler;

    short int var[st_end];
    char *var_nome;
    char var_value[16];

} Display_Interface_t;

void Display_Config();
void initDisplay(Display_Interface_t *display);
void displayUpdate(Display_Interface_t *display, HD44780 *me);
uint16_t getButtonFlag();
void displayTurnOn(Display_Interface_t *display, int isOn);

void initStateMachine(Display_Interface_t *display);
void exeStateMachine(Display_Interface_t *display, uint16_t flag);

#endif /* DISPLAY_H_ */
