/*
 * Display.c
 *
 *  Created on: 22 de set de 2018
 *      Author: raul
 */

#include    <string.h>
#include    "Display.h"
#include    "Global_Var.h"
#include    "IO/Buzzer/Buzzer.h"
#include    "IO/SR04/SR04.h"
#include    "Display/HD44780/port_HD44780.h"
#include    "driverlib.h"

short int hora_alarme1;
short int hora_alarme2;
short int hora_alarme3;
unsigned short int tempo_motor;
unsigned char buzzer;

void Display_Config(){
    GPIO_setAsOutputPin(DISPLAY_PORT, LED_PIN);
    GPIO_setOutputLowOnPin(DISPLAY_PORT, LED_PIN);
    GPIO_setAsInputPinWithPullDownResistor(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);
    GPIO_enableInterrupt(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);
    GPIO_selectInterruptEdge(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);
}

void displayTurnOn(Display_Interface_t *display, int isOn){
    if(isOn){
        GPIO_setOutputHighOnPin(DISPLAY_PORT, LED_PIN);
        display->var[st_alarme1] = hora_alarme1;
        display->var[st_alarme2] = hora_alarme2;
        display->var[st_alarme3] = hora_alarme3;
        display->var[st_nivelRacao] = nivel_dist;
        display->var[st_tmpAberto] = (short int)tempo_motor;
    }
    else    {
        GPIO_setOutputLowOnPin(DISPLAY_PORT, LED_PIN);
        hora_alarme1 = display->var[st_alarme1];
        hora_alarme2 = display->var[st_alarme2];
        hora_alarme3 = display->var[st_alarme3];
        nivel_dist = display->var[st_nivelRacao];
        tempo_motor = (unsigned short int)display->var[st_tmpAberto];
    }
}

void initDisplay(Display_Interface_t *display){

    hora_alarme1 = display->var[st_alarme1] = 2500;
    hora_alarme2 = display->var[st_alarme2] = 2500;
    hora_alarme3 = display->var[st_alarme3] = 2500;
    tempo_motor = display->var[st_tmpAberto] = 1000;
    tempo_motor = display->var[st_buzzer] = 0;

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
    static uint8_t hora, minuto, seg;

    if(display->state == st_off){
        strcpy(display->var_value, "");
    }

    else if(display->state == st_buzzer){

        if(display->var[display->state] == 1){
            Abrir_Valvula (tempo_motor);
            strcpy(display->var_value, "ON");
        }
        /*
        else    {
            delayMilliseconds(100);
            set_BuzzerOff();
            delayMilliseconds(100);
            strcpy(display->var_value, "Alimentado");
        }*/
    }

    else if(display->state == st_nivelRacao){
        strcpy(display->var_value, "         ");
        //display->var[display->state] = 3;
        display->var[display->state] = get_Distance();
        display->var_value[0] = display->var[display->state] + 48;
    }

    else if(display->state == st_tmpAberto){  // Estado do tempo do motor
        seg = display->var[display->state] / 1000;

        display->var_value[0] = ((seg/10) % 10) + 48;
        display->var_value[1] = (seg % 10) + 48;
        strcpy(&display->var_value[2], ".");
        display->var_value[3] = (((display->var[display->state] % 1000) / 100) + 48);
        strcpy(&display->var_value[4], "s");
    }

    else if(display->var[display->state] < 2400 && display->var[display->state] >= 0){       // Estado de alarme
            hora = display->var[display->state] / 100;
            minuto = display->var[display->state] % 100;

            if(minuto > 60){
                minuto = 0;
                hora++;
            }

            display->var_value[0] = ((hora/10) % 10) + 48;
            display->var_value[1] = ((hora % 10) + 48);
            strcpy(&display->var_value[2], ":");
            display->var_value[3] = ((minuto/10) % 10) + 48;
            display->var_value[4] = ((minuto % 10) + 48);

    }   else    {
        strcpy(display->var_value, "OFF");
    }

}

void displayUpdate(Display_Interface_t *display, HD44780 *me){

    displayClear(display, me);

    displaySetState(display);

    HD44780_print_string(me, display->var_nome);
    HD44780_setCursor(me, 1, 2);
    HD44780_print_string(me, display->var_value);
}
