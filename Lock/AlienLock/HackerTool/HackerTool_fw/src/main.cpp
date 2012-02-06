/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "main.h"
#include "led.h"

int main(void) {
    GeneralInit();

    while (1) {
        Led1.Task();
        Led2.Task();
    } // while 1
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();

    KLUartInit();
    Led1.Init(GPIOB, GPIO_Pin_10);
    Led2.Init(GPIOB, GPIO_Pin_11);

    Led1.Blink();
    Led2.Blink();

    klPrintf("\rHacker Tool v.0.1\r");
}


// ================================ IRQs =======================================
void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // Read one byte from the receive data register
        uint16_t b = USART_ReceiveData(USART1);
        if (b == 'U') {
            klPrintf("U:%X\r", 0xDEADBEEF);
            Led1.Blink();
        }
        else {
            Led2.Blink();
            USART_SendData(USART2, b);
            // Loop until the end of transmission
            while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        }
    } // if rx
}

void USART2_IRQHandler(void) {

}

// =============================== USARTs ======================================
void KLUartInit(void) {
    // ==== Clocks ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);      // UART clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);      // UART clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ==== USART configuration ====
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    // USART1
    USART_InitStructure.USART_BaudRate = 128000;
    USART_Init(USART1, &USART_InitStructure);
    // USART2
    USART_InitStructure.USART_BaudRate = 9600;
    USART_Init(USART2, &USART_InitStructure);

    // ==== NVIC ====
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     // Configure the NVIC Preemption Priority Bits
    // Enable the USART1 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Enable the USART2 Interrupt
//    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // Enable USARTs
    USART_Cmd(USART1, ENABLE);
    USART_Cmd(USART2, ENABLE);
}


