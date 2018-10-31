/*
 * Config_IO.h
 *
 *  Created on: 6 de out de 2018
 *      Author: raul
 */

#ifndef IO_CONFIG_IO_H_
#define IO_CONFIG_IO_H_

void config_RTC();
void config_BCS(uint16_t FREQ_MCLK_KHZ);
void config_IO();
void Abrir_Valvula (short int tempo);

#endif /* IO_CONFIG_IO_H_ */
