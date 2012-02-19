/*
 * led.cpp
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#include "led.h"
#include "stm32f10x_rcc.h"

// =============================== Led_t =======================================
void Led_t::Blink(void) {
    IsInsideBlink = true;
    On();
    Delay.Reset(&Timer);
}

void Led_t::Task() {
    if (!IsInsideBlink) return;
    if (Delay.Elapsed(&Timer, LED_BLINK_DELAY)) {
        IsInsideBlink = false;
        Off();
    }
}

// ============================= RGBLed_t ======================================
void RGBLed_t::Init() {
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
    // ==== PWM ====
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    // Inverted PWM
    // Init channels
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);         // Enable autoreload of preload
}

void RGBLed_t::On() {
    SetColor(clBlack);
    NeededColor = clBlack;
    // GPIO
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // Timer
    TIM_Cmd(TIM4, ENABLE);
}

void RGBLed_t::Off() {
    SetColor(clBlack);
    NeededColor = clBlack;
    TIM_Cmd(TIM4, DISABLE);
    // Make PWM output InputFloating
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void RGBLed_t::SetColor(Color_t AColor) {
    TIM_SetCompare2(TIM4, AColor.Red);
    TIM_SetCompare3(TIM4, AColor.Green);
    TIM_SetCompare4(TIM4, AColor.Blue);
    CurrentColor = AColor;
}

void RGBLed_t::Task(void) {
    if (!Delay.Elapsed(&Timer, 10)) return;
    if (CurrentColor == NeededColor) return;
    // Red channel
    if (CurrentColor.Red != NeededColor.Red) {
        if (NeededColor.Red < CurrentColor.Red)
            CurrentColor.Red--;
        else
            CurrentColor.Red++;
    }
    // Green channel
    if (CurrentColor.Green != NeededColor.Green) {
        if (NeededColor.Green < CurrentColor.Green)
            CurrentColor.Green--;
        else
            CurrentColor.Green++;
    }
    // Blue channel
    if (CurrentColor.Blue != NeededColor.Blue) {
        if (NeededColor.Blue < CurrentColor.Blue)
            CurrentColor.Blue--;
        else
            CurrentColor.Blue++;
    }
    SetColor(CurrentColor);
}
