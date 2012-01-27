/*
 * beep.cpp
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#include "beep.h"

BeepSnd_t NothingBeep = {
        2,
        {
            {1503, 0},
            {100, 2},
        }
};
BeepSnd_t AlienBeep = {
        4,
        {
            {999,  0},
            {100, 10},
            {35,   0},
            {100, 10},
        }
};

Beep_t Beep;


// Beep settings
//BeepSnd_t NothingBeep = {
//        .ChunkCount = 3
//
//};

//#include <string.h>
//
//struct A {
//    A(int _a, int _b, int* _c, int _c_len)
//        {
//            a = _a;
//            b = _b;
//            c = new int[_c_len];
//            memcpy(c, _c, sizeof(int) * _c_len);
//            c_len = _c_len;
//        }
//    ~A() {
//        if(c) delete[] c;
//    }
//
//    int     a;
//    int     b;
//    int*    c;
//    int c_len;
//};
//
//
//A aaa(1, 2, {1, 2, 3}, 3);
//{OnLength: 215, OffLength:  27,}
//{OnLength: 170, OffLength:  80, Volume:  50},
//{OnLength: 114, OffLength: 999, Volume:  10},


//const BeepChunk_t AlienBeep[3] = {
//        {215,  27, 10},
//        {170,  80, 50},
//        {114, 999, 10}
//};



void Beep_t::Init() {
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
    if (ISnd == 0) return;
    if (Delay.Elapsed(&ITimer, ISnd->Chunks[ICounter].Length)) {
        // Switch to next chunk
        ICounter++;
        if(ICounter == ISnd->ChunkCount) ICounter = 0;
        // Set current snd params
        if (ISnd->Chunks[ICounter].Volume != 0) On();
        else Off();
        Delay.Reset(&ITimer);
    }
}

void Beep_t::SetSound(BeepSnd_t *ASnd) {
    if (ISnd == ASnd) return;   // Already set
    ISnd = ASnd;
    ICounter = 0;
    Delay.Reset(&ITimer);
    if (ISnd->Chunks[0].Volume != 0) On();
    else Off();
}

void Beep_t::On() {
    // PWM
    TIM_SetCompare1(TIM4, ISnd->Chunks[ICounter].Volume);
    // GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // Timer
    TIM_Cmd(TIM4, ENABLE);
}

void Beep_t::Off() {
    TIM_Cmd(TIM4, DISABLE);
    // Make PWM output InputFloating
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
