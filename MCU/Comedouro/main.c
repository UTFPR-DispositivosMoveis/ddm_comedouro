#include "driverlib.h"


volatile Calendar newTime;

#define FREQ_MOTOR 50
#define DUTY_CYCLE  8 //Varia entre 0 e 100%

uint16_t TIMER_PERIOD = 32768 / FREQ_MOTOR;
uint16_t DUTYv = 0;
Timer_A_outputPWMParam param = {0};

uint32_t ACLK, MCLK, SMCLK;

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
     * SMCLK = MCLK/2
     * ACLK = XT1CLK
     */
    UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_4); //FLL = 1MHZ
    uint16_t MCLK_FLLREF_RATIO = ((FREQ_MCLK_KHZ)/(1000)); //Frequência do sistema em KHz / FLL em KHz
    UCS_initFLLSettle(FREQ_MCLK_KHZ, MCLK_FLLREF_RATIO); //Configurando DCO
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1); //MCLK = DCO
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_2); //SMCLK = DCO/4
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1); //ACLK = XT1CLK
}

void Config_GPIO ()
{
    //P2.0 as PWM output
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0);
}

void Config_TimerA ()
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    //Generate PWM - Timer runs in Up mode
    param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = TIMER_PERIOD;
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    param.dutyCycle = 26;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);
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
    Config_GPIO ();
    Config_TimerA ();
    Condig_RTC ();
    while (true)
    {
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
            GPIO_toggleOutputOnPin(
            GPIO_PORT_P4,
            GPIO_PIN7);
            Girar_Motor();
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

