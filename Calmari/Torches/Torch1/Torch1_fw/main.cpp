/*
 * main.cpp
 *
 *  Created on: 27.08.2011
 *      Author: Kreyl
 */

#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "delay_util.h"
#include "uart.h"
#include "pwm.h"
// Touch
#include "stm32_tsl_api.h"
#include "stm32l15x_tsl_ct_acquisition.h"

#define KEY_DETECTED()  (sSCKeyInfo[0].Setting.b.DETECTED)


int main(void) {
    // ==== Touch ====
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    // Enable Comparator clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP, ENABLE);
    // Initialize Touch Sensing Library
    TSL_Init();
    sSCKeyInfo[0].Setting.b.IMPLEMENTED = 1;
    sSCKeyInfo[0].Setting.b.ENABLED = 1;
    sSCKeyInfo[0].DxSGroup = 0x01;

    // ==== General ====
    UART_Init();
    Delay.ms(7);
    UART_PrintString("Torch1\r");

    // LED init
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_2);

    // Delay init
    Delay.Init();

    Pwm.RampUp();

    // ==== Main cycle ====
    while (1) {
        Pwm.Task();

        if (Pwm.IsReached()) Pwm.RampDown();

//        TSL_Action();
//        if ((TSL_GlobalSetting.b.CHANGED) && (TSLState == TSL_IDLE_STATE)) {
//            TSL_GlobalSetting.b.CHANGED = 0;
//            if (KEY_DETECTED()) {
//                UART_PrintString("Key\r");
//                //GPIOB->ODR ^= GPIO_Pin_7;
//                //GPIO_ResetBits(GPIOB, GPIO_Pin_7);
//            }
//        }
    } // while
}
