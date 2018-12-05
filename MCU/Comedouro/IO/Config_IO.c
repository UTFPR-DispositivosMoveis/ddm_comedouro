/*
 * Config_IO.c
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */

#include    "driverlib.h"
#include    "Config_IO.h"
#include    "Motor/Motor.h"
#include    "SR04/SR04.h"
#include    "Buzzer/Buzzer.h"
#include    "Display/HD44780/port_HD44780.h"
#include    "Global_Var.h"

unsigned char buzzer;
unsigned short int hora_atual;
unsigned int nivel_dist;


void config_RTC(){
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
/*
        RTC_A_configureCalendarAlarmParam param = {0};
        param.minutesAlarm = 0x00;
        param.hoursAlarm = 0x17;
        param.dayOfWeekAlarm = RTC_A_ALARMCONDITION_OFF;
        param.dayOfMonthAlarm = 0x05;
        RTC_A_configureCalendarAlarm(RTC_A_BASE, &param);
*/
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


void config_BCS(uint16_t FREQ_MCLK_KHZ){
    //Ajustando PMM
    if (FREQ_MCLK_KHZ >= 8000 && FREQ_MCLK_KHZ < 12000)
        PMM_setVCore(PMM_CORE_LEVEL_1);
    else if (FREQ_MCLK_KHZ >= 12000 && FREQ_MCLK_KHZ < 20000)
        PMM_setVCore(PMM_CORE_LEVEL_2);
    else if (FREQ_MCLK_KHZ >= 20000 && FREQ_MCLK_KHZ <= 25000)
        PMM_setVCore(PMM_CORE_LEVEL_3);
    /*
     * Cofigura��o cristais:
     * XT1 - 32768 Hz para ser utilizado pelo ACLK
     * XT2 - 4 MHz para ser utilizado pelo FLL que ajustar� a frequ�ncia do DCO
     * Liga��es:
     * XT1: IN  P5.4
     *      OUT P5.5
     * XT2: IN  P5.2
     *      OUT P5.3
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN2); //XT1IN + XT2IN
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN5 | GPIO_PIN3); //XT1OUT + XT2OUT
    UCS_setExternalClockSource(32768, 4000000); //Configurando frequ�ncia dos cristais
    UCS_turnOnLFXT1 (UCS_XT1_DRIVE_0, UCS_XCAP_3); //Ligando XT1
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ); //Ligando XT2
    /*
     * Configura��o Clocks:
     * DCO = MCLK
     * SMCLK = XT2CLK/4 = 1MHz
     * ACLK = XT1CLK = 32768
     */
    UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_4); //FLL = 1MHZ
    uint16_t MCLK_FLLREF_RATIO = ((FREQ_MCLK_KHZ)/(1000)); //Frequ�ncia do sistema em KHz / FLL em KHz
    UCS_initFLLSettle(FREQ_MCLK_KHZ, MCLK_FLLREF_RATIO); //Configurando DCO
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1); //MCLK = 24MHz
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_4); //SMCLK = 1MHz
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1); //ACLK = XT1CLK = 32KHz
}

void config_IO(){
    config_RTC();
    config_Buzzer();
    config_Motor();
    config_SR04();

    buzzer = 1;
    hora_alarme1 = 0;
    hora_alarme2 = 800;
    hora_alarme3 = 1600;
    tempo_motor = 1000;
}

void Abrir_Valvula (short int tempo)
{
    if (buzzer == 1)
    {
        set_BuzzerOn();
    }
    delayMilliseconds(5000);

    set_BuzzerOff();

    set_MotorAngle(90);

    delayMilliseconds(tempo);

    set_MotorAngle(0);

    nivel_dist = 6 - (get_Distance() * 0.0017);
}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RTC_VECTOR)))
#endif
void RTC_A_ISR (void){
    switch (__even_in_range(RTCIV,16)){
        case 0: break;  //No interrupts
        case 2: break;  //RTCRDYIFG
        case 4:         //RTCEVIFG
            //Interrupts every minute
            hora_atual++;
            if(hora_atual % 100 >= 60){
                hora_atual = 0;
                hora_atual = (hora_atual + 100) % 2400;
            }
            if (hora_atual == hora_alarme1 || hora_atual == hora_alarme2 || hora_atual == hora_alarme3)
                Abrir_Valvula (tempo_motor);


            break;
        case 6: break;  //RTCAIFG
        case 8: break;  //RT0PSIFG
        case 10: break; //RT1PSIFG
        case 12: break; //Reserved
        case 14: break; //Reserved
        case 16: break; //Reserved
        default: break;
    }
}
