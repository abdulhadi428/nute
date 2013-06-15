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
#define EVTMASK_RADIO_RX    EVENT_MASK(0)
#define EVTMASK_NEW_SLOT    EVENT_MASK(1)

static Thread *PThr;
static VirtualTimer rTmr;
rLevel1_t rLevel1;
static EventSource IEvtSrcNewSlot;
static EventListener EvtLstnrRadioRx, EvtListenerNewSlot;

// ============================== Implementation ===============================
// Timer callback
void rTmrCallback(void *p) {
    // Restart the timer, resume thread if needed
    chSysLockFromIsr();
    chVTSetI(&rTmr, MS2ST(TIMESLOT_MS), rTmrCallback, p);
    chEvtBroadcastI(&IEvtSrcNewSlot);
    chSysUnlockFromIsr();
}

void rLevel1_t::NewSlot() {
    DBG1_CLR();
    // Handle Slot & Cycle
    if(++Slot > SLOT_CNT) { // New cycle
        Slot = 1;
        if(++Cycle >= CYCLE_CNT) {
            Cycle = 0;
            TimeOwner = SelfID;     // Reset timeOwner
        }
    }
    // Check if transmit
    if(Slot == SelfID) {
        // TODO: check if channel is clear
        PktTx.Cycle = Cycle;
        PktTx.TimeOwner = TimeOwner;
//        DBG1_SET();
        CC.TransmitAsync(&PktTx);
        DBG1_SET();
//        DBG1_CLR();
    }
    else {
        if(Cycle == 0) {
            if(CC.State != ccReceiving) CC.ReceiveAsync();
        }
        else {  // Cycle != 0
            if(CC.State != ccSleeping) CC.Sleep();
        }
    }
}

void rLevel1_t::NewRxPkt() {
    bool RxOk = CC.ReadFIFO(&PktRx);

    //Uart.Printf("Sf: ID=%u, C=%u, Ow=%u\r", SelfID, Cycle, TimeOwner);
    Uart.Printf("Rx: ID=%u, C=%u, Ow=%u\r", PktRx.ID, PktRx.Cycle, PktRx.TimeOwner);
    if(RxOk == OK) {
        if(PktRx.TimeOwner < TimeOwner) { // Sync with lower ID
            chVTReset(&rTmr);
            Cycle = PktRx.Cycle;
            Slot = PktRx.ID;
            TimeOwner = PktRx.ID;
            chVTSet(&rTmr, MS2ST(SLOT_REMAINDER_MS), rTmrCallback, NULL);  // Start timer
        }
    }
    //Uart.Printf("Sf: ID=%u, C=%u, Ow=%u\r\r", SelfID, Cycle, TimeOwner);
}

// ================================= Thread ====================================
static WORKING_AREA(warLvl1Thread, 256);
static msg_t rLvl1Thread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl1");
    // Events
    uint32_t EvtMsk;
    chEvtRegisterMask(&IEvtSrcNewSlot, &EvtListenerNewSlot, EVTMASK_NEW_SLOT);
    CC.RegisterEvtRx(&EvtLstnrRadioRx, EVTMASK_RADIO_RX);

    while(1) {
        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_NEW_SLOT);
        if(EvtMsk & EVTMASK_RADIO_RX) rLevel1.NewRxPkt();
        else if(EvtMsk & EVTMASK_NEW_SLOT) rLevel1.NewSlot();
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
    CC.SetChannel(207);
    CC.SetTxPower(PwrPlus5dBm);
    //CC.SetTxPower(PwrMinus6dBm);

    // Events
    chEvtInit(&IEvtSrcNewSlot);

    // Variables
    Slot = 1;   // Slot number to start from
    Cycle = 0;  // Cycle to start from
    SelfID = ASelfID;
    PktTx.ID = ASelfID;
    // Timeslot Timer
    chVTSet(&rTmr, MS2ST(TIMESLOT_MS), rTmrCallback, NULL);  // Start timer

    // Init thread. High priority is required to satisfy timings.
    PThr = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}
