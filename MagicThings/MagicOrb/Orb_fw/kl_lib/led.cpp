/*
 * led.cpp
 *
 *  Created on: 03.11.2011
 *      Author: g.kruglov
 */

#include "led.h"
#include "stm32f10x_rcc.h"

void LedRGBW_t::Init() {
    // ==== GPIO ====
	klGpioSetupByN(GPIOA, 1, GPIO_Mode_AF_PP);
	klGpioSetupByN(GPIOB, 1,  GPIO_Mode_AF_PP);
	klGpioSetupByN(GPIOB, 8, GPIO_Mode_AF_PP);
    klGpioSetupByN(GPIOB, 9, GPIO_Mode_AF_PP);

    // ==== Timers ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // ==== Tim2 ====
    TIM2->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
    TIM2->CR2 = 0;          // Output Idle State
    TIM2->PSC = 0;          // No clock division
    TIM2->ARR = 255;        // Autoreload register: top value of PWM
    // Outputs
    TIM2->CCMR1 = 0x6000;   // Ch2 is output, PWM mode 1
    TIM2->CCER = 0x0010;    // Ch2P enabled, active low

    // ==== Tim3 ====
    TIM3->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
    TIM3->CR2 = 0;          // Output Idle State
    TIM3->PSC = 0;          // No clock division
    TIM3->ARR = 255;        // Autoreload register: top value of PWM
    // Outputs
    TIM3->CCMR2 = 0x6000;   // Ch4 is output, PWM mode 1
    TIM3->CCER = 0x3000;    // Ch4P enabled, active low

    // ==== Tim4 ====
    TIM4->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
    TIM4->CR2 = 0;          // Output Idle State
    TIM4->PSC = 0;          // No clock division
    TIM4->ARR = 255;        // Autoreload register: top value of PWM
    // Outputs
    TIM4->CCMR2 = 0x6060;   // Ch3,4 are outputs, PWM mode 1
    TIM4->CCER = 0x3300;    // Ch3P, 4P enabled, active low

    // Initial values
    SetColor(clBlack);
    NeededColor = clBlack;
}

void LedRGBW_t::Task(void) {
//    if (Delay.Elapsed(&ITimer, 10)) {
//        if (ICurrentColor == INeededColor) return;
//        // Red channel
//        if (ICurrentColor.Red != INeededColor.Red) {
//            if (INeededColor.Red < ICurrentColor.Red) ICurrentColor.Red--;
//            else ICurrentColor.Red++;
//        }
//        // Green channel
//        if (ICurrentColor.Green != INeededColor.Green) {
//            if (INeededColor.Green < ICurrentColor.Green) ICurrentColor.Green--;
//            else ICurrentColor.Green++;
//        }
//        // Blue channel
//        if (ICurrentColor.Blue != INeededColor.Blue) {
//            if (INeededColor.Blue < ICurrentColor.Blue) ICurrentColor.Blue--;
//            else ICurrentColor.Blue++;
//        }
//        SetColor(ICurrentColor);
//    }
}
