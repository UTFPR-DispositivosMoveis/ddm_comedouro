/*
 * SR04.c
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */

#include    <driverlib.h>
#include    "SR04.h"

Timer_B_initContinuousModeParam SR04Timer = {0};

void config_SR04(){

    /*
     * Timer A0 -> Clock = SMLCK = 1MHz
     * P4.3 -> Gera Pulso para o sensor ultrassonico
     * P1.2 -> Captura o tempo de resposta do sensor para determinar a dist�ncia
     */

    GPIO_setAsInputPin (SR04_ECHO_PORT, SR04_ECHO_PIN);
    GPIO_setAsOutputPin (SR04_TRIG_PORT, SR04_TRIG_PIN);

    SR04Timer.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;
    SR04Timer.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    SR04Timer.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    SR04Timer.timerClear = TIMER_B_DO_CLEAR;
    SR04Timer.startTimer = false;
    Timer_B_initContinuousMode(TIMER_B0_BASE, &SR04Timer);
    Timer_B_startCounter(TIMER_B0_BASE, TIMER_B_CONTINUOUS_MODE);
}

unsigned int get_Distance(){
    /*
     * Gera o pulso de 10 uS
     */
    GPIO_setOutputHighOnPin(SR04_TRIG_PORT, SR04_TRIG_PIN);
    __delay_cycles (264);
    GPIO_setOutputLowOnPin(SR04_TRIG_PORT, SR04_TRIG_PIN);

    while (!GPIO_getInputPinValue(SR04_ECHO_PORT, SR04_ECHO_PIN));   // Wait for start of echo pulse
    uint16_t start_Count = Timer_B_getCounterValue (TIMER_B0_BASE);

    while (GPIO_getInputPinValue(SR04_ECHO_PORT, SR04_ECHO_PIN));    // Wait for end of echo pulse
    uint16_t stop_Count = Timer_B_getCounterValue (TIMER_B0_BASE);

    static unsigned int dist;

    if(stop_Count > start_Count)
        dist = stop_Count - start_Count;
    else
        dist = 65535 + stop_Count - start_Count;

    return dist;
}
