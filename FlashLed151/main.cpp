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

//void BtnInit (void) {
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    GPIO_InitTypeDef  GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Pin = BTN_P;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//}


int main(void) {
//    // Enable HSI Clock
//    RCC_HSICmd(ENABLE);
//    // Wait till HSI is ready
//    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
//
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
//
//    RCC_MSIRangeConfig(RCC_MSIRange_6);
//    RCC_HSEConfig(RCC_HSE_OFF);
//
//    if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
//    {
//      while(1);
//    }

    // LED init
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_7);

    // Delay init
    Delay.Init();

    while (1) {
        GPIOB->ODR ^= GPIO_Pin_7;

        Delay.ms(400);
    }
}
