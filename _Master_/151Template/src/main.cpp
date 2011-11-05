/*
 * main.cpp
 *
 *  Created on: 27.08.2011
 *      Author: Kreyl
 */

#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "delay_util.h"
#include "main.h"
#include "stm32l1xx_exti.h"
#include "misc.h"
#include "klprintf.h"

int main(void) {
    Delay.Init();
    klPrintf_Init();

    uint32_t tmr;

    while (1) {
        if(Delay.Elapsed(&tmr, 400))
            klPrintf("aga\r");

    }
}
