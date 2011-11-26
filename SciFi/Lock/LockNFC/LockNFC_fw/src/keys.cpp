/*
 * keys.cpp
 *
 *  Created on: 26.11.2011
 *      Author: Kreyl
 */

#include "keys.h"
#include "stm32f10x_rcc.h"
#include "delay_util.h"


Keys_t Keys;

void Keys_t::Init() {
    // Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // Pins
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    for(uint32_t i=0; i<KEY_COUNT; i++) {
        GPIO_InitStructure.GPIO_Pin  = KeyPin[i];
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        KeyIsDown[i] = false;
    }
}

void Keys_t::Task() {
    if (!Delay.Elapsed(&Timer, KEY_DELAY)) return;
    for (uint32_t i=0; i<KEY_COUNT; i++) {
        if ((GPIO_ReadInputDataBit(GPIOA, KeyPin[i]) == Bit_RESET) && !KeyIsDown[i]) {
            KeyIsDown[i] = true;
            if(EvtKeyPress[i] != 0) EvtKeyPress[i]();
        }
        else if ((GPIO_ReadInputDataBit(GPIOA, KeyPin[i]) == Bit_SET) && KeyIsDown[i]) {
            KeyIsDown[i] = false;
        }
    }
}
