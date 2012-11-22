/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "led.h"
#include "kl_lib.h"
#include "stm32f10x_gpio.h"

#include "lcd6101.h"

int main(void) {
    Uart.Init(115200);
    Uart.Printf("Lcd\r");

    Lcd.Init();


    while(1) {
        Uart.Task();

    }
}

/*
 *     klPinSetup(GPIOC, 8, pmOutAFPushPull);

    //AFIO->MAPR &= ~0x00000C00;
    AFIO->MAPR |=  0x00000C00;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM3->CR1 = 0x01;
    TIM3->ARR = 100;
    TIM3->PSC = 0;
    TIM3->CCMR2 |= 0x0060;
    TIM3->CCER  |= 0x0100;
    TIM3->CCR3 = 54;
 *
 */
