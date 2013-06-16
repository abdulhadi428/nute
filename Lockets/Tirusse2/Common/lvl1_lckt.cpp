/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "lvl1_lckt.h"
#include "cc1101.h"

#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOA
#define DBG_PIN1    15
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

// Event masks
#define EVTMASK_RADIO_TX    EVENT_MASK(0)

rLevel1_t rLevel1;

// ================================= Thread ====================================
static WORKING_AREA(warLvl1Thread, 256);
static msg_t rLvl1Thread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl1");

#ifdef TX2
    while(1) {
        DBG1_SET();
        CC.TransmitSync(&rLevel1.PktTx);
        DBG1_CLR();
    }
#else
    while(1) rLevel1.Task();
#endif
    return 0;
}

// Timer callback
static void DisappearTmrCallback(void *p) {
    rLevel1.TimeoutHandler();
}

void rLevel1_t::Task() {
    CC.EnterIdle();
    CC.SetChannel(CHANNEL_ZERO);
    uint8_t Result = CC.ReceiveSync(RX_DURATION_MS, &PktRx);
    if(Result == OK) {
        if(PktRx.TheByte == THE_BYTE) {
            Uart.Printf("Rx %d\r", PktRx.RSSI);
            // Check if changed
            if(!SomethingIsNear) {
                SomethingIsNear = true;
                chEvtBroadcast(&IEvtSrcAppearance);
            }
            // Handle disappearTmr
            chVTReset(&DisappearTmr);
            chVTSet(&DisappearTmr, MS2ST(DISAPPEAR_TIMEOUT_MS), DisappearTmrCallback, NULL);
        }
    }
}

void rLevel1_t::TimeoutHandler() {
    if(SomethingIsNear) {
        SomethingIsNear = false;
        chSysLockFromIsr();
        chEvtBroadcastI(&IEvtSrcAppearance);
        chSysUnlockFromIsr();
    }
}

// ================================= Common ====================================
#ifdef TX2
void rLevel1_t::Init(uint16_t ASelfID) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
#endif
    // Init radioIC
    CC.Init();
    CC.SetChannel(CHANNEL_ZERO + ASelfID-1);
    CC.SetTxPower(Pwr0dBm);
    //CC.SetTxPower(PwrMinus6dBm);
    // Variables
    PktTx.TheByte = THE_BYTE;
    // Init thread. High priority is required to satisfy timings.
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}
#else
void rLevel1_t::Init() {
    Enabled = true;
    chEvtInit(&IEvtSrcAppearance);
    // Init radioIC
    CC.Init();
    // Init thread. High priority is required to satisfy timings.
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}
#endif
