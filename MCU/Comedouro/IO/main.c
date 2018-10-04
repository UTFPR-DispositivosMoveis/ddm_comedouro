#include "driverlib.h"
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
int NivelRacao = 0; //Altura é de 60cm (1-> 4a15cm, 2->15a30, 3->30a45, 4-> 45-55, 5-> >55)
int dis;
int echo_pulse_duration;

void Config_BCS (uint16_t FREQ_MCLK_KHZ)
{
    //Ajustando PMM
    if (FREQ_MCLK_KHZ >= 8000 && FREQ_MCLK_KHZ < 12000)
    {
        PMM_setVCore(PMM_CORE_LEVEL_1);
    }
    else if (FREQ_MCLK_KHZ >= 12000 && FREQ_MCLK_KHZ < 20000)
    {
        PMM_setVCore(PMM_CORE_LEVEL_2);
    }
    else if (FREQ_MCLK_KHZ >= 20000 && FREQ_MCLK_KHZ <= 25000)
    {
        PMM_setVCore(PMM_CORE_LEVEL_3);
    }
    /*
     * Cofiguração cristais:
     * XT1 - 32768 Hz para ser utilizado pelo ACLK
     * XT2 - 4 MHz para ser utilizado pelo FLL que ajustará a frequência do DCO
     * Ligações:
     * XT1: IN  P5.4
     *      OUT P5.5
     * XT2: IN  P5.2
     *      OUT P5.3
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN2); //XT1IN + XT2IN
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN5 | GPIO_PIN3); //XT1OUT + XT2OUT
    UCS_setExternalClockSource(32768, 4000000); //Configurando frequência dos cristais
    UCS_turnOnLFXT1 (UCS_XT1_DRIVE_0, UCS_XCAP_3); //Ligando XT1
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ); //Ligando XT2
    /*
     * Configuração Clocks:
     * DCO = MCLK
     * SMCLK = XT2CLK/4 = 1MHz
     * ACLK = XT1CLK = 32768
     */
    UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_4); //FLL = 1MHZ
    uint16_t MCLK_FLLREF_RATIO = ((FREQ_MCLK_KHZ)/(1000)); //Frequência do sistema em KHz / FLL em KHz
    UCS_initFLLSettle(FREQ_MCLK_KHZ, MCLK_FLLREF_RATIO); //Configurando DCO
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1); //MCLK = 24MHz
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_4); //SMCLK = 1MHz
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1); //ACLK = XT1CLK = 32KHz
}

void Config_TimerA ()
{
    GPIO_setAsInputPin (GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsOutputPin (GPIO_PORT_P4, GPIO_PIN3);
    /*
     * Timer A1 -> Motor
     */
    //TA1.1 -> P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    //Generate PWM - Timer runs in Up mode
    param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = TIMER_PERIOD_MOTOR;
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    param.dutyCycle = 26;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);
    /*
     * Timer A2 -> Buzzer
     */
    //Generate PWM - Timer runs in Up mode
    //TA2.2 -> P2.5
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    param2.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param2.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param2.timerPeriod = TIMER_PERIOD_BUZZER;
    param2.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
    param2.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    param2.dutyCycle = param2.timerPeriod / 2;
    //Timer_A_outputPWM(TIMER_A2_BASE, &param2);
    /*
     * Timer A0 -> Clock = SMLCK = 1MHz
     * P4.3 -> Gera Pulso para o sensor ultrassonico
     * P1.2 -> Captura o tempo de resposta do sensor para determinar a distância
     */
    paramUP.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    paramUP.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    paramUP.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    paramUP.timerClear = TIMER_A_DO_CLEAR;
    paramUP.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A0_BASE, &paramUP);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);

}

void getClockFreq ()
{
    ACLK = UCS_getACLK();
    SMCLK = UCS_getSMCLK();
    MCLK = UCS_getMCLK();
}

void Girar_Motor ()
{
    Timer_A_stop(TIMER_A1_BASE);
    if (DUTYv==0)
    {
        DUTYv = 26; //4% duty
    }
    else if (DUTYv==26) //4% duty
    {
        DUTYv = 52; //8% duty
    }
    else if (DUTYv==52)
    {
        DUTYv = 26;
    }
    param.dutyCycle = DUTYv;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);
}

void Liga_Buzzer ()
{
    Timer_A_stop(TIMER_A2_BASE);
    TIMER_PERIOD_BUZZER = 32768 / FREQ_SOM;
    param2.timerPeriod = TIMER_PERIOD_BUZZER;
    param2.dutyCycle = param2.timerPeriod / 2;
    Timer_A_outputPWM(TIMER_A2_BASE, &param2);
}

