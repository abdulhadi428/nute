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

#ifdef __cplusplus
extern "C" {
#endif


// ============================ Prototypes =====================================
void UART_Init(void);
void klPrintf(const char *S, ...);

#ifdef __cplusplus
}
#endif


#endif

