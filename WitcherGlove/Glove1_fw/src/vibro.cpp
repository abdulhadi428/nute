/*
 * vibro.cpp
 *
 *  Created on: 18.09.2012
 *      Author: kreyl
 */

#include "vibro.h"

PwmPin_t Vibro;

void VibroInit() {
    Vibro.Init(GPIOB, 3, 2, 2, VIBRO_TOP);
}
