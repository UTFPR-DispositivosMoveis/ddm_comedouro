#include    "Display.h"

static void fn_off(Display_Interface_t *display, uint16_t flag){

    if(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT)){
        //displayTurnOn(true);  TODO
        display->var_nome = "";
        display->state = st_alarme1;
        return;
    }

}


static void fn_alarme1(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT)){
    case    NEXT_BIT:
        display->var_nome = "Alarme 1:";
        display->state = st_alarme2;
        return;
    case    PREV_BIT:
        display->var_nome = "Alarme 1:";
        display->state = st_alarme3;
        return;
    case    INC_BIT:
        display->var_nome = "Alarme 1:";
        display->var[display->state]++;
        if(display->var[display->state] > 24)
            display->var[display->state] = 24;
        return;
    case    DEC_BIT:
        display->var_nome = "Alarme 1:";
        display->var[display->state]--;
        if(display->var[display->state] < -1)
            display->var[display->state] = -1;
        return;
    }

}

static void fn_alarme2(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT)){
    case    NEXT_BIT:
        display->var_nome = "Alarme 2:";
        display->state = st_alarme3;
        return;
    case    PREV_BIT:
        display->var_nome = "Alarme 2:";
        display->state = st_alarme1;
        return;
    case    INC_BIT:
        display->var_nome = "Alarme 2:";
        display->var[display->state]++;
        if(display->var[display->state] > 24)
            display->var[display->state] = 24;
        return;
    case    DEC_BIT:
        display->var_nome = "Alarme 2:";
        display->var[display->state]--;
        if(display->var[display->state] < -1)
            display->var[display->state] = -1;
        return;
    }

}

static void fn_alarme3(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT)){
    case    NEXT_BIT:
        display->var_nome = "Alarme 3:";
        display->state = st_alarme1;
        return;
    case    PREV_BIT:
        display->var_nome = "Alarme 3:";
        display->state = st_alarme2;
        return;
    case    INC_BIT:
        display->var_nome = "Alarme 3:";
        display->var[display->state]++;
        if(display->var[display->state] > 24)
            display->var[display->state] = 24;
        return;
    case    DEC_BIT:
        display->var_nome = "Alarme 3:";
        display->var[display->state]--;
        if(display->var[display->state] < -1)
            display->var[display->state] = -1;
        return;
    }

}

void initStateMachine(Display_Interface_t *display){


    display->action[st_off] = (Action_t)fn_off;
    display->action[st_alarme1] = (Action_t)fn_alarme1;
    display->action[st_alarme2] = (Action_t)fn_alarme2;
    display->action[st_alarme3] = (Action_t)fn_alarme3;
}


void exeStateMachine(Display_Interface_t *display, uint16_t flag){
    display->action[display->state](display, flag);
}



