/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "lvl1_assym.h"
#include "cc1101.h"
#include "kl_lib_f2xx.h"

#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOD
#define DBG_PIN1    6
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

rPkt_t pktTx, pktRx;
uint16_t SelfID;
VirtualTimer rTmr;
void rTmrCallback(void *p);
Thread *PThr;
Surround_t Surround;

#ifdef CONCENTRATOR
uint16_t CurrentID; // ID of device to currently speak with
#endif

// ============================ Concentrator task ==============================
#ifdef CONCENTRATOR
static inline void rTask() {
    chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by rTmr

    // Prepare pkt to send
    pktTx.To = CurrentID;
    pktTx.Cmd = RCMD_PING;  // DEBUG
    // Send pkt
    DBG1_SET();
    CC.Transmit(&pktTx);
    DBG1_CLR();
    // Pkt transmitted, enter RX
    RxResult_t RxRslt = CC.Receive(R_RX_WAIT_MS, &pktRx);
    if(RxRslt == rrOk) {
        if(pktRx.To == pktTx.From) {
            // Send data up
        }
    } // if ok
}

// Timer callback
void rTmrCallback(void *p) {
    // ==== Periodic code here ====
    //CurrentID++;  // DEBUG
    if(CurrentID > RTOP_ID) CurrentID = RBOTTOM_ID;

    // Restart the timer, resume thread if needed
    chSysLockFromIsr();
    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, p);
    if(PThr->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThr);
    chSysUnlockFromIsr();
}
#endif

// ============================= Device task ===================================
#ifdef DEVICE
enum rMode_t {rmAlone, rmInSync} rMode;
static uint16_t CntrN;   // Number of concentrator to use

static inline void rDiscovery() {
    //DBG1_SET();
    bool SomeoneIsNear = false;
    int8_t BestRssi = -126; // Lowest possible
    // Iterate all channels
    for(uint16_t i=0; i<RNEIGHBOUR_CNT; i++) {
        CC.SetChannel(i);
        RxResult_t RxRslt = CC.Receive(RDISCOVERY_RX_MS, &pktRx);
        if(RxRslt == rrOk) {
            DBG1_SET();
            Uart.Printf("N: %u; Rssi: %d\r", i, pktRx.RSSI);
            Surround.RegisterPkt(i, &pktRx);
            SomeoneIsNear = true;
            if(pktRx.RSSI > BestRssi) {
                CntrN = i;
                BestRssi = pktRx.RSSI;
            }
        }
        else Surround.Reset(i);
        DBG1_CLR();
    }
    // Decide which channel to use
    //if(SomeoneIsNear == false) rMode = rmAlone; // Silence answers our cries
    //else {
    //rMode = rmInSync;
    DBG1_CLR();
}

static inline void rTask() {
    switch(rMode) {
        case rmAlone:
            chThdSleepMilliseconds(207);
            rDiscovery();
            break;

        case rmInSync:

            break;
    }
}
#endif

static WORKING_AREA(warLvl1Thread, 1024);
static msg_t rLvl1Thread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl1");

    pktTx.From = 1;   // DEBUG
    while(1) rTask();
    return 0;
}

// ================================= Init ======================================
void rLvl1_Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif
    // Variables
    SelfID = 100;   // DEBUG

    // Init radioIC
    CC.Init();
    CC.SetTxPower(Pwr0dBm);

#ifdef CONCENTRATOR
    CurrentID = RBOTTOM_ID; // Device ID to start from
    // Get concentrator channel (0...RCONC_CNT-1). Channel is not ID!
    uint8_t SelfChannel = 0;
    CC.SetChannel(SelfChannel);
    // Timer
    chSysLock();
    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, NULL);     // Start timer
    chSysUnlock();
#else
    rMode = rmAlone;
#endif

    // Init thread. High priority is required to satisfy timings.
    PThr = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}
