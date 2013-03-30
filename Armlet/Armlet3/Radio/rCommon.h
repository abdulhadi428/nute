/*
 * rCommon.h
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#ifndef RCOMMON_H_
#define RCOMMON_H_

#include "ch.h"
#include "hal.h"

// Address space
#define RBOTTOM_ID      1000
#define RTOP_ID         1100
#define RID_CNT         (RTOP_ID - RBOTTOM_ID)

// ==== Commands ====
#define RCMD_PING       11

// ==== Pkt_t ====
#define RDATA_CNT       4
struct rPkt_t {
    uint16_t From;
    uint16_t To;
    uint8_t Cmd;
    uint8_t Data[RDATA_CNT];
    int8_t RSSI;                // Received signal level, RX use only
} __attribute__ ((__packed__));
#define RPKT_LEN    (sizeof(rPkt_t)-1)  // Exclude RSSI

// ==== Timings ====
#define R_TX_TIME_MS    2   // Measured value
#define R_RX_WAIT_MS    4   // How long to wait reply
#define RTIMESLOT_MS    (R_TX_TIME_MS + R_RX_WAIT_MS)

// ==== Precise timings timer ====
//#define RTMR    TIM7
//class rTimer_t {
//public:
//    void Stop()  { RTMR->CR1 &= ~TIM_CR1_CEN; }
//    void Start() { RTMR->CR1 |=  TIM_CR1_CEN; }
//    uint16_t GetTime() { return RTMR->CNT; }
//    void SetTime(uint16_t Time) { RTMR->CNT = Time; }
//    void Init() {
//        RTMR->CR1 = TIM_CR1_OPM;    // One-pulse mode
//        RTMR->DIER = TIM_DIER_UIE;  // Update interrupt enabled
//    }
//};



#endif /* RCOMMON_H_ */