void Desliga_Buzzer ()
{
    Timer_A_stop(TIMER_A2_BASE);
}

int Mede_Distancia ()
{
    int d=0;
    /*
     * Gera o pulso de 10 uS
     */
    GPIO_setOutputHighOnPin (GPIO_PORT_P4, GPIO_PIN3);
    __delay_cycles (264);
    GPIO_setOutputLowOnPin (GPIO_PORT_P4, GPIO_PIN3);

    while ((P1IN & BIT2)==0);   // Wait for start of echo pulse
    echo_pulse_duration = Timer_A_getCounterValue (TIMER_A0_BASE);

    while ((P1IN & BIT2)>0);    // Wait for end of echo pulse
    if (Timer_A_getCounterValue (TIMER_A0_BASE) > echo_pulse_duration)
    {
        echo_pulse_duration = Timer_A_getCounterValue (TIMER_A0_BASE) - echo_pulse_duration;
    }
    else
    {
        echo_pulse_duration = 65535 - echo_pulse_duration + Timer_A_getCounterValue (TIMER_A0_BASE);
    }
    distancia = 0.017 * echo_pulse_duration; // Convert from us to cm
    if (distancia < 15)
    {
        d = 1;
    }
    else if (distancia >= 15 && distancia < 30)
    {
        d = 2;
    }
    else if (distancia >= 30 && distancia < 45)
    {
        d = 3;
    }
    else if (distancia >= 45 && distancia < 55)
    {
        d = 4;
    }
    else if (distancia >= 55)
    {
        d = 5;
    }
    distancia=0;
    return d;
}
void Condig_RTC ()
{
    Calendar currentTime;

        //Set P1.0 to output direction
        GPIO_setAsOutputPin(
            GPIO_PORT_P1,
            GPIO_PIN0
            );


        // Select XT1
        GPIO_setAsPeripheralModuleFunctionInputPin(
                GPIO_PORT_P5,
                GPIO_PIN4 + GPIO_PIN5
                );


        //Initialize LFXT1
        UCS_turnOnLFXT1(UCS_XT1_DRIVE_3,
            UCS_XCAP_3
            );

        //Setup Current Time for Calendar
        currentTime.Seconds    = 0x00;
        currentTime.Minutes    = 0x55;
        currentTime.Hours      = 0x14;
        currentTime.DayOfWeek  = 0x4;
        currentTime.DayOfMonth = 0x5;
        currentTime.Month      = 0x9;
        currentTime.Year       = 0x2017;

        //Initialize Calendar Mode of RTC
        /*
         * Base Address of the RTC_A_A
         * Pass in current time, intialized above
         * Use BCD as Calendar Register Format
         */
        RTC_A_initCalendar(RTC_A_BASE,
            &currentTime,
            RTC_A_FORMAT_BCD);

        //Setup Calendar Alarm for 5:00pm on the 5th day of the week.
        //Note: Does not specify day of the week.
        RTC_A_configureCalendarAlarmParam param = {0};
        param.minutesAlarm = 0x00;
        param.hoursAlarm = 0x17;
        param.dayOfWeekAlarm = RTC_A_ALARMCONDITION_OFF;
        param.dayOfMonthAlarm = 0x05;
        RTC_A_configureCalendarAlarm(RTC_A_BASE, &param);

        //Specify an interrupt to assert every minute
        RTC_A_setCalendarEvent(RTC_A_BASE,
            RTC_A_CALENDAREVENT_MINUTECHANGE);

        //Enable interrupt for RTC Ready Status, which asserts when the RTC
        //Calendar registers are ready to read.
        //Also, enable interrupts for the Calendar alarm and Calendar event.
        RTC_A_clearInterrupt(RTC_A_BASE,
            RTCRDYIFG + RTCTEVIFG + RTCAIFG);
        RTC_A_enableInterrupt(RTC_A_BASE,
            RTCRDYIE + RTCTEVIE + RTCAIE);

        //Start RTC Clock
        RTC_A_startClock(RTC_A_BASE);

}

void main (void)
{
   WDT_A_hold(WDT_A_BASE);
    Config_BCS(24000);
    getClockFreq ();
    Config_TimerA ();
    Condig_RTC ();
    while (true)
    {
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
            dis = Mede_Distancia ();
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
