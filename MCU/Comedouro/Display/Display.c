/*
 * Display.c
 *
 *  Created on: 22 de set de 2018
 *      Author: raul
 */

#include    <string.h>
#include    "Display.h"


void displayTurnOn(int isOn){
    if(isOn)
        GPIO_setOutputHighOnPin(DISPLAY_PORT, LED_PIN);
    else
        GPIO_setOutputLowOnPin(DISPLAY_PORT, LED_PIN);
}

void initDisplay(Display_Interface_t *display){


    display->var[st_alarme1] = -1;
    display->var[st_alarme2] = -1;
    display->var[st_alarme3] = -1;

    display->state = st_off;
    display->var_nome = "";
}

uint16_t getButtonFlag(){

    uint16_t flag = 0x0000;

    if(GPIO_getInterruptStatus(BUTTON_PORT, OK_PIN))
        flag |= OK_BIT;

    if(GPIO_getInterruptStatus(BUTTON_PORT, NEXT_PIN))
        flag |= NEXT_BIT;

    if(GPIO_getInterruptStatus(BUTTON_PORT, PREV_PIN))
        flag |= PREV_BIT;

    if(GPIO_getInterruptStatus(BUTTON_PORT, INC_PIN))
        flag |= INC_BIT;

    if(GPIO_getInterruptStatus(BUTTON_PORT, DEC_PIN))
        flag |= DEC_BIT;

    return flag;
}

void displayClear(Display_Interface_t *display, HD44780 *me){
    HD44780_clear(me);
    strcpy(display->var_value, "");
}

void displaySetState(Display_Interface_t *display){

    if(display->state == st_off){
        strcpy(display->var_value, "");
    }

    else    {

        if(display->var[display->state] < 24 && display->var[display->state] >= 0){
            display->var_value[0] = ((display->var[display->state]/10) % 10) + 48;
            display->var_value[1] = ((display->var[display->state] % 10) + 48);
            strcpy(&display->var_value[2], ":00");
        }   else    {
            strcpy(display->var_value, "OFF");
        }
    }

}

void displayUpdate(Display_Interface_t *display, HD44780 *me){

    displayClear(display, me);

    displaySetState(display);

    HD44780_print_string(me, display->var_nome);
    HD44780_setCursor(me, 1, 2);
    HD44780_print_string(me, display->var_value);
}
