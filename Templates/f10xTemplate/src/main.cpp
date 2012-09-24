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
    RCC->APB2ENR |= 0x08;
    GPIOB->CRH = 0x00300000;
    GPIOB->ODR = 0x2000;

    while(1);
}

//klGpioSetupByN(GPIOB, 13, GPIO_Mode_Out_PP);
//klGpioSetByN(GPIOB, 13);


/*
 *    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 *
 *     Delay.Init();
    Led_t Led;
    Led.Init(GPIOB, 10);

    while (1) {
        Led.On();
        Delay.ms(450);
        Led.Off();
        Delay.ms(450);
    } // while 1
 */
