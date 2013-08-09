/*
 * vibro.cpp
 *
 *  Created on: 18.09.2012
 *      Author: kreyl
 */

#include "vibro.h"

#define VIBRO_TOP   100

PwmPin_t Vibro;

void VibroInit() {
    Vibro.Init(GPIOB, 3, 2, 2, VIBRO_TOP);
//    uint32_t divider = VIBRO_TOP * 180; // 171 Hz is rezonant for iPhone4S vibro
//    uint32_t FPrescaler = Clk.APB1FreqHz / divider;
//    if(FPrescaler != 0) FPrescaler--;   // do not decrease in case of high freq
//    Vibro.SetPrescaler(FPrescaler);
    Tim2Remap(T2_PA15_PB3_PA2_PA3);
}

