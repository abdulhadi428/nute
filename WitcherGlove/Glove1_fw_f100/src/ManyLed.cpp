/*
 * ManyLed.cpp
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#include "ManyLed.h"

PwmPin_t RedLed, GreenLed, YellowLed, BlueLed, UvLed;

void LedsInit() {
    RedLed.Init   (GPIOB, 9, 4, 4, LED_TOP);
    RedLed.SetPrescaler(LED_PRESCALER);
    GreenLed.Init (GPIOB, 7, 4, 2, LED_TOP);
    GreenLed.SetPrescaler(LED_PRESCALER);
    YellowLed.Init(GPIOB, 8, 4, 3, LED_TOP);
    YellowLed.SetPrescaler(LED_PRESCALER);
    BlueLed.Init  (GPIOA, 8, 1, 1, LED_TOP);
    BlueLed.SetPrescaler(LED_PRESCALER);
    UvLed.Init    (GPIOB, 6, 4, 1, LED_TOP);
    UvLed.SetPrescaler(LED_PRESCALER);
}


