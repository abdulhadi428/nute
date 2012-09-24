/*
 * main.cpp
 *
 *  Created on: 27.08.2011
 *      Author: Kreyl
 */

#include <stdint.h>
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

int main(void) {



    while(1);
}

/*
 *     RCC->AHBENR |= 0x00000002;

    GPIOB->MODER = 0x5000;
    GPIOB->OSPEEDR = 0x5000;

    GPIOB->ODR = 0x00C0;
 */


//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
//
//GPIO_InitTypeDef GPIO_InitStructure;
//GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//GPIO_Init(GPIOC, &GPIO_InitStructure);
//
////    GPIO_SetBits(GPIOC, GPIO_Pin_6);
//
//
//
//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//
//GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//GPIO_SetBits(GPIOB, GPIO_Pin_7);
//
//
//GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
//
////    uint32_t PrescalerValue = (uint16_t) (SystemCoreClock / 8000000) - 1;
//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//TIM_OCInitTypeDef  TIM_OCInitStructure;
//TIM_TimeBaseStructure.TIM_Period = 1;
//TIM_TimeBaseStructure.TIM_Prescaler = 0;
//TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//
//TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//TIM_OCInitStructure.TIM_Pulse = 1;
//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//
//TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//
//TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//TIM_Cmd(TIM3, ENABLE);
//
