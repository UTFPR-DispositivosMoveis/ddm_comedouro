/*
 * Buzzer.c
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */
#include    <driverlib.h>
#include    "Buzzer.h"

Timer_A_outputPWMParam buzzerTimer = {0};

void config_Buzzer(){
    /*
     * Timer A0 -> Buzzer
     */
    //Generate PWM - Timer runs in Up mode
    //TA0.4 -> P1.5
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    buzzerTimer.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    buzzerTimer.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    buzzerTimer.timerPeriod = 15625/20;
    buzzerTimer.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
    buzzerTimer.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    buzzerTimer.dutyCycle = buzzerTimer.timerPeriod / 2;
    //Timer_A_outputPWM(TIMER_A0_BASE, &buzzerTimer);
}

void set_BuzzerOn(){
    Timer_A_stop(TIMER_A0_BASE);
    buzzerTimer.timerPeriod = 32768 / FREQ_BUZZER;
    buzzerTimer.dutyCycle = buzzerTimer.timerPeriod / 2;
    Timer_A_outputPWM(TIMER_A0_BASE, &buzzerTimer);
}

void set_BuzzerOff(){
    buzzerTimer.dutyCycle = 0;
    Timer_A_outputPWM(TIMER_A0_BASE, &buzzerTimer);
    Timer_A_stop(TIMER_A0_BASE);
}

