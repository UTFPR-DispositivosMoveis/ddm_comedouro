/*
 * Display.h
 *
 *  Created on: 21 de set de 2018
 *      Author: raul
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "HD44780.h"

#define BUTTON_PORT         GPIO_PORT_P1
#define BUTTON_PORT_VECTOR  PORT1_VECTOR

#define NEXT_PIN            GPIO_PIN4
#define PREV_PIN            GPIO_PIN5
#define INC_PIN             GPIO_PIN3
#define DEC_PIN             GPIO_PIN2


#define NEXT_BIT            (0x0001)
#define PREV_BIT            (0x0002)
#define INC_BIT             (0x0004)
#define DEC_BIT             (0x0008)

typedef void (*Action_t)(void *display, uint16_t flag);
//typedef void (*handler_t)(uint64_t *flag);

typedef enum {
    st_off,
    st_alarme1,
    st_alarme2,
    st_alarme3,
    st_end
}   state_t;

typedef struct Display_Interface_t  {
    state_t state;
    Action_t action[st_end];
//    handler_t handler;

    int var[st_end];
    char *var_nome;
    char var_value[16];

} Display_Interface_t;


void initDisplay(Display_Interface_t *display);
void displayUpdate(Display_Interface_t *display, HD44780 *me);
uint16_t getButtonFlag();


void initStateMachine(Display_Interface_t *display);
void exeStateMachine(Display_Interface_t *display, uint16_t flag);

#endif /* DISPLAY_H_ */
