/*
 * led.cpp
 *
 *  Created on: 15.10.2011
 *      Author: Kreyl
 */

#include "led.h"

Led_t Led;

void Led_t::Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Led_t::Task() {
    if (Delay.Elapsed(&Timer, LED_ON_TIME)) Off();
}
