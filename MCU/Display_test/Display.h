/*
 * Display.h
 *
 *  Created on: 21 de set de 2018
 *      Author: raul
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


//typedef void (*Action_t)(void *sm, unsigned char data);

//typedef void (*handler_t)(unsigned char* );

typedef enum {
    st_alarme1,
    st_alarme2,
    st_alarme3,
    st_end
}   state_t;

typedef struct Display_Interface_t  {
    state_t state;
//    Action_t action[st_end];
//    handler_t handler;

    int var[st_end];
    char* var_nome[st_end];
    char* var_value;

} Display_Interface_t;

void displaySetState(Display_Interface_t *display);
void initDisplay(Display_Interface_t *display);


#endif /* DISPLAY_H_ */
