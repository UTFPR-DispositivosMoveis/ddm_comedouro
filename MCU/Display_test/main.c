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


void displayUpdate(Display_Interface_t *display, HD44780 *me){

    HD44780_clear(me);
    displaySetState(display);
    HD44780_print_string(me, display->var_nome[display->state]);
    HD44780_setCursor(me, 1, 2);
    HD44780_print_string(me, display->var_value);

}

void main(void)
{
    WDT_A_hold(WDT_A_BASE);

    HD44780_init(&theHD44780, GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5);

    HD44780_begin(&theHD44780, 16, 2);
    initDisplay(&display);

    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1, GPIO_PIN4 | GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4 | GPIO_PIN5);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN4 | GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN4 | GPIO_PIN5);

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

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN4)){
        display.state++;
        if(display.state == st_end)
            display.state = st_alarme1;
    }

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN5)){
        if(display.state <= st_alarme1)
            display.state = st_end;
        else
            display.state--;
    }

    displayUpdate(&display, &theHD44780);

    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN4 | GPIO_PIN5);
}
