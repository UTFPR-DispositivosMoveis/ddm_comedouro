/*
 * main.c
 *
 *  Created on: 21 de set de 2018
 *      Author: raul
 */

#include    "driverlib.h"
#include    "Display/Display.h"
#include    "IO/Config_IO.h"
#include    "IO/Motor/Motor.h"
#include    "IO/SR04/SR04.h"
#include    <stdio.h>

HD44780 theHD44780;

Timer_B_initContinuousModeParam param = {0};

uint8_t state = 0;
Display_Interface_t display;


unsigned int dist;

void Config_BCS (uint16_t FREQ_MCLK_KHZ){
    if (FREQ_MCLK_KHZ >= 8000 && FREQ_MCLK_KHZ < 12000)
        PMM_setVCore(PMM_CORE_LEVEL_1);
    else if (FREQ_MCLK_KHZ >= 12000 && FREQ_MCLK_KHZ < 20000)
        PMM_setVCore(PMM_CORE_LEVEL_2);
    else if (FREQ_MCLK_KHZ >= 20000 && FREQ_MCLK_KHZ <= 25000)
        PMM_setVCore(PMM_CORE_LEVEL_3);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN2); //XT1IN + XT2IN
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN5 | GPIO_PIN3); //XT1OUT + XT2OUT
    UCS_setExternalClockSource(32768, 4000000); //Configurando frequência dos cristais
    UCS_turnOnLFXT1 (UCS_XT1_DRIVE_0, UCS_XCAP_3); //Ligando XT1
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ); //Ligando XT2

    UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_4); //FLL = 1MHZ
    uint16_t MCLK_FLLREF_RATIO = ((FREQ_MCLK_KHZ)/(1000)); //Frequência do sistema em KHz / FLL em KHz
    UCS_initFLLSettle(FREQ_MCLK_KHZ, MCLK_FLLREF_RATIO); //Configurando DCO
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1); //MCLK = DCO
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_2); //SMCLK = DCO/4
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1); //ACLK = XT1CLK
}

void main(void){
    WDT_A_hold(WDT_A_BASE);

    HD44780_init(&theHD44780, DISPLAY_PORT, DISPLAY_PIN_RS, DISPLAY_PIN_EN,
                 DISPLAY_PIN_D4, DISPLAY_PIN_D5, DISPLAY_PIN_D6, DISPLAY_PIN_D7);

    HD44780_begin(&theHD44780, 16, 2);

    initStateMachine(&display);

    initDisplay(&display);

    displayUpdate(&display, &theHD44780);

    Display_Config();

    WDT_A_hold(WDT_A_BASE);

    config_BCS(24000);

    config_IO();

    set_MotorAngle(0);

    __delay_cycles(24000000);

    __enable_interrupt();

    while(1){
        __bis_SR_register(LPM0_bits + GIE);
    }


    __no_operation();

}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=BUTTON_PORT_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(BUTTON_PORT_VECTOR)))
#endif
void Port_1 (void){



    __delay_cycles(50000);

    exeStateMachine(&display, getButtonFlag());

    displayUpdate(&display, &theHD44780);

    GPIO_clearInterrupt(BUTTON_PORT, PREV_PIN | NEXT_PIN | INC_PIN | DEC_PIN | OK_PIN);

}

