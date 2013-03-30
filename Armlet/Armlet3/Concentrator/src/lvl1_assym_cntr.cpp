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
//rTimer_t rTmr;
uint16_t CurrentID;
//VirtualTimer rTmr;
//void rTmrCallback(void *p);
//Thread *PThr;


static WORKING_AREA(warLvl1Thread, 1024);
static msg_t rLvl1Thread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl1");

    pktTx.From = 1;   // DEBUG
    RxResult_t RxRslt;

    while(1) {
        // Prepare pkt to send
        pktTx.To = CurrentID;
        pktTx.Cmd = RCMD_PING;
        // Send pkt
        DBG1_SET();
        CC.Transmit(&pktTx);
        DBG1_CLR();
        // Pkt transmitted, enter RX
        RxRslt = CC.Receive(R_RX_WAIT_MS, &pktRx);
        if(RxRslt == rrOk) {
            if(pktRx.To == pktTx.From) {
                // Send data up
            }
        } // if ok

        //chThdSleepMilliseconds(7);
    }
    return 0;
}

// Timer callback
//void rTmrCallback(void *p) {
//    // ==== Periodic code here ====
//    //CurrentID++;  // DEBUG
//    if(CurrentID > RTOP_ID) CurrentID = RBOTTOM_ID;
//
//    // Restart the timer, resume thread if needed
//    chSysLockFromIsr();
//    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, p);
//    if(PThr->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThr);
//    chSysUnlockFromIsr();
//}


void rLvl1_Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif
    // Init Level0
    CurrentID = RBOTTOM_ID;
    CC.Init();
    CC.SetChannel(0);
    CC.SetTxPower(Pwr0dBm);
    // Init thread. High priority is required to satisfy timings.
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);

//    // Timer
//    chSysLock();
//    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, NULL);     // Start timer
//    chSysUnlock();
}

// ============================= Interrupts ====================================
//extern "C" {
//
//CH_IRQ_HANDLER(TIM7_IRQHandler) {
//    CH_IRQ_PROLOGUE();
//    // Clear IRQ pending bit
//    RTMR->SR = (uint16_t)~TIM_SR_UIF;
//
//    //chBSemSignalI(&semIrq);
//
//    CH_IRQ_EPILOGUE();
//}
//
//} // extern c

