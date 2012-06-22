/*
 * kl_time.h
 *
 *  Created on: 13.06.2012
 *      Author: kreyl
 */

#ifndef KL_TIME_H_
#define KL_TIME_H_

#include <stdint.h>
#include "kl_lib.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"

#define DEFAULT_YEAR    2012
#define DEFAULT_MONTH   6
#define DEFAULT_DAY     22

#define BKPREG_CHECK    BKP_DR1
#define BKPREG_YEAR     BKP_DR2
#define BKPREG_MONTH    BKP_DR3
#define BKPREG_DAY      BKP_DR4

class TimeCounter_t {
private:

public:
    void DateUpdate(void);
    void Init(void);
    bool IsSet(void) {return (BKP_ReadBackupRegister(BKPREG_CHECK) == 0xA5A5); }
    bool IsLeap(uint16_t AYear);
    void SetDate(uint16_t AYear, uint8_t AMonth, uint8_t ADay);
    void SetTime(uint8_t AHour, uint8_t AMinute, uint8_t ASecond);
    void GetTime(Time_t *PTime);
    void Print(void);
    // Delay utils
    bool SecElapsed(uint32_t *PTimer, uint32_t ASeconds) {
        if ((RTC_GetCounter() - *PTimer) >= ASeconds) {
            *PTimer = RTC_GetCounter(); // Reset delay
            return true;
        }
        else return false;
    }
    void Bypass(uint32_t *AVar, const uint32_t ADelay) { *AVar = RTC_GetCounter() - ADelay; }
};

extern TimeCounter_t Time;

// IRQ
extern "C" {
void RTC_IRQHandler(void);
}

#endif /* KL_TIME_H_ */
