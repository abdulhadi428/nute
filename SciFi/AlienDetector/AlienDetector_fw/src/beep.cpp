/*
 * beep.cpp
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#include "beep.h"

BeepSnd_t IdleBeep = {
        2,
        {
            {880, 9, 100},
            {0,   0, 1503},
        }
};
BeepSnd_t AlienBeep = {
        6,
        {
            {1800,  7,  150},   // On
            {0,     0,   35},   // Off
            {1800, 18,  170},   // On
            {0,     0,   35},   // Off
            {1800,  7,  114},   // On
            {0,     0,  500},   // Off
        }
};

Beep_t Beep;

void Beep_t::Init() {
    NewSnd = 0;
    // ==== GPIO ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    // ==== Timer4 as PWM ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 200;
    TIM_TimeBaseStructure.TIM_Prescaler = 50;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    // ==== PWM ====
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    // Init channel 1
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);         // Enable autoreload of preload
}

void Beep_t::Task() {
    if (ISnd == 0) {
        if (NewSnd == 0) return;
        else {  // for initial conditions
            ISnd = NewSnd;
            Delay.Reset(&ITimer);
        }
    }
    // Switch to next sound if current chunk is last
    if ((ISnd != NewSnd) && (ICounter == (ISnd->ChunkCount-1))) {
        ICounter = 0;
        ISnd = NewSnd;
        if (ISnd->Chunks[ICounter].Volume != 0) On();
        else Off();
        Delay.Reset(&ITimer);
    }

    if (Delay.Elapsed(&ITimer, ISnd->Chunks[ICounter].Length)) {
        // Switch to next chunk
        ICounter++;
        if(ICounter == ISnd->ChunkCount) ICounter = 0;  // Current beep finished
        // Set current snd params
        if (ISnd->Chunks[ICounter].Volume != 0) On();
        else Off();
        Delay.Reset(&ITimer);
    }
}

void Beep_t::SetSound(BeepSnd_t *ASnd) {
    if (ISnd == ASnd) return;   // Already set
    NewSnd = ASnd;
}

void Beep_t::On() {
    // PWM
    TIM_SetCompare1(TIM4, ISnd->Chunks[ICounter].Volume);
    uint16_t FPrescaler = (40000/(ISnd->Chunks[ICounter].Freq)) - 1;
    TIM_PrescalerConfig(TIM4, FPrescaler, TIM_PSCReloadMode_Update);
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // Timer
#ifndef BEEP_DISABLE
    TIM_Cmd(TIM4, ENABLE);
#endif
}

void Beep_t::Off() {
    TIM_Cmd(TIM4, DISABLE);
    // Make PWM output InputFloating
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
