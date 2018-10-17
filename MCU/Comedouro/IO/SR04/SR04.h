/*
 * SR04.h
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */

#ifndef IO_SR04_H_
#define IO_SR04_H_

#define SR04_TRIG_PORT  GPIO_PORT_P4
#define SR04_TRIG_PIN   GPIO_PIN3
#define SR04_ECHO_PORT  GPIO_PORT_P1
#define SR04_ECHO_PIN   GPIO_PIN2

void config_SR04();
unsigned int get_Distance();


#endif /* IO_SR04_H_ */
