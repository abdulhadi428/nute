/*
 * lvl2.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: g.kruglov
 */

#include "lvl2.h"

rLevel2_t rLevel2;

// =========================== Rx dispatch thread ==============================
static EventListener EvtLstnrRxThd;
static rPkt_t RxPkt, TxPkt;

static WORKING_AREA(warLvl2RxThread, 128);
static msg_t rLvl2RxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl2Rx");
    // Register RadioRx evt
    rLevel1.RegisterEvtRx(&EvtLstnrRxThd, 0);
    while(1) {
        chEvtWaitOne(1);    // Single event, mask is 0x01
        // Pkt received, process it
        uint32_t Cnt = rLevel1.GetRxCount();
        Uart.Printf("RxEvt: Cnt = %u\r", Cnt);

        while(Cnt--) {
            if(rLevel1.GetRxPkt(&RxPkt) == OK) {
                Uart.Printf(" RSSI = %d\r", RxPkt.RSSI);
                // Construct TX pkt and send it in reply
                TxPkt.Cmd++;
                TxPkt.From = rLevel1.SelfID;
                TxPkt.To = RxPkt.From;
                rLevel1.AddPktToTx(&TxPkt);
            }
        }

    } // while 1
    return 0;
}

void rLevel2_t::Init() {
    // Rx rpackets handler
    chThdCreateStatic(warLvl2RxThread, sizeof(warLvl2RxThread), NORMALPRIO, rLvl2RxThread, NULL);
}
