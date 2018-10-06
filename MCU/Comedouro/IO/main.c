#include "driverlib.h"
#include "Config_IO.h"
#include <stdio.h>


volatile Calendar newTime;

#define FREQ_MOTOR 50
#define DUTY_CYCLE  8 //Varia entre 0 e 100%
uint16_t FREQ_SOM = 20;

uint16_t TIMER_PERIOD_MOTOR = 32768 / FREQ_MOTOR;
uint16_t DUTYv = 0;

uint16_t TIMER_PERIOD_BUZZER = 32768 / 10;

Timer_A_outputPWMParam param = {0}; //Motor
Timer_A_outputPWMParam param2 = {0}; //Buzzer
Timer_A_initContinuousModeParam paramUP = {0}; //TimerA0 p/ ultrassonico

uint32_t ACLK, MCLK, SMCLK;
int cont =0, FlagMedir=0;
uint16_t upCounter = 0;
uint16_t distancia = 0;
int leitura=0;
int NivelRacao = 0; //Altura � de 60cm (1-> 4a15cm, 2->15a30, 3->30a45, 4-> 45-55, 5-> >55)
int dis;
int echo_pulse_duration;

void getClockFreq(){
    ACLK = UCS_getACLK();
    SMCLK = UCS_getSMCLK();
    MCLK = UCS_getMCLK();
}

void Girar_Motor(){

    Timer_A_stop(TIMER_A1_BASE);

    if (DUTYv==0)
        DUTYv = 26; //4% duty

    else if (DUTYv==26) //4% duty
        DUTYv = 52; //8% duty

    else if (DUTYv==52)
        DUTYv = 26;

    param.dutyCycle = DUTYv;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);
}

void set_BuzzerOn(){
    Timer_A_stop(TIMER_A2_BASE);
    TIMER_PERIOD_BUZZER = 32768 / FREQ_SOM;
    param2.timerPeriod = TIMER_PERIOD_BUZZER;
    param2.dutyCycle = param2.timerPeriod / 2;
    Timer_A_outputPWM(TIMER_A2_BASE, &param2);
}

void set_BuzzerOff(){
    Timer_A_stop(TIMER_A2_BASE);
}

int get_Distance(){
    /*
     * Gera o pulso de 10 uS
     */
    GPIO_setOutputHighOnPin (GPIO_PORT_P4, GPIO_PIN3);
    __delay_cycles (264);
    GPIO_setOutputLowOnPin (GPIO_PORT_P4, GPIO_PIN3);

    while ((P1IN & BIT2)==0);   // Wait for start of echo pulse

    echo_pulse_duration = Timer_A_getCounterValue (TIMER_A0_BASE);

    while ((P1IN & BIT2)>0);    // Wait for end of echo pulse

    if(Timer_A_getCounterValue (TIMER_A0_BASE) > echo_pulse_duration)
        echo_pulse_duration = Timer_A_getCounterValue (TIMER_A0_BASE) - echo_pulse_duration;
    else
        echo_pulse_duration = 65535 - echo_pulse_duration + Timer_A_getCounterValue (TIMER_A0_BASE);

    //retorna distancia em mm
    return 0.017 * echo_pulse_duration;

}


void main (void){
   WDT_A_hold(WDT_A_BASE);
    Config_BCS(24000);
    getClockFreq ();
    Config_TimerA ();
    Config_RTC ();
    while(true){
        //dis = Mede_Distancia ();
        //fprintf("Distancia: %d", &dis);
        __bis_SR_register(LPM0_bits + GIE);
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RTC_VECTOR)))
#endif
void RTC_A_ISR (void)
{
    switch (__even_in_range(RTCIV,16)){
        case 0: break;  //No interrupts
        case 2:         //RTCRDYIFG
            //Toggle P1.0 every second
            dis = get_Distance();
            GPIO_toggleOutputOnPin(
            GPIO_PORT_P1,
            GPIO_PIN0);
            /*Girar_Motor();
            if (cont == 0)
            {
                Liga_Buzzer ();
                cont =1;
            }
            else
            {
                Desliga_Buzzer ();
                cont=0;
            }*/
            break;
        case 4:         //RTCEVIFG
            //Interrupts every minute
            __no_operation();

            //Read out New Time a Minute Later BREAKPOINT HERE
            newTime = RTC_A_getCalendarTime(RTC_A_BASE);
            break;
        case 6:         //RTCAIFG
            //Interrupts 5:00pm on 5th day of week
            __no_operation();
            break;
        case 8: break;  //RT0PSIFG
        case 10: break; //RT1PSIFG
        case 12: break; //Reserved
        case 14: break; //Reserved
        case 16: break; //Reserved
        default: break;
    }
}
