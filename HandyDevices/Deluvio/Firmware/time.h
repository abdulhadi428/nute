/* 
 * File:   time.h
 * Author: Kreyl Laurelindo
 *
 * Created on 30 Ноябрь 2010 г., 23:34
 */

#ifndef TIME_H
#define	TIME_H

#include <inttypes.h>
#include <stdbool.h>

// ============================== Types ========================================
struct Time_t {
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
    bool SecondPassed: 1;  // Set in IRQ, reset manually
    bool IsSetCorrectly: 1;
};

// =========================== Variables =======================================
extern struct Time_t Time;

// =========================== Prototypes ======================================
void TimeInit(void);

uint8_t TimeGetMinuteUnits(void);

void EVENT_NewSecond(void);
void EVENT_NewMinute(void);
void EVENT_NewHour(void);
void EVENT_NewDay(void);


#endif	/* TIME_H */

