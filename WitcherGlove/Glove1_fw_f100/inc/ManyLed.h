/*
 * ManyLed.h
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#ifndef MANYLED_H_
#define MANYLED_H_

#include "kl_lib_f100.h"

#define LED_PRESCALER   4000
#define LED_TOP         1000
#define LED_VALUE       (LED_TOP/2)

extern PwmPin_t RedLed, GreenLed, YellowLed, BlueLed, UvLed;

void LedsInit();

#endif /* MANYLED_H_ */
