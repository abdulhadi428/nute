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

uint16_t SelfID;
VirtualTimer rTmr;
void rTmrCallback(void *p);
Thread *PThr;
Surround_t Surround;

// ============================ Concentrator task ==============================
#ifdef CONCENTRATOR
// Queue of packets to transmit
struct QueueItem_t {
    rPkt_t TxPkt, RxPkt;
};
static QueueItem_t rQueue[RDEVICE_CNT];
uint16_t CurrentN; // N of block to currently speak with. N != ID

// Radiotask
static inline void rTask() {
    chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by rTmr
    // Sleep ended, transmit new pkt
    rPkt_t *tx = &rQueue[CurrentN].TxPkt;
    rPkt_t *rx = &rQueue[CurrentN].RxPkt;
    // Send pkt
    DBG1_SET();
    CC.Transmit(tx);
    DBG1_CLR();
    // Pkt transmitted, enter RX
    RxResult_t RxRslt = CC.Receive(R_RX_WAIT_MS, rx);

    // Process result
    if(RxRslt == rrOk) {
        Surround.RegisterPkt(CurrentN, rx);
        // If something not trivial transmitted or received (not ping), signal reply up
        if((rx->Cmd != RCMD_PING) or (tx->Cmd != RCMD_PING)) {
            // ...
        }
    } // if ok
    else {  // No answer
        Surround.RegisterNoAnswer(CurrentN);
        // If it was not ping, signal "no answer" up
        if(tx->Cmd != RCMD_PING) {
            // ...
        }
    }
    tx->Cmd = RCMD_PING;   // Reset pkt
}

