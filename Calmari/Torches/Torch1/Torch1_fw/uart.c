#include "uart.h"
#include "stm32l1xx_usart.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"

// ************** Implementation ***********************
void UART_Init(void) {
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);      // UART clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); // GPIO clock
    // ==== GPIO init ====
    // Configure USART Tx as alternate function push-pull
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Connect Port to USART Tx
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

    // ==== USART configuration ====
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 128000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART1, ENABLE);
}
void UART_Print (uint8_t AByte) {
    // Write a character to the USART
    USART_SendData(USART1, AByte);
    // Loop until the end of transmission
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
void UART_PrintAsHex (uint8_t AByte){
    UART_Print(UintToHexChar (AByte >> 4));
    UART_Print(UintToHexChar (AByte & 0x0F));
}

char UintToHexChar (uint8_t b) {
    return ((b<=0x09) ? (b+'0') : (b+'A'-10));
}

// ============================== High-level ===================================
// Send as digits
void UART_PrintUint (uint32_t ANumber) {
    uint8_t digit = '0';
    bool ShouldPrint = false;
    const uint32_t m[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};

    for(uint8_t i=0; i<9; i++) {
        while (ANumber >= m[i]) {
            digit++;
            ANumber -= m[i];
        }
        if (digit != '0' || ShouldPrint) {
            UART_Print (digit);
            ShouldPrint = true;
        }
        digit = '0';
    } // for
    UART_Print ((uint8_t)('0'+ANumber));
}
void UART_PrintInt (int32_t ANumber) {
    if (ANumber < 0) {
        UART_Print ('-');
        ANumber = -ANumber;
    }
    else UART_Print (' ');
    UART_PrintUint (ANumber);
}

void UART_PrintString (const char *S) {
    while (*S != '\0') UART_Print (*S++);
}

void UART_PrintArrAsString (uint8_t *Arr, uint8_t ALength) {
    while (ALength--) {
        UART_PrintUint(*(Arr++));
        UART_Print (' ');
    }
    UART_NewLine();
}
void UART_PrintArrAsHex(uint8_t *Arr, uint8_t ALength) {
    while (ALength--) {
        UART_PrintAsHex(*(Arr++));
        UART_Print (' ');
    }
    UART_NewLine();
}

void UART_NewLine (void) {
    UART_Print ('\r');
}

void UART_StrUint(const char *S, uint32_t ANumber) {
    UART_PrintString(S);
    UART_PrintUint(ANumber);
    UART_NewLine();
}
void UART_StrInt  (const char *S,  int32_t ANumber) {
    UART_PrintString(S);
    UART_PrintInt(ANumber);
    UART_NewLine();
}
void UART_StrHex8(const char *S, uint8_t ANumber) {
    UART_PrintString(S);
    UART_PrintAsHex(ANumber);
    UART_NewLine();
}
void UART_StrHex16(const char *S, uint16_t ANumber) {
    UART_PrintString(S);
    UART_PrintAsHex((uint8_t)(ANumber>>8));
    UART_PrintAsHex((uint8_t)(ANumber & 0x00FF));
    UART_NewLine();
}
