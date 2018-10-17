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
     * Timer A2 -> Buzzer
     */
    //Generate PWM - Timer runs in Up mode
    //TA2.2 -> P2.5
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    buzzerTimer.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    buzzerTimer.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    buzzerTimer.timerPeriod = 32768/FREQ_BUZZER;
    buzzerTimer.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
    buzzerTimer.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    buzzerTimer.dutyCycle = buzzerTimer.timerPeriod / 2;
    //Timer_A_outputPWM(TIMER_A2_BASE, &buzzerTimer);
}

void set_BuzzerOn(){
    Timer_A_stop(TIMER_A2_BASE);
    buzzerTimer.timerPeriod = 32768 / FREQ_BUZZER;
    buzzerTimer.dutyCycle = buzzerTimer.timerPeriod / 2;
    Timer_A_outputPWM(TIMER_A2_BASE, &buzzerTimer);
}

void set_BuzzerOff(){
    //buzzerTimer.dutyCycle = 0;
    //Timer_A_outputPWM(TIMER_A2_BASE, &buzzerTimer);
    Timer_A_stop(TIMER_A2_BASE);
}

