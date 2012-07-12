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
    Led_t Led;
    Led.Init(GPIOB, 10);

    while (1) {
        Led.On();
        Delay.ms(450);
        Led.Off();
        Delay.ms(450);
    } // while 1
}
