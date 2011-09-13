/*
 * pwm.cpp
 *
 *  Created on: 12.09.2011
 *      Author: Kreyl
 */

#include "pwm.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

Pwm_t Pwm;

void Pwm_t::Init() {
    // Interrupt config
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Timer clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    // GPIO clock enable
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    // ==== GPIO Configuration ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

    // ==== Timebase config ====
    // Time base configuration
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = PWM_MAX;
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_PRESCALER;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    // PWM1 Mode configuration
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    // Enable timer
    TIM_Cmd(TIM4, ENABLE);
    // Clear Timer update pending flag
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    // Interrupts config
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

void Pwm_t::IrqHandler() {
    if (IPwm == IPwmDesired) return;
    // Increase tick counter
    ITicker++;
    // Check if may change
    if      (IPwm < 72)  { if (ITicker < 36) return; }
    else if (IPwm < 207) { if (ITicker < 4 ) return; }
    else                 { if (ITicker < 2 ) return; }
    ITicker = 0;

    if (IPwm < IPwmDesired) {
        Set(++IPwm);
        if (IPwm == PWM_MAX) Set(PWM_MAX+1);
    }
    else {
        Set(--IPwm);
    }
}

// ================================ Interrupts =================================
// Delay counter
void TIM4_IRQHandler(void) {
    // Clear TIM2 update interrupt
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    Pwm.IrqHandler();
}

