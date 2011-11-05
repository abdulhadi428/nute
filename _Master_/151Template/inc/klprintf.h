/*
 * File:   uartsoftware.h
 * Author: Laurelindo
 *
 * Created on 21 Октябрь 2009 г., 0:20
 */

#ifndef _KLPRINTF_H
#define	_KLPRINTF_H

#include <inttypes.h>
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

void klPrintf_Init(void);
void klPrintf(const char *S, ...);

#ifdef __cplusplus
}
#endif


#endif

