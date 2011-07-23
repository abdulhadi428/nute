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

//#define DELAY_USE_TIMER0
#define DELAY_USE_TIMER1


struct Delay_t {
    void Init(void);
    void Reset(uint16_t *AVar);
    bool Elapsed(uint16_t *AVar, const uint16_t ADelay);
};

extern Delay_t Delay;


#endif	/* _TIME_UTILS_H */

