/*
 * main.c
 *
 *  Created on: 21 de set de 2018
 *      Author: raul
 */

#include    "Display/Display.h"

HD44780 theHD44780;

Timer_B_initContinuousModeParam param = {0};

uint8_t state = 0;
Display_Interface_t display;


void GPIO_Config(){
    GPIO_setAsOutputPin(DISPLAY_PORT, LED_PIN);
    GPIO_setAsInputPinWithPullDownResistor(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);
    GPIO_enableInterrupt(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);
    GPIO_selectInterruptEdge(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);
}

void main(void)
{
    WDT_A_hold(WDT_A_BASE);

    HD44780_init(&theHD44780, GPIO_PIN0, GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, GPIO_PIN4, GPIO_PIN5);

    HD44780_begin(&theHD44780, 16, 2);

    initStateMachine(&display);

    initDisplay(&display);

    displayUpdate(&display, &theHD44780);

    GPIO_Config();

    __bis_SR_register(LPM3_bits + GIE);

    __no_operation();

}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=BUTTON_PORT_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(BUTTON_PORT_VECTOR)))
#endif
void Port_1 (void){



    delayMilliseconds(2);

    exeStateMachine(&display, getButtonFlag());

    displayUpdate(&display, &theHD44780);

    GPIO_clearInterrupt(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);

}

