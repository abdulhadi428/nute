/*
 * mdm.cpp
 *
 *  Created on: 12.10.2011
 *      Author: Kreyl
 *
  */
#include "stm32f10x_rcc.h"
#include "arm_common.h"
#include "mdm.h"
#include "delay_util.h"
#include "uart.h"

#define MDM_VERBOSE_IO

mdm_t Mdm;

void mdm_t::Init() {
    GPIOInit();
    PwrKeyHi();
    //BufLength = 0;
    USARTInit();

    // ==== Setup modem ====
    // Cycle PowerKey
    Delay.ms(1008);
    PwrKeyLo();
    Delay.ms(1008);
    PwrKeyHi();
    Delay.ms(1008);
    // Send initialization commands
    // Setup baudrate
    while (1) {
        SendAndWaitOk("AT");
        UART_PrintString("#\r");
        Delay.ms(999);
    }
}

void mdm_t::SendSMSWithTime(const char* AStrNumber, const char *AMsg) {

}

Error_t mdm_t::SendAndWaitOk(const char *S) {
    //Error_t Result;
    //DisableRxIrq();
    //EnableRxIrq();
    USART_Cmd(USART2, DISABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
    RxCounter = 0;
    //USART_ClearFlag(USART2, USART_FLAG_RXNE);
    SendString(S);
    // Receive reply
//    uint8_t b;
//    while(1) {
//        if ((Result = ReceiveByte(&b)) != erOK) return Result;
//        UART_Print(b);
//    }

    return erOK;
}


void mdm_t::SendString(const char *S) {
    //UART_Print('>');
    while (*S != '\0') {
        //UART_Print(*S);
        USART_SendData(USART2, *S++);
        // Loop until the end of transmission
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    USART_SendData(USART2, '\r');   // Send CR (13 or 0x0D)
    USART_SendData(USART2, '\n');   // Send LF (10 or 0x0A)
}

// ================================ Low-level =================================
Error_t mdm_t::ReceiveByte(uint8_t *AByte) {
    for (uint32_t i=0; i<MDM_RX_TIMEOUT; i++) {
        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
            *AByte = USART_ReceiveData(USART2);
            return erOK;
        }
        Delay.us(100);
    }
    return erTimeout;
}

void mdm_t::GPIOInit() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    // PA0=CTS, PA1=RTS, PA2=TX, PA3=RX, PA4=PWR_KEY
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure USART Tx and RTS as alternate function push-pull
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure USART Rx and CTS as input floating
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure PWR_KEY as push-pull output
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void mdm_t::USARTInit() {
    // Clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    // USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    //USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART2, ENABLE);
    // Setup interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// Interrupt
void mdm_t::IRQHandler() {
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        // Read one byte from the receive data register
        uint8_t b = USART_ReceiveData(USART2);
        //Buf[RxCounter++] = b;
#ifdef MDM_VERBOSE_IO
        UART_Print(b);
#endif
    }
}
void USART2_IRQHandler(void) {
    Mdm.IRQHandler();
}
