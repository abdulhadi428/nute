/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "led.h"
#include "cc2500.h"

uint16_t ID;

void GeneralInit(void);

int main(void) {
    GeneralInit();

    while (1) {
        CC.Task();
    }
}

void GeneralInit(void) {
    // Get self ID: set by pins
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    Delay.ms(1);
    // Read ID
    ID = GPIO_ReadInputData(GPIOB);
    ID &= 0x07; // clear all unneeded
    if (ID > 6) ID = 0;
    // Deinit pins
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);

    // Proceed with init
    Delay.Init();
    Delay.ms(63);
    UART_Init();

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = ID;
    CC.SetChannel(CC_CHNL);

    klPrintf("\rTransmitter\r");
}
