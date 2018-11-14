/*
 * Motor.c
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */

#include    <driverlib.h>
#include    "Motor.h"

Timer_A_outputPWMParam motorTimer = {0};

void config_Motor(){
    /*
     * Timer A1 -> Motor
     */
    //TA2.2 -> P2.5
    GPIO_setAsPeripheralModuleFunctionOutputPin(MOTOR_PORT, MOTOR_PIN);
    //Generate PWM - Timer runs in Up mode
    motorTimer.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    motorTimer.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    motorTimer.timerPeriod = 32768 / FREQ_MOTOR;
    motorTimer.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
    motorTimer.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    motorTimer.dutyCycle = 26;
    Timer_A_outputPWM(TIMER_A2_BASE, &motorTimer);
}

void set_MotorAngle(int angle){

    static uint8_t angl;

    Timer_A_stop(TIMER_A2_BASE);

    if(angle < 0)
        angle = 0;

    if(angle > 180)
        angle = 180;

    angl = angle;

/*
    regra de 3 para setar o angulo
    por tentativa e erro, 0° = 4% e 180° = 11%
*/
    motorTimer.dutyCycle = (uint16_t)(motorTimer.timerPeriod * (0.04f + 7 * (float)angl / 18000));  //(0.04 + 7 * (float)angle / 18000)

    Timer_A_outputPWM(TIMER_A2_BASE, &motorTimer);
}


/*

void Girar_Motor(){

    Timer_A_stop(TIMER_A1_BASE);

    if (DUTYv==0)
        DUTYv = 26; //4% duty

    else if (DUTYv==26) //4% duty
        DUTYv = 52; //8% duty

    else if (DUTYv==52)
        DUTYv = 26;

    motorTimer.dutyCycle = DUTYv;
    Timer_A_outputPWM(TIMER_A1_BASE, &motorTimer);
}
*/
