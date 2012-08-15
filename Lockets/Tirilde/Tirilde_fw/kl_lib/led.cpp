/*
 * led.cpp
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#include "led.h"
#include "stm32f10x_rcc.h"

// =============================== LedBlink_t ==================================
void LedBlink_t::Blink(uint32_t ABlinkDelay) {
    IsInsideBlink = true;
    IBlinkDelay = ABlinkDelay;
    On();
    Delay.Reset(&ITimer);
}

void LedBlink_t::Task() {
    if (!IsInsideBlink) return;
    if (Delay.Elapsed(&ITimer, IBlinkDelay)) {
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

// ============================= RGBLed_t ======================================
void LedRGB_t::Init() {
    // ==== GPIO ====
	klGpioSetupByN(GPIOA, 10, GPIO_Mode_AF_PP);
	klGpioSetupByN(GPIOA, 8,  GPIO_Mode_AF_PP);
	klGpioSetupByN(GPIOA, 11, GPIO_Mode_AF_PP);
    // ==== Timer ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	// Clock
    // ==== Timebase and general ====
    TIM1->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
    TIM1->CR2 = 0;          // Output Idle State
    TIM1->PSC = 0;          // No clock division
    TIM1->ARR = 255;        // Autoreload register: top value of PWM
    // ==== Outputs ====
    TIM1->BDTR = 0x8000;    // Enable output
    TIM1->CCMR1 = 0x0060;   // Ch1 is output, PWM mode 1
    TIM1->CCMR2 = 0x6060;   // Ch4 & Ch3 are outputs, PWM mode 1
    TIM1->CCER = 0x3303;    // Ch4P, Ch3P, Ch1P outputs enabled, active low
    // Initial values
    SetColor(clBlack);
    INeededColor = clBlack;
}

void LedRGB_t::Task(void) {
    if (IsInsideBlink) {
        if (Delay.Elapsed(&ITimer, IBlinkDelay)) {
            IsInsideBlink = false;
            SetColor(clBlack);
            INeededColor = clBlack;
        }
    }
    else {
        if (Delay.Elapsed(&ITimer, 10)) {
            if (ICurrentColor == INeededColor) return;
            // Red channel
            if (ICurrentColor.Red != INeededColor.Red) {
                if (INeededColor.Red < ICurrentColor.Red) ICurrentColor.Red--;
                else ICurrentColor.Red++;
            }
            // Green channel
            if (ICurrentColor.Green != INeededColor.Green) {
                if (INeededColor.Green < ICurrentColor.Green) ICurrentColor.Green--;
                else ICurrentColor.Green++;
            }
            // Blue channel
            if (ICurrentColor.Blue != INeededColor.Blue) {
                if (INeededColor.Blue < ICurrentColor.Blue) ICurrentColor.Blue--;
                else ICurrentColor.Blue++;
            }
            SetColor(ICurrentColor);
        }
    }
}
