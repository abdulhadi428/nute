/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_L15x.h"
#include "stm32l1xx.h"

int main(void) {
    PinSetupOut(GPIOB, 0, omPushPull, pudNone);

    PinSetupAlterFunc(GPIOA, 9, omPushPull, pudNone, AF7);

    rccEnableUSART1(FALSE);
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE;
    USART1->BRR = 2400000 / 115200;

    while(1) {
        PinSet(GPIOB, 0);
        USART1->DR = 'a';
        DelayLoop(100000);
        PinClear(GPIOB, 0);
        DelayLoop(100000);
    } // while
}

/*
 *     // someth
    PinSetupAlterFunc(GPIOA, 9, omPushPull, pudNone, AF7);
    rccEnableUSART1(FALSE);
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE;     // Enable USART
    USART1->BRR = 2097000 / 115200;
    USART1->DR = 'a';
 *
 */
