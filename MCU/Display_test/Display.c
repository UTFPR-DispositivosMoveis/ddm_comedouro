
#include "Display.h"

/*

static void fn_alarme1(sm_t *sm, uint8_t data){
    if(data == '>')
        sm->handler(sm->buffer);
    else if(data == '<')

    sm->state = st_stx;
}

static void fn_alarme2(Display_Interface_t *display, uint8_t data){
    if(data == '$')
        sm->handler(sm->buffer);
    sm->state = st_stx;
}

void initSM(Display_Interface_t *display, short addr, handler_t handl){
    sm->state = st_stx;
    sm->devAddr = addr;
    sm->action[st_alarme1] = (Action_t)fn_alarme1;
    sm->action[st_alarme2] = (Action_t)fn_alarme2;
    sm->devAddr = addr;
    sm->handler = handl;
    sm->flag_addr = 0;
}



void exeSM(Display_Interface_t *display, uint8_t data){
    sm->action[sm->state](sm, data);
}


*/

void displaySetState(Display_Interface_t *display){

    switch(display->state){
        case   st_alarme1:
            if(display->var[display->state] < 24 && display->var[display->state] >= 0){
                char c[2];
                c[0] = (display->var[display->state] % 10) + 48;
                c[1] =  ((display->var[display->state]/10) % 10) + 48;
                display->var_value = c;
            }   else
                display->var_value = "OFF";
            break;
    }
}

void initDisplay(Display_Interface_t *display){

    display->var_nome[st_alarme1] = "Alarme 1";
    display->var[st_alarme1] = 1;

    display->var_nome[st_alarme2] = "Alarme 2";
    display->var[st_alarme2] = 1;

    display->var_nome[st_alarme3] = "Alarme 3";
    display->var[st_alarme3] = 1;

    display->var_value = "";



}


/*
void exeSM(Display_Interface_t *display, uint8_t data){

}
*/
