/*
 * kl_time.cpp
 *
 *  Created on: 13.06.2012
 *      Author: kreyl
 */

#include "kl_time.h"
#include "misc.h"
#include "stm32f10x_pwr.h"


TimeCounter_t Time;

//#define RTC_OUTPUT_ENABLE

void TimeCounter_t::Init() {
    // ==== IRQ ====
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    // Enable PWR and BKP clocks
    PWR_BackupAccessCmd(ENABLE);    // Allow access to BKP Domain
    // Check if time is set
    if (!IsSet()) {
        TimeIsSet = false;
        DbgUART.SendPrintF("Nothing is set\r");
        // ==== Rtc config ====
        BKP_DeInit();                   // Reset Backup Domain
        RCC_LSEConfig(RCC_LSE_ON);      // Enable LSE
        uint32_t FTmr;
        Delay.Reset(&FTmr);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {    // Wait till LSE is ready
            if (Delay.Elapsed(&FTmr, 1800)) {
            	DbgUART.SendPrintF("32768 clk failure\r");
                break;
            }
        }
        if (RCC_GetFlagStatus(RCC_FLAG_LSERDY)) {
        	DbgUART.SendPrintF("32768 clk started\r");
            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);     // Select LSE as RTC Clock Source
            RCC_RTCCLKCmd(ENABLE);                      // Enable RTC Clock
            RTC_WaitForSynchro();                       // Wait for RTC registers synchronization
            RTC_WaitForLastTask();  // Wait until last write operation on RTC registers has finished
            // Set RTC prescaler: set RTC period to 1sec
            RTC_SetPrescaler(32767);// RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
            RTC_WaitForLastTask();  // Wait until last write operation on RTC registers has finished
        }
        // Set default date values and do not set time
        SetDate(DEFAULT_YEAR, DEFAULT_MONTH, DEFAULT_DAY);
        BKP_WriteBackupRegister(BKPREG_CHECK, 0xA5A5);  // Signal is set
    }
    else {
    	DbgUART.SendPrintF("Something is stored\r");
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
    }
    // Enable IRQ
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    RTC_WaitForLastTask();

#ifdef RTC_OUTPUT_ENABLE
    BKP_TamperPinCmd(DISABLE);      // To output RTCCLK/64 on Tamper pin, the tamper functionality must be disabled
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);    // Enable RTC Clock Output on Tamper Pin
#endif
}

void TimeCounter_t::GetTime(Time_t *PTime) {
    uint32_t t = RTC_GetCounter();
    PTime->H = t / 3600;
    PTime->M = (t % 3600) / 60;
    PTime->S = (t % 3600) % 60;
}

void TimeCounter_t::GetDateTime(DateTime_t *PDateTime) {
    PDateTime->Year  = BKP_ReadBackupRegister(BKPREG_YEAR);
    PDateTime->Month = BKP_ReadBackupRegister(BKPREG_MONTH);
    PDateTime->Day   = BKP_ReadBackupRegister(BKPREG_DAY);
    uint32_t t = RTC_GetCounter();
    PDateTime->H = t / 3600;
    PDateTime->M = (t % 3600) / 60;
    PDateTime->S = (t % 3600) % 60;
}

void TimeCounter_t::Print(void) {
    uint16_t Year = BKP_ReadBackupRegister(BKPREG_YEAR);
    uint8_t Month = BKP_ReadBackupRegister(BKPREG_MONTH);
    uint8_t Day   = BKP_ReadBackupRegister(BKPREG_DAY);

    uint32_t t = RTC_GetCounter();
    uint32_t THH = t / 3600;
    uint32_t TMM = (t % 3600) / 60;
    uint32_t TSS = (t % 3600) % 60;
    DbgUART.SendPrintF("%u-%u-%u %u:%u:%u\r", Year, Month, Day, THH, TMM, TSS);
}

bool TimeCounter_t::IsLeap(uint16_t AYear) {
    if     ((AYear % 400)==0) return true;
    else if((AYear % 100)==0) return false;
    else if((AYear % 4  )==0) return true;
    else return false;
}

void TimeCounter_t::SetDate(uint16_t AYear, uint8_t AMonth, uint8_t ADay) {
    // Check if date is correct
    if(((AMonth==4 or AMonth==6 or AMonth==9 or AMonth==11) and ADay==31) or \
        (AMonth==2 and ADay==31) or \
        (AMonth==2 and ADay==30) or \
        (AMonth==2 and ADay==29 and !IsLeap(AYear))) {
    	DbgUART.SendPrintF("Wrong date\r");
        return;
    }
    else {
        BKP_WriteBackupRegister(BKPREG_YEAR, AYear);
        BKP_WriteBackupRegister(BKPREG_MONTH, AMonth);
        BKP_WriteBackupRegister(BKPREG_DAY, ADay);
    }
}

void TimeCounter_t::SetTime(uint8_t AHour, uint8_t AMinute, uint8_t ASecond) {
    // Set time
    uint32_t t = AHour*3600 + AMinute*60 + ASecond;
    RTC_WaitForLastTask();
    RTC_SetCounter(t);
    RTC_WaitForLastTask();
}


// This function is called when 1 Day has elapsed
void TimeCounter_t::DateUpdate(void) {
    uint16_t Year = BKP_ReadBackupRegister(BKPREG_YEAR);
    uint8_t Month = BKP_ReadBackupRegister(BKPREG_MONTH);
    uint8_t Day   = BKP_ReadBackupRegister(BKPREG_DAY);

    if(Month==2) {
        if(Day < 28) Day++;
        else if(Day == 28) {    // Leap Year Correction
            if (IsLeap(Year)) Day++;
            else {
                Month++;
                Day=1;
            }
        }
        else if (Day == 29) {
            Month++;
            Day=1;
        }
    }
    else if (Month==4 or Month==6 or Month==9 or Month==11) {
        if(Day < 30) Day++;
        else {  // == 30, inc month
            Month++;
            Day=1;
        }
    }
    else { // 1, 3, 5, 7, 8, 10, 12
        if(Day < 31) Day++;
        else {  // == 31, inc month
            if(Month != 12) {
                Month++;
                Day=1;
            }
            else { // New year
                Month = 1;
                Day = 1;
                Year++;
            }
        }
    }
    // Save date
    BKP_WriteBackupRegister(BKPREG_YEAR, Year);
    BKP_WriteBackupRegister(BKPREG_MONTH, Month);
    BKP_WriteBackupRegister(BKPREG_DAY, Day);
}

// ================================== IRQ ======================================
void RTC_IRQHandler(void) {
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
        RTC_ClearITPendingBit(RTC_IT_SEC);
        NVIC_ClearPendingIRQ(RTC_IRQn);
        if(RTC_GetCounter() == 86399) {
            RTC_WaitForLastTask();
            RTC_SetCounter(0);
            Time.DateUpdate();
            RTC_WaitForLastTask();
        }
    }
}

