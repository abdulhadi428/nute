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
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


// ============================ Prototypes =====================================
void UART_Init(void);
//void UART_Print (uint8_t AByte);
//char UintToHexChar (uint8_t b);
//void UART_PrintAsHex (uint8_t AByte);
//void UART_PrintUint (uint32_t ANumber);
//void UART_PrintInt (int32_t ANumber);
//void UART_PrintString (const char *S);
//void UART_PrintArrAsString (uint8_t *Arr, uint8_t ALength);
//void UART_PrintArrAsHex(uint8_t *Arr, uint8_t ALength);
//void UART_NewLine (void);
//
//void UART_StrUint (const char *S, uint32_t ANumber);
//void UART_StrInt  (const char *S,  int32_t ANumber);
//void UART_StrHex8 (const char *S, uint8_t  ANumber);
//void UART_StrHex16(const char *S, uint16_t ANumber);

void klPrintf(const char *S, ...);

#ifdef __cplusplus
}
#endif


#endif

