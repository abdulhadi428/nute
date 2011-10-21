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
    State = erBuzy;
    Delay.ms(1008);
    Error_t r = erError;
    while (r != erOk) {     // Try to setup modem
        while (1) { // Cycle PwrKey and send AT
            UART_PrintString("#\r");
            PwrKeyLo();
            Delay.ms(540);
            PwrKeyHi();
            Delay.ms(540);
            if(SendAndWaitString("AT", "OK") == erOk) break;
            if(SendAndWaitString("AT", "OK") == erOk) break;
            if(SendAndWaitString("AT", "OK") == erOk) break;
        }
        // Send initialization commands
        //if(SendAndWaitString("ATE0", "OK") != erOk) return; // Echo off
        if((r = SendAndWaitString("AT+IPR=9600", "OK")) != erOk) continue; // Setup speed
        if((r = ProcessSIM()) != erOk) continue;
    } // while r
    UART_PrintString("Modem is ready\r");
    State = erOk;
}

// ========================== Card, call, sms =================================
void mdm_t::SendSMSWithTime(const char* AStrNumber, const char *AMsg) {

}

Error_t mdm_t::ProcessSIM() {
    Error_t r = SendRequest("AT+CPIN?");
    if (r != erOk) return r;
    // Check what was answered
    UART_Print('>');
    UART_PrintString(Line);
    UART_NewLine();
    if (strcmp(Line, "READY") == 0) return erOk;
    return erError;
}

// =========================== AT commands ====================================
Error_t mdm_t::SendAndWaitString(const char *ACmd, const char *AReply) {
    Reply = erBuzy;
    CharCounter = 0;
    strcpy(Line, ACmd);
    EnableRxIrq();
    SendString(Line);
    for (uint32_t i=0; i<MDM_RX_TIMEOUT; i++) {
        Delay.us(450);
        if (Reply == erOk) { // Check what received
//            UART_Print('>');
//            UART_PrintString(Line);
//            UART_NewLine();
            if (strstr(Line, ACmd) == 0) {   // Substr not found: not echo
                if (strcmp(Line, AReply) == 0) {
                    //UART_PrintString("Reply got\r");
                    DisableRxIrq();
                    return erOk;
                }
            }
            else { // Echo
                //UART_PrintString("Echo\r");
            } // if echo
            // Reset line
            Reply = erBuzy;
            CharCounter = 0;
        }
    }
    DisableRxIrq();
    return erTimeout;
}

Error_t mdm_t::SendRequest(const char *ARequest) {
    Reply = erBuzy;
    CharCounter = 0;
    char S[18];
    // Copy command part of request: AT+CPIN? => +CPIN
    //UART_Print('1');
    char *c = strchr(ARequest, '+');    // Find '+' in request
    if (c == 0) return erError;
    //UART_Print('2');
    //c++;    // point to next char
    uint8_t L = strcspn(c, "?");
    strlcpy(S, c, L+1);
    //UART_PrintString(S);
    //return erOk;
    // Copy request
    strcpy(Line, ARequest);
    EnableRxIrq();
    SendString(Line);

    UART_Print('q');
    Delay.ms(400);
    DisableRxIrq();
    return erError;

    for (uint32_t i=0; i<MDM_RX_TIMEOUT; i++) {
        Delay.us(100000);
        if (Reply == erOk) { // New line received, check it
            UART_Print('3');
            if (strstr(Line, ARequest) == 0) {  // Substr not found: not echo
                UART_Print('4');
                // Check if reply contains what requested
                if (strstr(Line, S) != 0) {     // Substr found (+CPIN: READY)
                    UART_Print('5');
                    c = strchr(Line, ':');      // Find ':' in reply
                    if (c == 0) return erError;
                    c += 2;                     // Point to first char of reply
                    L = strlen(c);
                    if (L == 0) return erError;
                    strlcpy(Line, c, L+1);      // Copy reply to Line
                    DisableRxIrq();
                    return erOk;
                } // if reply contains
            } // if not echo
            // Reset line
            Reply = erBuzy;
            CharCounter = 0;
        }
    }
    DisableRxIrq();
    return erTimeout;
}

void mdm_t::SendString(char *S) {
    //UART_Print('>');
    while (*S != '\0') {
        //UART_Print(*S);
        USART_SendData(USART2, *S++);
        // Loop until the end of transmission
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    USART_SendData(USART2, '\r');   // Send CR (13 or 0x0D)
}

// ================================ Low-level =================================
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
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == RESET) return;
    // Read one byte from the receive data register
    uint8_t b = USART_ReceiveData(USART2);
#ifdef MDM_VERBOSE_IO
    USART_SendData(USART1, b);
#endif
    // Process the byte
    if (b == '\n') return;  // ignore LF
    if (b == '\r') {
        if (CharCounter != 0) {
            Line[CharCounter] = 0;  // line end
            Reply = erOk;
        }
        return;
    }
    Line[CharCounter++] = b;
}
void USART2_IRQHandler(void) {
    Mdm.IRQHandler();
}
