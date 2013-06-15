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

    while(1) {
        DBG1_SET();
        CC.TransmitSync(&rLevel1.PktTx);
        DBG1_CLR();
    }
    return 0;
}

// ================================= Common ====================================
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
    PktTx.TheByte = 0xCA;
    // Init thread. High priority is required to satisfy timings.
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}
