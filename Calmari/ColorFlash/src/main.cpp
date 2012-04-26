/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "kl_util.h"
#include "kl_lib.h"
#include "led.h"

int main(void) {
    Led_t Led(GPIOB, 6);

    Led.On();

    Delay.ms(144);

    Led.Off();

    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div512);
    SystemCoreClockUpdate();

    while (1);
}

