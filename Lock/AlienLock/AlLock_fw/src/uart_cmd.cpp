/*
 * uart_cmd.cpp
 *
 *  Created on: 20.02.2012
 *      Author: kreyl
 */

#include "uart_cmd.h"
#include "stm32f10x_rcc.h"
#include "kl_gpio.h"
#include "delay_util.h"

CmdUnit_t CmdUnit;

void CmdUnit_t::Init(void) {
    // ==== Clocks init ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);      // UART clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    // ==== GPIO init ====
    klPin_t TxPin(GPIOA, 2, GPIO_Mode_AF_PP);       // TX2
    klPin_t RxPin(GPIOA, 3, GPIO_Mode_IN_FLOATING); // RX2
    // ==== USART configuration ====
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART2, ENABLE);
}

void CmdUnit_t::Task() {
    if (Delay.Elapsed(&ITimer, 99)) {
        if (ReadyToWrite()) {
            WriteByte(0xAA);
        }
    }
}
