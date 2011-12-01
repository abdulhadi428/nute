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
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    Key1IsDown = false;
    Key2IsDown = false;
}

void Keys_t::Task() {
    if (!Delay.Elapsed(&Timer, KEY_DELAY)) return;
    // Key1
    if (Key1HwPressed() && !Key1IsDown) {
        Key1IsDown = true;
        // Check if both
        if (Key2IsDown) {
            if (EvtKeyPressBoth != 0) EvtKeyPressBoth();
        }
        else {
            if(EvtKey1Press != 0) EvtKey1Press();
        }
    }
    else if (!Key1HwPressed() && Key1IsDown) {
        Key1IsDown = false;
    }

    // Key2
    if (Key2HwPressed() && !Key2IsDown) {
        Key2IsDown = true;
        // Check if both
        if (Key1IsDown) {
            if (EvtKeyPressBoth != 0) EvtKeyPressBoth();
        }
        else {
            if(EvtKey2Press != 0) EvtKey2Press();
        }
    }
    else if (!Key2HwPressed() && Key2IsDown) {
        Key2IsDown = false;
    }
}
