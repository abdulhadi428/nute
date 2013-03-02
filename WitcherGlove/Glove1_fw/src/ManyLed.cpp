/*
 * ManyLed.cpp
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#include "ManyLed.h"

Pin_t RedLed    = { GPIOB, 9 };
Pin_t GreenLed  = { GPIOB, 7 };
Pin_t YellowLed = { GPIOB, 8 };
Pin_t BlueLed   = { GPIOA, 8 };
Pin_t UvLed     = { GPIOB, 6 };

void LedsInit() {
    RedLed.SetupOut();
    GreenLed.SetupOut();
    YellowLed.SetupOut();
    BlueLed.SetupOut();
    UvLed.SetupOut();
}

