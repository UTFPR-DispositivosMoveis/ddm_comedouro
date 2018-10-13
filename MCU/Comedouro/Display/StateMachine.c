#include    "Display.h"

static void fn_off(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT | OK_BIT)){
    case    NEXT_BIT:
        displayTurnOn(display, HIGH);
        display->var_nome = "Alarme 1:";
        display->state = st_alarme1;
        return;
    case    PREV_BIT:
        displayTurnOn(display, HIGH);
        display->var_nome = "Alarme 1:";
        display->state = st_alarme1;
        return;
    case    INC_BIT:
        displayTurnOn(display, HIGH);
        display->var_nome = "Alarme 1:";
        display->state = st_alarme1;
        return;
    case    DEC_BIT:
        displayTurnOn(display, HIGH);
        display->var_nome = "Alarme 1:";
        display->state = st_alarme1;
        return;
    case    OK_BIT:
        displayTurnOn(display, LOW);
        display->var_nome = "";
        display->state = st_off;
        return;
    }

}


static void fn_alarme1(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT | OK_BIT)){
    case    NEXT_BIT:
        display->var_nome = "Alarme 2:";
        display->state = st_alarme2;
        return;
    case    PREV_BIT:
        display->var_nome = "Tempo Aberto:";
        display->state = st_tmpAberto;
        return;
    case    INC_BIT:
        display->var_nome = "Alarme 1:";
        display->var[display->state] += 100;
        if(display->var[display->state] > 2400)
            display->var[display->state] = 2400;
        return;
    case    DEC_BIT:
        display->var_nome = "Alarme 1:";
        display->var[display->state] -= 100;
        if(display->var[display->state] < -100)
            display->var[display->state] = -100;
        return;
    case    OK_BIT:
        displayTurnOn(display, LOW);
        display->var_nome = "";
        display->state = st_off;
        return;
    }

}

static void fn_alarme2(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT | OK_BIT)){
    case    NEXT_BIT:
        display->var_nome = "Alarme 3:";
        display->state = st_alarme3;
        return;
    case    PREV_BIT:
        display->var_nome = "Alarme 1:";
        display->state = st_alarme1;
        return;
    case    INC_BIT:
        display->var_nome = "Alarme 2:";
        display->var[display->state] += 100;
        if(display->var[display->state] > 2400)
            display->var[display->state] = 2400;
        return;
    case    DEC_BIT:
        display->var_nome = "Alarme 2:";
        display->var[display->state] -= 100;
        if(display->var[display->state] < -100)
            display->var[display->state] = -100;
        return;
    case    OK_BIT:
        displayTurnOn(display, LOW);
        display->var_nome = "";
        display->state = st_off;
        return;
    }

}

static void fn_alarme3(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT | OK_BIT)){
    case    NEXT_BIT:
        display->var_nome = "Tempo Aberto:";
        display->state = st_tmpAberto;
        return;
    case    PREV_BIT:
        display->var_nome = "Alarme 2:";
        display->state = st_alarme2;
        return;
    case    INC_BIT:
        display->var_nome = "Alarme 3:";
        display->var[display->state] += 100;
        if(display->var[display->state] > 2400)
            display->var[display->state] = 2400;
        return;
    case    DEC_BIT:
        display->var_nome = "Alarme 3:";
        display->var[display->state] -= 100;
        if(display->var[display->state] < -100)
            display->var[display->state] = -100;
        return;
    case    OK_BIT:
        displayTurnOn(display, LOW);
        display->var_nome = "";
        display->state = st_off;
        return;
    }

}

static void fn_tmpAberto(Display_Interface_t *display, uint16_t flag){

    switch(flag & (PREV_BIT | NEXT_BIT | INC_BIT | DEC_BIT | OK_BIT)){
    case    NEXT_BIT:
        display->var_nome = "Alarme 1:";
        display->state = st_alarme1;
        return;
    case    PREV_BIT:
        display->var_nome = "Alarme 3:";
        display->state = st_alarme3;
        return;
    case    INC_BIT:
        display->var_nome = "Tempo Aberto:";
        display->var[display->state] += 200;
        return;
    case    DEC_BIT:
        display->var_nome = "Tempo Aberto:";
        display->var[display->state] -= 200;
        return;
    case    OK_BIT:
        displayTurnOn(display, LOW);
        display->var_nome = "";
        display->state = st_off;
        return;
    }

}

void initStateMachine(Display_Interface_t *display){


    display->action[st_off] = (Action_t)fn_off;
    display->action[st_alarme1] = (Action_t)fn_alarme1;
    display->action[st_alarme2] = (Action_t)fn_alarme2;
    display->action[st_alarme3] = (Action_t)fn_alarme3;
    display->action[st_tmpAberto] = (Action_t)fn_tmpAberto;
}


void exeStateMachine(Display_Interface_t *display, uint16_t flag){
    display->action[display->state](display, flag);
}



