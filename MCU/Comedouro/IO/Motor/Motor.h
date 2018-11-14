/*
 * Motor.h
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */

#ifndef IO_MOTOR_H_
#define IO_MOTOR_H_


#define MOTOR_PORT  GPIO_PORT_P2
#define MOTOR_PIN   GPIO_PIN5

#define FREQ_MOTOR  50
#define DUTY_CYCLE  8 //Varia entre 0 e 100%




void config_Motor();
void set_MotorAngle(int angle);


#endif /* IO_MOTOR_H_ */
