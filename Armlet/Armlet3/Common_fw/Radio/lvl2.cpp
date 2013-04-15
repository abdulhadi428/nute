/*
 * lvl2.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: g.kruglov
 */

#include "lvl2.h"

#include "evt_mask.h"

rLevel2_t rLevel2;

// =========================== Rx dispatch thread ==============================
static EventListener EvtLstnrRxThd;

DataPktPtr_t DataPkt;
uint8_t RxPktData[180];

static WORKING_AREA(warLvl2RxThread, 128);
static msg_t rLvl2RxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("rLvl2Rx");

    // Init Rx Pkt
    DataPkt.Ptr = RxPktData;

    // Register RadioRx evt
    rLevel1.RegisterEvtRx(&EvtLstnrRxThd, EVTMASK_RADIO_RX);

    while(1) {
        chEvtWaitOne(EVTMASK_RADIO_RX);
        // Pkt received, process it
        uint32_t Cnt = rLevel1.GetRxPktCount();

        while(Cnt--) {
            if(rLevel1.GetRxPkt(&DataPkt) == OK) {
                Uart.Printf("RxEvt: Cnt=%u, Sz=%u\r", Cnt, DataPkt.Length);
                Uart.Printf("> %A\r", DataPkt.Ptr, DataPkt.Length);
            }
        }

    } // while 1
    return 0;
}

void rLevel2_t::Init() {
    // Rx rpackets handler
    chThdCreateStatic(warLvl2RxThread, sizeof(warLvl2RxThread), NORMALPRIO, rLvl2RxThread, NULL);
}

void rLevel2_t::Transmit(uint8_t DevID, uint8_t *Ptr, int32_t Length, uint8_t PResult) {
//    chSemWait(&AddPktSem); // Wait here if other thread is spinning below
//    // Now add pkt to queue
//    rPkt_t Pkt;
//    Pkt.rID = DevID;
//    // Slot number will be set by Level1
//    while(Length > 0) {
//        memcpy(&Pkt.Data, Ptr, RDATA_CNT);
//        Ptr += RDATA_CNT;
//        Length -= RDATA_CNT;
//        if(Length > 0) Pkt.Srv = R_DIR_GATE2DEV + R_NXTSLT;
//        else Pkt.Srv = R_DIR_GATE2DEV;
//        rLevel1.AddPktToTx(&Pkt);
//    }

}
