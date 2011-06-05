/*
 * File:   uartsoftware.h
 * Author: Laurelindo
 *
 * Created on 21 Октябрь 2009 г., 0:20
 */

#ifndef _UART_H
#define	_UART_H

#include <inttypes.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

// ============================ Definitions ====================================
#define UART_HARDWARE   // Software or hardware uart

#ifdef UART_HARDWARE
// USART1 definitions
#define USART1_CLK      RCC_APB2Periph_USART1
#define USART1_TX_PIN   GPIO_Pin_9
#define USART1_RX_PIN   GPIO_Pin_10
#define USART1_GPIO     GPIOA
#define USART1_GPIO_CLK RCC_APB2Periph_GPIOA

//#define TxPin
//#define RxPin
//#define UARTPort

#else
#define UARTTxOneBit    1	// for BaudRate = 490000, sysClock = 8 MHz
#define SET_TX_PIN( )	(UARTPort |=  (1<<TxPin))
#define CLEAR_TX_PIN( ) (UARTPort &= ~(1<<TxPin))
#define GET_RX_PIN( )	(UARTPin & (1<<RxPin))
#endif

class UART_t {
private:
    char UintToHexChar (uint8_t b);
public:
    void Init(void);
    void Print (uint8_t AByte);
    void PrintAsHex (uint8_t AByte);
    void NewLine (void) { Print ('\r'); }
    void PrintUint (uint16_t);
    void PrintInt (int16_t);
    void PrintString (const char *S);
    void PrintStringLen (const char *S, const uint8_t ALength);
    void PrintArr (uint8_t *Arr, uint8_t ALength);
    void PrintArrAsString (uint8_t *Arr, uint8_t ALength);
};

extern UART_t Uart;

// ============================ Prototypes =====================================

#endif

