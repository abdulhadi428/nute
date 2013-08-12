/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "acc.h"
#include "ManyLed.h"
#include "signs.h"
#include "vibro.h"
#include <math.h>

//#define TRAIN_MODE  // uncomment this to output accelerations
App_t App;

#define TOLERANCE               1.2
#define INITIAL_COUNTDOWN       80
#define T_FILTER                5

// ==== Sleep-related consts ====
#define SLEEP_ENABLED
#define SLEEP_TIMEOUT           100
#define SLEEP_TIMEOUT_INITIAL   4
#define STABLE_TOLERANCE        20
// ==============================

// Sleep related
IWDG_t Iwdg;
static bool FirstTime = true;  // First time after reset
static int16_t SleepTimeoutCounter = SLEEP_TIMEOUT_INITIAL;

void GoSleep() {
    // Start LSI
    Clk.LsiEnable();
    // Start IWDG
    Iwdg.SetTimeout(4500);
    Iwdg.Enable();
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
}

// =========================== Process_t =======================================
class Process_t {
private:
    void AperiodicFilter(int16_t T, int16_t *PrevValue, int16_t NewValue) {
        if(FirstTime) *PrevValue = NewValue;
        else *PrevValue = *PrevValue + ((NewValue - *PrevValue) / T);
        // Check if value changed. Reset SleepTimeoutCounter if yes.
        if(Abs(NewValue - *PrevValue) > STABLE_TOLERANCE) SleepTimeoutCounter = SLEEP_TIMEOUT;
    }
    int16_t Abs(int16_t v) {
        return (v < 0)? -v : v;
    }
public:
    int16_t AccVector[ACC_VECTOR_SZ];
    uint8_t CurrentSubsign;
    void AddAcc(uint8_t ID, int16_t *PAcc);
    int16_t IsSimilar(const int16_t *PSign, const int16_t radius);
    void PrintVect() {
        for(uint8_t i = 0; i < 21; i++) Uart.Printf("%d,", 0xFF);//AccVector[i]);
        Uart.Printf("\r\n");
    }
} Process;

void Process_t::AddAcc(uint8_t ID, int16_t *PAcc) {
    AperiodicFilter(T_FILTER, &AccVector[ID * 3 + 0], PAcc[0]);
    AperiodicFilter(T_FILTER, &AccVector[ID * 3 + 1], PAcc[1]);
    AperiodicFilter(T_FILTER, &AccVector[ID * 3 + 2], PAcc[2]);
}

int16_t Process_t::IsSimilar(const int16_t *PSign, const int16_t radius) {
    double dist = 0;
    double diff = 0;
    int16_t result = 0;
    for(uint8_t i = 0; i < ACC_VECTOR_SZ; i++) {
        diff = PSign[i] - AccVector[i];
        dist = dist + diff * diff;
    }
    result = int(sqrt(dist));

    if(result > int(float(radius) * TOLERANCE)) result = -1;

    return result;
}

#ifndef TRAIN_MODE
// Returns stIdle if no making sence sequence found
static Status_t Analyze(uint32_t code) {
    uint32_t sub = 0;
    Status_t newStatus = stIdle;

    sub = code % 100;

    switch(sub) {
        case AARD_SOURCE_DONE:
            newStatus = stAardSrc;
            break;
        case IGNI_SOURCE_DONE:
            newStatus = stIgniSrc;
            break;
        case KVEN_SOURCE_DONE:
            newStatus = stKvenSrc;
            break;
        case IRDEN_SOURCE_DONE:
            newStatus = stIrdenSrc;
            break;
        case GELIO_SOURCE_DONE:
            newStatus = stGelioSrc;
            break;
        case RESET_DONE:
            newStatus = stSignReset;
            break;
    }

    sub = code % 10000;

    switch(sub) {
        case AARD_FORCE_DONE:
            newStatus = stAardForce;
            break;
        case IGNI_FORCE_DONE:
            newStatus = stIgniForce;
            break;
        case KVEN_FORCE_DONE:
            newStatus = stKvenForce;
            break;
        case IRDEN_FORCE_DONE:
            newStatus = stIrdenForce;
            break;
        case GELIO_FORCE_DONE:
            newStatus = stGelioForce;
            break;
    }

    sub = code;

    switch(sub) {
        case AARD_EXEC_DONE:
            newStatus = stAardExec;
            break;
        case IGNI_EXEC_DONE:
            newStatus = stIgniExec;
            break;
        case KVEN_EXEC_DONE:
            newStatus = stKvenExec;
            break;
        case IRDEN_EXEC_DONE:
            newStatus = stIrdenExec;
            break;
        case GELIO_EXEC_DONE:
            newStatus = stGelioExec;
            break;
    }
    return newStatus;
}
#endif
// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 256);

