/*
 * pwm.cpp
 *
 *  Created on: 12.09.2011
 *      Author: Kreyl
 */

#include "pwm.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "delay_util.h"

Pwm_t Pwm;

void Pwm_t::Init() {
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
    TIM_TimeBaseStructure.TIM_Prescaler = 7; // timer freq = SysClk/(Period*(Prescaler+1)); 2000/250 = 8 kHz; 8/(7+1) = 1 kHz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    // PWM1 Mode configuration
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = IPwm;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_Cmd(TIM4, ENABLE);

    Delay.Reset(&ITimer);
}

void Pwm_t::Task() {
    if (IPwm == IPwmDesired) return;
    if (!MayChange()) return;
    if (IPwm < IPwmDesired) {
        if (IPwm == 0) Init();      // Start PWM if stopped
        Set(++IPwm);
    }
    else {
        Set(--IPwm);
    }
}

bool Pwm_t::MayChange() {
    if      (IPwm < 27) return Delay.Elapsed(&ITimer, 108);
    else if (IPwm < 54) return Delay.Elapsed(&ITimer, 72);
    else                return Delay.Elapsed(&ITimer, 18);
}
