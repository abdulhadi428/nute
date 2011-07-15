/* 
 * File:   beep.h
 * Author: Kreyl Laurelindo
 *
 * Created on 30 Ноябрь 2010 г., 17:37
 */

#ifndef BEEP_H
#define	BEEP_H

#include <inttypes.h>
#include <stdbool.h>

#define BEEP_SHORT      27
#define BEEP_LONG       999
#define BEEP_TOP_VALUE  200 // Timer/counter setting

struct Beep_t {
    uint16_t Timer;
    uint16_t Duration;
    bool IsYelling;
};

extern struct Beep_t EBeep;

void Task_Beep(void);
void BeepInit(void);
void Beep(uint16_t ADuration);

#endif	/* BEEP_H */