static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");
#ifdef TRAIN_MODE
    uint32_t counter;
#else
    int16_t bestResult;
    int16_t result = 0;
    uint32_t tmpCode = 0;
    uint32_t newCode = 0;
    Status_t tmpStatus = stIdle;
    uint32_t SignCodeNumb = 999999;
    int32_t SignTimeoutCounter = 0;
#endif

    while(1) {
        chThdSleepMilliseconds(9);
        if(!App.Enabled) continue;  // Do nothing

        // ==== Read accelerations ====
        bool AllOperational = true;
        for(uint8_t i = 0; i < ACC_CNT; i++) {
            Acc[i].ReadAccelerations();
            if(Acc[i].IsOperational) {
//                Uart.Printf("%u %d %d %d; ", i, Acc[i].a[0], Acc[i].a[1], Acc[i].a[2]);
                // Copy accelerations to vector. Also, filter is applied here, and acceleration change is checked.
                Process.AddAcc(i, &Acc[i].a[0]);
#ifndef TRAIN_MODE
                // Prepare to compare
                bestResult = 9999;
                newCode = SignCodeNumb;
                // *** Iterate all subsigns ***
                chSysLock();// Do not switch to other thread
                for(uint8_t j = 0; j < SUBSIGN_CNT; j++) {
                    // Check if similar
                    result = Process.IsSimilar(Subsign[j], hyperRadius[j]);
                    if(result > -1) {
                        // Seems like similar
                        tmpCode = (SignCodeNumb * 100) % 1000000 + j;
                        tmpStatus = Analyze(tmpCode);
                        if((tmpStatus != stIdle) and (result < bestResult)) {
                            newCode = tmpCode;
                            App.Status = tmpStatus;
                            bestResult = result;
                            Process.CurrentSubsign = j;
                            SignTimeoutCounter = INITIAL_COUNTDOWN; // Reset timeout counter
                            Uart.Printf("%d\r\n", j);
                        } // if
                    } // if result > -1
                } // for
                chSysUnlock();
                SignCodeNumb = newCode;
#endif
            } // if operational
            else {
                Uart.Printf("Acc %u fail\r", i);
                // Try to rise from dead
                Acc[i].Init();
                if(!Acc[i].IsOperational) {
                    AllOperational = false;
                    chThdSleepMilliseconds(450);
                }
            }
        } // for i

        // Handle status depending on accelerometers state
        if(!AllOperational) App.Status = stAccFail;
        else if(App.Status == stAccFail) App.Status = stIdle; // All operational

        if(App.Status != stAccFail) {
            // Signal that 'first after power-on' measurement completed
            FirstTime = false;

            // ==== Check if time to sleep ====
            SleepTimeoutCounter--;
            if(SleepTimeoutCounter < 0) {
                SleepTimeoutCounter = 0;
//                Uart.Printf("Zzz\r");
#if defined SLEEP_ENABLED && !defined TRAIN_MODE
                GoSleep();
#endif
            }
#ifndef TRAIN_MODE
            // ==== Check if sign timed out ====
            SignTimeoutCounter--;
            if(SignTimeoutCounter < 0) {
                SignTimeoutCounter = 0;
                App.Status = stIdle;    // Reset indication
                SignCodeNumb = (SignCodeNumb * 100) % 1000000 + 99;
            }
#endif
        } // if not failure
#ifdef TRAIN_MODE
        counter++;
        if(counter >= 10) {
            counter=0;
//            Process.PrintVect();
            for(uint8_t i = 0; i < 21; i++) Uart.Printf("%d,", Process.AccVector[i]);
            Uart.Printf("\r\n");
        }
#endif
    } // while 1
        return 0;
    }

// =============================== App class ===================================
void App_t::Init() {
    Enabled = true;
    Status = stIdle;
    chThdCreateStatic(waAppThread, sizeof(waAppThread), HIGHPRIO, AppThread, NULL);
}
