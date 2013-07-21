/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "lvl1_candle.h"
//#include "cc1101.h"

//#define DBG_PINS

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
    while(1) rLevel1.Task();
    return 0;
}

void rLevel1_t::Task() {
    if(!Enabled) {
        chThdSleepMilliseconds(999);
        return;
    }
    // Iterate channels
//    for(uint32_t n = CHANNEL_ZERO; n < (CHANNEL_ZERO + RDEVICE_CNT - 1); n++) {
//        CC.SetChannel(n);
//        uint8_t Result = CC.ReceiveSync(RX_DURATION_MS, &PktRx);
//        if(Result == OK) {
//            if(PktRx.TheByte == THE_BYTE) {
//                Uart.Printf("%u; Lvl=%d\r", n, PktRx.RSSI);
//                // Check if changed
//                if(!SomethingIsNear) {
//                    SomethingIsNear = true;
//                    chEvtBroadcast(&IEvtSrcAppearance);
//                }
//                // Handle disappearTmr
//                chVTReset(&DisappearTmr);
//                chVTSet(&DisappearTmr, MS2ST(DISAPPEAR_TIMEOUT_MS), DisappearTmrCallback, NULL);
//            } // if TheByte
//        } // id rslt ok
//    } // for
//    CC.Sleep();
//    chThdSleepMilliseconds(1800);
}

//void rLevel1_t::TimeoutHandler() {
//    if(SomethingIsNear) {
//        SomethingIsNear = false;
//        chSysLockFromIsr();
//        chEvtBroadcastI(&IEvtSrcAppearance);
//        chSysUnlockFromIsr();
//    }
//}

// ================================= Common ====================================
void rLevel1_t::Init(uint8_t TxPwr) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
#endif
    // Init radioIC
//    CC.Init();
//    CC.SetChannel(R_CHANNEL);
//    CC.SetTxPower(TxPwr);
    // Variables
    chEvtInit(&IEvtSrcRadioRx);
    // Init thread. High priority is required to satisfy timings.
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}
