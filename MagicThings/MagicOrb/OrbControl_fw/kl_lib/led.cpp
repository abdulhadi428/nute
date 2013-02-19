/*
 * led.cpp
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#include "led.h"
#include "stm32f10x_rcc.h"

// =============================== LedBlink_t ==================================
void LedBlink_t::Blink(uint16_t ABlinkDelay) {
    IsInsideBlink = true;
    IBlinkDelay = ABlinkDelay;
    On();
    Delay.Reset(&ITimer);
}

void LedBlink_t::Task() {
    if(!IsInsideBlink) return;
    if(Delay.Elapsed(&ITimer, IBlinkDelay)) {
        IsInsideBlink = false;
        Off();
    }
}

// ============================= LedSmooth_t ===================================
/*
 * Parameters:
 * GPIO_TypeDef *AGpio, uint16_t APinNumber,
 * TIM_TypeDef* ATimer, uint16_t ATopValue, uint16_t APrescaler,
 * uint8_t AChannelNumber, bool InvertedPolarity
 */
void LedSmooth_t::Init(GPIO_TypeDef *AGpio, uint16_t APinNumber, TIM_TypeDef* ATimer, uint16_t ATopValue, uint16_t APrescaler, uint8_t AChannelNumber, bool InvertedPolarity) {
    // ==== Gpio ====
    klGpioSetupByN(AGpio, APinNumber, GPIO_Mode_AF_PP);
    // ==== Timer ====
    IPwmTimer = ATimer;
    // Clock
    if      (IPwmTimer == TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    else if (IPwmTimer == TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if (IPwmTimer == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if (IPwmTimer == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    // Timebase
    IPwmTimer->CR1 = 0x01;  // Enable timer
    IPwmTimer->CR2 = 0;
    IPwmTimer->PSC = APrescaler;
    IPwmTimer->ARR = ATopValue; // Autoreload register
    if (IPwmTimer == TIM1) IPwmTimer->BDTR = 0xC000;    // Enable output
    // PWM channel
    switch (AChannelNumber) {
        case 1:
            ICCR = (uint16_t*)(&IPwmTimer->CCR1);
            IPwmTimer->CCMR1 |= 0x60;  // Output, PWM mode 1
            IPwmTimer->CCER  |= InvertedPolarity? 0x0003 : 0x0001;  // Compare output 1 enabled, OC1 is active low/high
            break;
        case 2:
            ICCR = (uint16_t*)(&IPwmTimer->CCR2);
            IPwmTimer->CCMR1 |= 0x6000;
            IPwmTimer->CCER  |= InvertedPolarity? 0x0030 : 0x0010;
            break;
        case 3:
            ICCR = (uint16_t*)(&IPwmTimer->CCR3);
            IPwmTimer->CCMR2 |= 0x60;
            IPwmTimer->CCER  |= InvertedPolarity? 0x0300 : 0x0100;
            break;
        case 4:
            ICCR = (uint16_t*)(&IPwmTimer->CCR4);
            IPwmTimer->CCMR2 |= 0x6000;
            IPwmTimer->CCER  |= InvertedPolarity? 0x3000 : 0x1000;
            break;
        default: break;
    }
}

void LedSmooth_t::RampUp(void) {
    State = lsUp;
    Delay.Reset(&IDelayTmr);
    ISetupDelay();
    //klPrintf("1: %u\r", IDelay);
}
void LedSmooth_t::RampDown(void) {
    State = lsDown;
    Delay.Reset(&IDelayTmr);
    ISetupDelay();
}


void LedSmooth_t::Task() {
    if (State == lsUp) {
        // Check if top achieved
        if (IValue == Top()) { State = lsOn; return; }
        // Otherwise, check if delay elapsed
        if (Delay.Elapsed(&IDelayTmr, IDelay)) {
            *ICCR = IValue++;
            ISetupDelay();
        }
    }
    else if (State == lsDown) {
        // Check if bottom achieved
        if (IValue == 0) { State = lsOff; return; }
        // Otherwise, check if delay elapsed
        if (Delay.Elapsed(&IDelayTmr, IDelay)) {
            *ICCR = --IValue;
            ISetupDelay();
        }
    }
}

/*

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
*/
