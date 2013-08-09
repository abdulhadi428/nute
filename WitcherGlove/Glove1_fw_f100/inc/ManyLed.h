/*
 * ManyLed.h
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#ifndef MANYLED_H_
#define MANYLED_H_

#include "kl_lib_f100.h"

#define LED_TOP         100
#define LED_LOW         4

extern PwmPin_t RedLed, GreenLed, YellowLed, BlueLed, WhiteLed;

void LedsInit();

#endif /* MANYLED_H_ */
