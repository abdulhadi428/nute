/*
 * ManyLed.cpp
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#include "ManyLed.h"

PwmPin_t RedLed, GreenLed, YellowLed, BlueLed, WhiteLed;

void LedsInit() {
    RedLed.Init   (GPIOB, 9, 4, 4, LED_TOP);
    GreenLed.Init (GPIOB, 7, 4, 2, LED_TOP);
    YellowLed.Init(GPIOB, 8, 4, 3, LED_TOP);
    BlueLed.Init  (GPIOA, 8, 1, 1, LED_TOP);
    WhiteLed.Init (GPIOB, 6, 4, 1, LED_TOP);
}


