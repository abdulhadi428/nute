/*
 * sim900.cpp
 *
 *  Created on: 01.06.2012
 *      Author: kreyl
 */

#include "sim900.h"
#include "stm32f10x_usart.h"

sim900_t Mdm;

void sim900_t::Init() {
    GPIOInit();
    PwrKeyHi();
    USARTInit();

}


// ================================== Init =====================================
void sim900_t::GPIOInit() {
    // PA11=CTS, PA12=RTS, PA9=TX, PA10=RX, PA15=PWR_KEY; PC8=Status, PC9 = DCD, PC10=RI, PC11=DTR, PC12=DSR
    // Configure USART Tx and RTS as alternate function push-pull
    klGpioSetupByMsk(GPIOA, GPIO_Pin_9|GPIO_Pin_12, GPIO_Mode_AF_PP);
    // Configure USART Rx and CTS as input floating
    klGpioSetupByMsk(GPIOA, GPIO_Pin_10|GPIO_Pin_11, GPIO_Mode_IN_FLOATING);
    // Configure PWR_KEY as push-pull output
    klGpioSetupByMsk(GPIOA, GPIO_Pin_15, GPIO_Mode_Out_PP);
}

void sim900_t::USARTInit() {
    // Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    // USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    //USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART1, ENABLE);
    // Setup interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
