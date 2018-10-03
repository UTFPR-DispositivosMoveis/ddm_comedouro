/*
 * port_HD44780.h
 *
 *  Created on: 3 de out de 2018
 *      Author: raul
 */

#ifndef DISPLAY_HD44780_PORT_HD44780_H_
#define DISPLAY_HD44780_PORT_HD44780_H_

//------------------------------------------------------------------------------
// Comando de digital I/O
void pinMode(unsigned int display_port, unsigned int pin, unsigned int isOutput);
//------------------------------------------------------------------------------
void digitalWrite(unsigned int display_port, unsigned int pin, unsigned int setOrReset);
//------------------------------------------------------------------------------
inline int digitalRead(unsigned int display_port, unsigned int pin);
//------------------------------------------------------------------------------

// Fun��es que fazem temporiza��es, baseadas nos Timers do MSP430
//------------------------------------------------------------------------------
void delayMicroseconds(unsigned int time);
//------------------------------------------------------------------------------
void delayMicrosecondszz(unsigned int time);
//------------------------------------------------------------------------------
inline void delayMilliseconds(unsigned int delay);
//------------------------------------------------------------------------------
void delayMillisecondszz(unsigned int time);
//------------------------------------------------------------------------------


#endif /* DISPLAY_HD44780_PORT_HD44780_H_ */
