/*
 * rilma.cpp
 *
 *  Created on: 22.01.2012
 *      Author: kreyl
 */

#include "rilma.h"

Rilma_t Rilma;

void Rilma_t::Init() {
    // ==== GPIO ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    // ==== Timer4 as PWM ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 255;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    // ==== PWM outputs ====
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    // Init channels
#ifdef CHANNEL1_ENABLE
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
#endif
#ifdef CHANNEL2_ENABLE
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
#endif
#ifdef CHANNEL3_ENABLE
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
#endif
    TIM_ARRPreloadConfig(TIM4, ENABLE);         // Enable autoreload of preload
}

void Rilma_t::Task() {

}

void Rilma_t::Set(uint32_t AChannel, uint8_t AValue) {
    switch (AChannel) {
        case 1: TIM_SetCompare1(TIM4, AValue); break;
        case 2: TIM_SetCompare2(TIM4, AValue); break;
        case 3: TIM_SetCompare3(TIM4, AValue); break;
        case 4: TIM_SetCompare4(TIM4, AValue); break;
        default: break;
    }
}

void Rilma_t::SmoothOn() {
    IState = rSmoothOn;
    Set (2, 10);
    // GPIO
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // Timer
    TIM_Cmd(TIM4, ENABLE);
}
