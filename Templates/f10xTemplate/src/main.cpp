/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "led.h"
#include "kl_lib.h"

int main(void) {
    Delay.Init();
    Led_t LedG, LedB;
    LedG.Init(GPIOC, 9);
    LedB.Init(GPIOC, 8);

    klPin_t Key;
    Key.Init(GPIOA, 0, GPIO_Mode_IN_FLOATING);

    while (1) {
        LedG.On();
        Delay.ms(45);
        LedG.Off();
        Delay.ms(45);

        if (Key == 1) LedB.On();
        else LedB.Off();
    } // while 1
}
