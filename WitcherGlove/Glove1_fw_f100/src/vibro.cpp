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
    Tim2Remap(T2_PA15_PB3_PA2_PA3);
}
