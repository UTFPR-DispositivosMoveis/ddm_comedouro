/*
 * port_HD44780.c
 *
 *  Created on: 3 de out de 2018
 *      Author: Raul Scarmocin (raulsfreitas@alunos.utfpr.edu.br)
 *
 *      HD44780 faz uso das seguintes funções:
 *
 *      (altere as seguintes funções genéricas para tornar funcional no microcontrolador desejado)
 *
 *      pinMode(porta, pino, modo)  // Altera o pino de uma porta para modo OUTPUT (saida) ou INPUT (entrada)
 *      digitalWrite(porta, pino, set) // Escreve no pino da porta como nivel alto (HIGH) ou baixo (LOW)
 *      digitalRead(porta, pino)    // Lê o nível lógico da porta, retornando HIGH ou LOW
 *      delayMicroseconds(delay)    // faz uso do timer para causar um delay BLOQUEANTE em microsegundos
 *
 *      (as seguintes não precisa alterar)
 *
 *      delayMilliseconds(delay)    // faz uso de delayMicroseconds() para causar um delay em millisegundos
 *      delayMicrosecondszz(delay)  //delay em microseconds com interrupção (não utilizado)
 *      delayMillisecondszz(delay)    // faz uso de delayMicrosecondszz() para causar um delay em millisegundos
 */

#include "port_HD44780.h"
#include "driverlib.h"

void pinMode(unsigned int display_port, unsigned int pin, unsigned int isOutput){
    if(isOutput)
        GPIO_setAsOutputPin(display_port, pin);
    else
        GPIO_setAsInputPin(display_port, pin);
}

void digitalWrite(unsigned int display_port, unsigned int pin, unsigned int setOrReset){
    if (setOrReset)
        GPIO_setOutputHighOnPin(display_port, pin);
    else
        GPIO_setOutputLowOnPin(display_port, pin);
}

inline int digitalRead(unsigned int display_port, unsigned int pin){
    return GPIO_getInputPinValue(display_port, pin) > 0;
}




void delayMicroseconds(unsigned int time){
/*
    TA0CCR0 = time-1; // Upper limit of count for TAR
    TA0CTL = MC_1|ID_0|TASSEL_2|TACLR; // Set up and start Timer A
    while ((TA0CTL & TAIFG) == 0){ // Wait for overflow
    } // doing nothing
    TA0CTL &= (~TAIFG); // Clear overflow flag
*/

    TB0CCR0 = time-1; // Upper limit of count for TAR
    TB0CTL = MC_1|ID_0|TBSSEL_2|TACLR; // Set up and start Timer B
    while (!(TB0CTL & TBIFG));
    TB0CTL &= (~TBIFG); // Clear overflow flag

}
/*
void delayMicrosecondszz(unsigned int time){
    TA0CCR0 = time; // Upper limit of count for TAR
    TA0CTL = MC_1|ID_0|TASSEL_2|TACLR; // Set up and start Timer A
}

void delayMillisecondszz(unsigned int time){
    delayMicrosecondszz(1000 * time);
}
*/
inline void delayMilliseconds(unsigned int delay){

    while(delay > 60){
        delayMicroseconds(60000);
        delay -= 60;
    }
    if (delay) delayMicroseconds((delay << 10) - (delay << 4) - (delay << 3));

}
