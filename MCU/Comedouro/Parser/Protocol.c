/*
 * Protocol.c
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */
#include "Protocol.h"
#include "driverlib.h"
#include "Global_Var.h"
#include "IO/Config_IO.h"

#define BAUD_RATE   9600
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
uint8_t posString = 0;
uint8_t estadoString = 0;
USCI_A_UART_initParam paramUART = {0};
unsigned char Comando [4];

unsigned int nivel_dist;
short int hora_alarme1;
short int hora_alarme2;
short int hora_alarme3;
unsigned short int tempo_motor;
unsigned char buzzer;

int config_UART ()
{
    //P3.3 -> UCA0TXD
    //P3.4 -> UCA0RXD
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN3 + GPIO_PIN4);
    //Baudrate = 9600, clock freq = 1.048MHz
    //UCBRx = 109, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
    paramUART.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
    paramUART.clockPrescalar = 109;
    paramUART.firstModReg = 0;
    paramUART.secondModReg = 2;
    paramUART.parity = USCI_A_UART_NO_PARITY;
    paramUART.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    paramUART.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    paramUART.uartMode = USCI_A_UART_MODE;
    paramUART.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
    if (STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &paramUART))
    {
        return -1;
    }
    //Enable UART module for operation
    USCI_A_UART_enable(USCI_A0_BASE);
    //Enable Receive Interrupt
    USCI_A_UART_clearInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    return 1;
}
void UART_get_status ()
{
    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) nivel_dist );
    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) ' ');

    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) hora_alarme1);
    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) ' ');

    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) hora_alarme2);
    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) ' ');

    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) hora_alarme3);
    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) ' ');

    USCI_A_UART_transmitData(USCI_A0_BASE, (uint8_t) tempo_motor);
    USCI_A_UART_transmitData(USCI_A0_BASE, ';');
}
void UART_Alarme ()
{
    short int temp = (short int) Comando [1];
    if (Comando[2] == '1')
    {
        hora_alarme1 = temp*100;
    }
    else if (Comando[2] == '2')
    {
        hora_alarme2 = temp*100;
    }
    else if (Comando[2] == '3')
    {
        hora_alarme3 = temp*100;
    }
    USCI_A_UART_transmitData(USCI_A0_BASE, 1);
}
void UART_Manual ()
{
    short int temp = (short int) Comando [1];

    Abrir_Valvula (temp);

    USCI_A_UART_transmitData(USCI_A0_BASE, 1);
}
void UART_Buzzer ()
{
    if (Comando [1] == '1') //Ligar
    {
        buzzer = 1;
    }
    else if (Comando [1] == '2') //Desligar
    {
        buzzer = 0;
    }
    USCI_A_UART_transmitData(USCI_A0_BASE, 1);
}
void Parser_Comandos ()
{
    if (Comando[0] == '1') //getStatus
    {
        UART_get_status ();
    }
    else if (Comando[0] == '2')//Alarme
    {
        UART_Alarme ();
    }
    else if (Comando[0] == '3')//Manual
    {
        UART_Manual ();
    }
    else if (Comando[0] == '4')//Buzzer
    {
        UART_Buzzer ();
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR (void)
{
    switch (__even_in_range(UCA0IV,4))
    {
        //Vector 2 - RXIFG
        case 2:
            receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            Comando [posString] = receivedData;
            if (posString >= 3)
            {
                posString = 0;
            }
            else
            {
                posString++;
            }
            if (receivedData == ';')
            {
                Parser_Comandos ();
                posString = 0;
                int i=0;
                for (i=0;i<4;i++)
                {
                    Comando [i] = ' ';
                }
            }
            break;
        default:
            break;
    }
}