// Timer callback
void rTmrCallback(void *p) {
    // ==== Periodic code here ====
    CurrentN++;
    if(CurrentN > RDEVICE_CNT) CurrentN = 0;

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
static rPkt_t pktTxAck, pktRx;

// Calculates how long to wait for our timeslot
static uint32_t CalcWaitRx_ms(uint16_t RcvdID) {
    uint16_t TimeslotsToWait;
    if(RcvdID < SelfID) TimeslotsToWait = SelfID - RcvdID;
    return 0;   // FIXME
}

static void SleepIfLongToWait(uint16_t RcvdID) {
    return;
    // Calculate how long to wait to enter RX
    uint32_t msToWaitRx = CalcWaitRx_ms(RcvdID);
    // Restart EnterRx timer and enter sleep
    chVTReset(&rTmr);
    if(msToWaitRx > RMIN_TIME_TO_SLEEP_MS) {    // If enough time to sleep
        chVTSet(&rTmr, MS2ST(msToWaitRx), rTmrCallback, NULL);  // Start timer
        CC.Sleep();                             // Shutdown CC
        chSchGoSleepS(THD_STATE_SUSPENDED);     // Will wake up by rTmr
    }
}

static inline void rDiscovery() {
    bool SomeoneIsNear = false, Retry = false;
    int8_t BestRssi = -126; // Lowest possible
    systime_t fTime;
    RxResult_t RxRslt;
    // Iterate all channels
    for(uint16_t i=0; i<RNEIGHBOUR_CNT; i++) {
        CC.SetChannel(i);
        do {
            RxRslt = CC.Receive(RDISCOVERY_RX_MS, &pktRx);
            switch(RxRslt) {
                case rrTimeout:
                    //Uart.Printf("TO\r");
                    Surround.RegisterNoAnswer(i);
                    Retry = false;
                    break;
                case rrPktFail:
                    //Uart.Printf("Fl\r");
                    Retry = true; // Someone is near, listen again
                    break;
                case rrOk:
                    //Uart.Printf("Ok\r");
                    // Check if sender is concentrator
                    if((pktRx.From >= RCONC_BOTTOM_ID) and (pktRx.From <= RCONC_TOP_ID)) {
                        Retry = false;              // No need to retry
                        //Uart.Printf("N: %u; Rssi: %d\r", i, pktRx.RSSI);
                        Surround.RegisterPkt(i, &pktRx);
                        SomeoneIsNear = true;
                        if(pktRx.RSSI > BestRssi) {     // Check if switch to this channel
                            CntrN = i;                  // Note, number of concentrator != ID of concentrator
                            pktTxAck.To = pktRx.From;   // Maybe, soon we will reply to this guy...
                            BestRssi = pktRx.RSSI;
                            fTime = chTimeNow();        // Save current time
                        }
                    }
                    else Retry = true;  // If not concentrator, try to receive again
                    break;
            } // switch
        } while(Retry);
    } // for

    // Decide which channel to use
    if(SomeoneIsNear == false) rMode = rmAlone; // Silence answers our cries
    else {  // Some concentrator is near
        //Uart.Printf("Dsc: %u\r", CntrN);
        rMode = rmInSync;
        CC.SetChannel(CntrN);
        // Calculate how much time passed since we found concentrator
        uint32_t t = chTimeNow() - fTime;
        // Calculate current ID concentrator is speaking with
        uint32_t fID = Surround.GetID(CntrN) + (t / RTIMESLOT_MS);
        if(fID > RTOP_ID) fID -= RDEVICE_CNT;
        SleepIfLongToWait(fID); // Sleep if needed
    } // if concentrator is near
}

static inline void rInSync() {
    RxResult_t RxRslt = CC.Receive(RDISCOVERY_RX_MS, &pktRx);
    if(RxRslt == rrOk) {
        // Check if pkt is ours
        if(pktRx.To == SelfID) {
            // Reply with ACK if ReplyQueue is empty
            if(1) {
                DBG1_SET();
                CC.Transmit(&pktTxAck);
                DBG1_CLR();
            }
            else {  // Queue is not empty
                //CC.Transmit(&pktTxAck);
            } // if Queue is empty

            // Signal up if Cmd is not Ping
            if(pktRx.Cmd != RCMD_PING) {
                // ...
            }
            Uart.Printf("Pkt\r");
            // Now for long  time there will be no requests. Perform discovery.
            rDiscovery();
        } // if pkt is ours
        else {  // Other's pkt, sleep if needed
            SleepIfLongToWait(pktRx.To);
        }
    } // if received ok
}

static inline void rTask() {
    switch(rMode) {
        case rmAlone:
            CC.Sleep();  // Shutdown CC
            chThdSleepMilliseconds(RDISCOVERY_PERIOD_MS);
            rDiscovery();
            break;

        case rmInSync:
            rInSync();
            break;
    } // switch
}

// Timer callback
void rTmrCallback(void *p) {
    // ==== Periodic code here ====
    // Resume thread if needed
    chSysLockFromIsr();
    if(PThr->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThr);
    chSysUnlockFromIsr();
}
#endif

// ================================= Thread ====================================
static WORKING_AREA(warLvl1Thread, 1024);
static msg_t rLvl1Thread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl1");
    while(1) rTask();
    return 0;
}

// ================================= Init ======================================
void rLvl1_Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif
    // Init radioIC
    CC.Init();
    CC.SetTxPower(Pwr0dBm);

#ifdef CONCENTRATOR
    SelfID = RCONC_BOTTOM_ID;   // DEBUG
    CurrentN = 0; // Block number to start from
    // Setup rqueue
    for(uint16_t i=0; i<RDEVICE_CNT; i++) {
        rQueue[i].TxPkt.Cmd = RCMD_PING;
        rQueue[i].TxPkt.From = SelfID;
        rQueue[i].TxPkt.To = RBOTTOM_ID+i;
    }
    // Get concentrator channel (0...RCONC_CNT-1). Channel is not ID!
    uint8_t SelfChannel = 0;    // DEBUG
    CC.SetChannel(SelfChannel);
    // Timer
    chSysLock();
    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, NULL);     // Start timer
    chSysUnlock();
#else
    SelfID = RBOTTOM_ID;   // DEBUG
    rMode = rmAlone;
    pktTxAck.From = SelfID;
    pktTxAck.Cmd = RCMD_PING;
#endif

    // Init thread. High priority is required to satisfy timings.
    PThr = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}
