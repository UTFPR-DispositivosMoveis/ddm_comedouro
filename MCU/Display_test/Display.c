#include "driverlib.h"
#include "Display.h"


static void fn_off(Display_Interface_t *display){

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, NEXT_PIN | PREV_PIN | INC_PIN | DEC_PIN)){
        //displayTurnOn(true);  TODO
        display->state = st_alarme1;
    }

}


static void fn_alarme1(Display_Interface_t *display){


    if(GPIO_getInterruptStatus(GPIO_PORT_P1, NEXT_PIN))
        display->state = st_alarme2;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, PREV_PIN))
        display->state = st_alarme3;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, INC_PIN))
        display->var[display->state]++;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, DEC_PIN))
        display->var[display->state]--;

     display->var_nome = "Alarme 1:";
}

static void fn_alarme2(Display_Interface_t *display){

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, NEXT_PIN))
        display->state = st_alarme3;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, PREV_PIN))
        display->state = st_alarme1;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, INC_PIN))
        display->var[display->state]++;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, DEC_PIN))
        display->var[display->state]--;

    display->var_nome = "Alarme 2:";
}

static void fn_alarme3(Display_Interface_t *display){

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, NEXT_PIN))
        display->state = st_alarme1;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, PREV_PIN))
        display->state = st_alarme2;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, INC_PIN))
        display->var[display->state]++;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, DEC_PIN))
        display->var[display->state]--;

    display->var_nome = "Alarme 3:";
}

void initDisplay(Display_Interface_t *display){

    display->state = st_off;

    display->var[st_alarme1] = 15;
    display->var[st_alarme2] = 15;
    display->var[st_alarme3] = 15;


    display->action[st_off] = (Action_t)fn_off;
    display->action[st_alarme1] = (Action_t)fn_alarme1;
    display->action[st_alarme2] = (Action_t)fn_alarme2;
    display->action[st_alarme3] = (Action_t)fn_alarme3;

}


void exeSM(Display_Interface_t *display){
    display->action[display->state](display);
}



