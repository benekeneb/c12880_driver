/*
 * ads1115.h
 *
 *  Created on: Jan 26, 2023
 *      Author: root
 */

#ifndef INC_ADS1115_H_
#define INC_ADS1115_H_

#include "stm32f3xx_hal.h"

int16_t adcRead();
int16_t adcReadVoltage();

#endif /* INC_ADS1115_H_ */
