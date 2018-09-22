/*
 * main.c
 *
 *  Created on: 21 de set de 2018
 *      Author: raul
 */

#include "driverlib.h"
#include "HD44780.h"
#include "Display.h"

HD44780 theHD44780;

uint8_t state = 0;
Display_Interface_t display;

void displaySetState(Display_Interface_t *display){

    if(display->var[display->state] < 24 && display->var[display->state] >= 0){
        display->var_value[1] = ((display->var[display->state] % 10) + 48);
        display->var_value[0] =  ((display->var[display->state]/10) % 10) + 48;
    }   else    {   // TODO tem um jeito melhor de fazer isso?
        display->var_value[0] = 'O';
        display->var_value[1] = 'F';
        display->var_value[2] = 'F';
    }

}

void displayUpdate(Display_Interface_t *display, HD44780 *me){
    HD44780_clear(me);
    displaySetState(display);
    HD44780_print_string(me, display->var_nome);
    HD44780_setCursor(me, 1, 2);
    HD44780_print_string(me, display->var_value);
}

void GPIO_Config(){
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN);
    GPIO_enableInterrupt(GPIO_PORT_P1, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P1, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN);

}

void main(void)
{
    WDT_A_hold(WDT_A_BASE);

    HD44780_init(&theHD44780, GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5);

    HD44780_begin(&theHD44780, 16, 2);

    initDisplay(&display);
    HD44780_clear(&theHD44780);

    GPIO_Config();

    __bis_SR_register(LPM4_bits + GIE);

    __no_operation();

}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT1_VECTOR)))
#endif
void Port_1 (void){

    delayMilliseconds(2);

    exeSM(&display);

    displayUpdate(&display, &theHD44780);

    GPIO_clearInterrupt(GPIO_PORT_P1, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN);
}
