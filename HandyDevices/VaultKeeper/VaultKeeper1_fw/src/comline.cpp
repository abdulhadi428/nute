/*
 * comline.cpp
 *
 *  Created on: 02.06.2012
 *      Author: kreyl
 */

#include "comline.h"
#include "kl_lib.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include <stdarg.h>

ComLine_t Com;

void ComLine_t::Init() {
    // ==== GPIO ====
    klGpioSetupByN(GPIOA, 2, GPIO_Mode_AF_PP);
    // ==== USART ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    //USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART2, ENABLE);
    // ==== Interrupt ====
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}

/*
 * c: character
 * i: signed int
 * u: unsigned int
 * s, S: string
 * X: hexadecimal, capital letters
 * (1...9) width - Minimum number of characters to be printed
 * A - array as 8-bit hexadecimal, length is to be next after uint8_t*
 */
void ComLine_t::Printf(const char *S, ...) {
    char c;
    bool WasPercent = false;
    va_list Arg;
    va_start(Arg, S);    // Set pointer to first argument
    while ((c = *S) != 0) {
        if (c == '%') {
            if (WasPercent) {
                SendChar(c);  // %%
                WasPercent = false;
            }
            else WasPercent = true;
        }
        else { // not %
            if (WasPercent) {
                if (c == 'c') SendChar((uint8_t)va_arg(Arg, int32_t));
                else if (c == 'u') IPrintUint(va_arg(Arg, uint32_t));
                else if (c == 'i') IPrintInt(va_arg(Arg, int32_t));
                else if (c == 'X') IPrintAsHex(va_arg(Arg, uint32_t));
                else if ((c == 's') || (c == 'S')) IPrintString(va_arg(Arg, char*));
                else if (c == 'A') IPrint8HexArray(va_arg(Arg, uint8_t*), va_arg(Arg, uint32_t));
                WasPercent = false;
            } // if was percent
            else SendChar(c);
        }
        S++;
    } // while
    va_end(Arg);
}

// ============================== Conversions ==================================
void ComLine_t::IPrintUint (uint32_t ANumber) {
    uint8_t digit = '0';
    bool ShouldPrint = false;
    const uint32_t m[9] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};

    for(uint8_t i=0; i<9; i++) {
        while (ANumber >= m[i]) {
            digit++;
            ANumber -= m[i];
        }
        if (digit != '0' || ShouldPrint) {
            SendChar (digit);
            ShouldPrint = true;
        }
        digit = '0';
    } // for
    SendChar ((uint8_t)('0'+ANumber));
}

void ComLine_t::IPrintInt (int32_t ANumber) {
    if (ANumber < 0) {
        SendChar ('-');
        ANumber = -ANumber;
    }
    else SendChar(' ');
    IPrintUint(ANumber);
}


char UintToHexChar (uint8_t b) {
    return ((b<=0x09) ? (b+'0') : (b+'A'-10));
}
void ComLine_t::IPrintAsHex (uint32_t ANumber) {
    bool MustPrint = false;
    if (ANumber & 0xFF000000) {
        SendChar(UintToHexChar (0x0F & (ANumber >> 28)));
        SendChar(UintToHexChar (0x0F & (ANumber >> 24)));
        MustPrint = true;
    }
    if ((ANumber & 0x00FF0000) || MustPrint) {
        SendChar(UintToHexChar (0x0F & (ANumber >> 20)));
        SendChar(UintToHexChar (0x0F & (ANumber >> 16)));
        MustPrint = true;
    }
    if ((ANumber & 0x0000FF00) || MustPrint) {
        SendChar(UintToHexChar (0x0F & (ANumber >> 12)));
        SendChar(UintToHexChar (0x0F & (ANumber >> 8)));
    }
    SendChar(UintToHexChar (0x0F & (ANumber >> 4)));
    SendChar(UintToHexChar (0x0F & (ANumber)));
}

void ComLine_t::IPrint8HexArray(uint8_t *Arr, uint32_t ALength) {
    for (uint32_t i=0; i<ALength; i++) {
        IPrintAsHex(Arr[i]);
        SendChar(' ');
    }
}
