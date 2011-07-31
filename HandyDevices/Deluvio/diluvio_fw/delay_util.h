/*
 * File:   time_utils.h
 * Author: Laurelindo
 *
 * Created on 5 Октябрь 2009 г., 0:40
 */

#ifndef _TIME_UTILS_H
#define	_TIME_UTILS_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/atomic.h>

// ========================== Function Prototypes ==============================
void DelayInit(void);
bool DelayElapsed(uint16_t *AVar, const uint16_t ADelay);
void DelayReset(uint16_t *AVar);
void DelayBypass(uint16_t *AVar, const uint16_t ADelay);

#endif	/* _TIME_UTILS_H */

