/*
 * keys.cpp
 *
 *  Created on: 08.02.2012
 *      Author: kreyl
 */

#include "keys.h"
#include "stm32f10x_gpio.h"

Keys_t Keys;
const uint16_t KeybOuts[2][3] = { {GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_10}, {GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5} };


void Keys_t::Task() {
    if (!Delay.Elapsed(&ITimer, KEYS_RATE)) return;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    uint8_t FKbd;

    // Iterate keyboards
    for (uint8_t k=0; k<2; k++) {
        FKbd = KEY_NONE;
        // Iterate outputs
        for (uint8_t o=0; o<3; o++) {
            // Make them inputs
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            GPIO_Init(GPIOB, &GPIO_InitStructure);
            // Make needed output be output
            GPIO_InitStructure.GPIO_Pin = KeybOuts[k][o];
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
            GPIO_Init(GPIOB, &GPIO_InitStructure);
            // Read current input state
            if (GPIO_ReadInputDataBit(GPIOB, (GPIO_Pin_11)) == RESET)      FKbd = 1+o;
            else if (GPIO_ReadInputDataBit(GPIOB, (GPIO_Pin_12)) == RESET) FKbd = 4+o;
            else if (GPIO_ReadInputDataBit(GPIOB, (GPIO_Pin_13)) == RESET) FKbd = 7+o;
            else if (GPIO_ReadInputDataBit(GPIOB, (GPIO_Pin_14)) == RESET) FKbd = (o == 0)? KEY_STAR : ((o == 1)? 0 : KEY_HASH);
        } // for o
        // Trigger event if key changed
        if (FKbd != Kbd[k]) {
            Kbd[k] = FKbd;
            if ((FKbd != KEY_NONE)  && (EvtKbd[k] != 0)) (*EvtKbd[k])();
        }
    } // for k
}

void Keys_t::Init() {
    // ==== Clocks ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Inputs
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // Outputs
    GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10);
    Kbd[0] = KEY_NONE;
    Kbd[1] = KEY_NONE;
}
