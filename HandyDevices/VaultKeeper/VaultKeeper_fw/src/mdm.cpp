/*
 * mdm.cpp
 *
 *  Created on: 12.10.2011
 *      Author: Kreyl
 *
  */
#include "stm32f10x_rcc.h"
#include "mdm.h"
#include "delay_util.h"

mdm_t Mdm;

void mdm_t::Init() {
    // Setup hardware: PA0=CTS, PA1=RTS, PA2=TX, PA3=RX, PA4=PWR_KEY
    // ==== Clocks init ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure USART Tx and RTS as alternate function push-pull
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure USART Rx and CTS as input floating
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure PWR_KEY as push-pull output
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    PwrKeyHi();

    // ==== USART configuration ====
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART2, ENABLE);

    // ==== Setup modem ====
    // Cycle PowerKey
    Delay.ms(999);
    PwrKeyLo();
    Delay.ms(999);
    PwrKeyHi();

}

void mdm_t::Send(char *AString) {
    while (*AString != '\0') {
        USART_SendData(USART2, *AString++);
        // Loop until the end of transmission
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    USART_SendData(USART2, '\r');   // Send CR (13 or 0x0D)
}
