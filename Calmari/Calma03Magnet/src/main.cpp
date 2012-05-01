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
    // ==== Init ====
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div8);
    SystemCoreClockUpdate();
    Delay.Init();

    // Peripheral
    LedSmooth_t Led;
    Led.Init(GPIOB, 6, TIM4, 250, 0, 1, false);

    Led.On();
    Delay.ms(144);
    Led.Off();

    Delay.ms(1800);
    Led.Set(1);

    // ==== Main cycle ====
    //uint32_t Tmr;
    while (1) {
        Led.Task();
    }
}

