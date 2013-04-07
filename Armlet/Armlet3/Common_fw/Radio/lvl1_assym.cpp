/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "lvl1_assym.h"
#include "cc1101.h"

#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOD
#define DBG_PIN1    6
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

VirtualTimer rTmr;
void rTmrCallback(void *p);
Thread *PThr;
Surround_t Surround;
rLevel1_t rLevel1;

// ============================ Concentrator task ==============================
#ifdef GATE
void rLevel1_t::PrepareTxPkt() {
    memcpy(&PktTx.Data, DataPktTx.Ptr, RDATA_CNT);
    DataPktTx.Ptr += RDATA_CNT;
    DataPktTx.Length -= RDATA_CNT;
    if(DataPktTx.Length > 0) PktTx.Srv = R_DIR_GATE2DEV + R_CMD_DATA + R_NXTSLT;
    else PktTx.Srv = R_DIR_GATE2DEV + R_CMD_DATA;
}
void rLevel1_t::PreparePing() {
    PktTx.rID = SlotN + RDEV_BOTTOM_ID;
    PktTx.Srv = R_DIR_GATE2DEV + R_CMD_PING;
}

// Radiotask
void rLevel1_t::Task() {
    chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by rTmr
    // Sleep ended, transmit new pkt
    PktTx.SlotN = SlotN;
    // If not transmitting long pkt, and if there is something in TX queue, try to get new data pkt
    if(DataPktTx.Length <= 0) ITx.Get(&DataPktTx);
    if(DataPktTx.Length > 0) {
        // If first rpkt of long pkt not sent, send it in correct SlotN
        if(DataPktTx.rID != RNO_ID) {
            if(SlotN == (DataPktTx.rID - RDEV_BOTTOM_ID)) {
                PktTx.rID = DataPktTx.rID;
                DataPktTx.rID = RNO_ID; // Signal that first rpkt was sent
                PrepareTxPkt();
            }
            else PreparePing();  // Incorrect slot, send Ping
        }
        // Since first rpkt of long pkt is already sent, send next part ignoring SlotN
        else PrepareTxPkt();
    }
    else PreparePing(); // no long data, send Ping

    DBG1_SET();
    CC.Transmit(&PktTx);
    DBG1_CLR();

    // Pkt transmitted, enter RX
//    RxResult_t RxRslt = CC.Receive(R_RX_WAIT_MS, rx);
//
//    // Process result
//    if(RxRslt == rrOk) {
//        Surround.RegisterPkt(CurrentN, rx);
//        // If something not trivial transmitted or received (not ping), signal reply up
//        if((rx->Cmd != RCMD_PING) or (tx->Cmd != RCMD_PING)) {
//            // ...
//        }
//    } // if ok
//    else {  // No answer
//        Surround.RegisterNoAnswer(CurrentN);
//        // If it was not ping, signal "no answer" up
//        if(tx->Cmd != RCMD_PING) {
//            // ...
//        }
//    }
//    tx->Cmd = RCMD_PING;   // Reset pkt
}

// Timer callback
void rTmrCallback(void *p) {
    // ==== Periodic code here ====
    rLevel1.IncSlotN();

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

// Calculates how long to wait for our timeslot
uint32_t rLevel1_t::ICalcWaitRx_ms(uint16_t RcvdID) {
    uint16_t TimeslotsToWait;
    if(SelfID >= RcvdID) TimeslotsToWait = SelfID - RcvdID;
    else TimeslotsToWait = RDEVICE_CNT - (RcvdID - SelfID);
    // Add some reserve
    if(TimeslotsToWait >= RRX_START_RESERVE) TimeslotsToWait -= RRX_START_RESERVE;
    //Uart.Printf("Self:%u; Rc: %u; TS: %u\r", ISelfID, RcvdID, TimeslotsToWait);
    // Convert timeslots to ms
    return (TimeslotsToWait * RTIMESLOT_MS);
}

void rLevel1_t::ISleepIfLongToWait(uint16_t RcvdID) {
    // Calculate how long to wait to enter RX
    uint32_t msToWaitRx = ICalcWaitRx_ms(RcvdID);
    //Uart.Printf("ms: %u\r", msToWaitRx);
    // Restart EnterRx timer and enter sleep
    chVTReset(&rTmr);
    if(msToWaitRx > RMIN_TIME_TO_SLEEP_MS) {    // If enough time to sleep
        chVTSet(&rTmr, MS2ST(msToWaitRx), rTmrCallback, NULL);  // Start timer
        CC.Sleep();                             // Shutdown CC
        chSchGoSleepS(THD_STATE_SUSPENDED);     // Will wake up by rTmr
    }
}

void rLevel1_t::IDiscovery() {
    //Uart.Printf("Dsc\r");
    bool SomeoneIsNear = false, Retry = false;
    int8_t BestRssi = -126; // Lowest possible
    systime_t fTime;
    RxResult_t RxRslt;
    // Iterate all channels
    for(uint16_t i=0; i<RGATE_CNT; i++) {
        CC.SetChannel(i);
        do {
            RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
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
                    // Check if rpkt is gate-to-device
                    if(PktRx.Srv & R_DIR_GATE2DEV) {
                        Retry = false;              // No need to retry
                        //Uart.Printf("N: %u; Rssi: %d\r", i, pktRx.RSSI);
                        Surround.RegisterPkt(i, &PktRx);
                        SomeoneIsNear = true;
                        if(PktRx.RSSI > BestRssi) {     // Check if switch to this channel
                            GateN = i;                  // Note, number of concentrator != ID of concentrator
                            BestRssi = PktRx.RSSI;
                            fTime = chTimeNow();        // Save current time
                        }
                    }
                    else Retry = true;  // If not gate, try to receive again
                    break;
            } // switch
        } while(Retry);
    } // for

    // Decide which channel to use
    if(SomeoneIsNear == false) rMode = rmAlone; // Silence answers our cries
    else {  // Some gate is near
        //Uart.Printf("Dsc: %u\r", CntrN);
        rMode = rmInSync;
        CC.SetChannel(GateN);
        // Calculate how much time passed since we found concentrator
        uint32_t t = chTimeNow() - fTime;
        // Calculate current gate's slot
        uint32_t fID = Surround.GetID(GateN) + (t / RTIMESLOT_MS);
        if(fID > RDEV_TOP_ID) fID -= RDEVICE_CNT;
        ISleepIfLongToWait(fID); // Sleep if needed
    } // if gate is near
}

// Called periodically when concentrator is successfully discovered
void rLevel1_t::IInSync() {
    RxResult_t RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
    if(RxRslt == rrOk) {
        //Uart.Printf("Pkt To=%u; From=%u; Cmd=%u\r", pktRx.To, pktRx.From, pktRx.Cmd);
        RxRetryCounter = 0;     // Something was successfully received, reset counter
        // Check if pkt is ours
        if(PktRx.rID == SelfID) {
            // ==== Handle received data ====
            if(PktRx.Srv & R_CMD_DATA) {
                if(IRx.Put(&PktRx) == OK) { // Put received pkt in buffer if Data
                    if(PktRx.Srv & R_NXTSLT) IListenNextSlot = true;
                    else {
                        IListenNextSlot = false;
                        chEvtBroadcast(&IEvtSrcRadioRx);    // Pkt received completely
                    }
                    PktTx.Srv = R_DIR_DEV2GATE + R_ACK;
                } // if add is ok
                else {  // Buffer oveflow
                    IRx.Reset();                // }
                    IListenNextSlot = false;    // } Nothing to do if buffer overflow
                    PktTx.Srv = R_DIR_DEV2GATE; // No ACK
                } // if put is ok
            } // if data
            else PktTx.Srv = R_DIR_DEV2GATE + R_ACK; // Just ping, just ACK

            // ==== Reply ====
            // Reply with Data if ReplyQueue is not empty
            if(ITx.Get(&DataPktTx) == OK) {
                PktTx.Srv += R_CMD_DATA;
                //...
            }
            //Uart.Printf("PktTx To=%u; From=%u; Cmd=%u\r", pktTx.To, pktTx.From, pktTx.Cmd);
            DBG1_SET();
            CC.Transmit(&PktTx);
            DBG1_CLR();

            if(!IListenNextSlot) IDiscovery(); // Now for long  time there will be no requests for us => perform discovery.
        } // if pkt is ours
        else {  // Other's pkt, sleep if needed; seems like sync failure
            if((PktRx.rID >= RDEV_BOTTOM_ID) and (PktRx.rID <= RDEV_TOP_ID))    // rPkt for someone else, seems like sync failure.
                ISleepIfLongToWait(PktRx.rID);                                  // Otherwise stay listening
        }
    } // if received ok
    else {
        // Reset any ongoing receptions
        IRx.Reset();
        IListenNextSlot = false;
        // Check if we get lost
        if(RxRetryCounter++ > R_IN_SYNC_RETRY_CNT) rMode = rmAlone;
    }
}

void rLevel1_t::Task() {
    switch(rMode) {
        case rmAlone:
            CC.Sleep();  // Shutdown CC
            chThdSleepMilliseconds(RDISCOVERY_PERIOD_MS);
            IDiscovery();
            break;

        case rmInSync:
            IInSync();
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
    while(1) rLevel1.Task();
    return 0;
}

// ================================= Common ====================================
void rLevel1_t::Init(uint16_t ASelfID) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif
    // Init Rx & Tx buffers
    chEvtInit(&IEvtSrcRadioRx);
    IRx.Init(IRxBuf, R_RX_BUF_SZ);
    ITx.Init(ITxBuf, R_TX_BUF_SZ);

    // ==== General ====
    SelfID = ASelfID;
    // Init radioIC
    CC.Init();
    CC.SetTxPower(Pwr0dBm);

#ifdef GATE
    SlotN = 0; // Slot number to start from
    // Get concentrator channel (0...RCONC_CNT-1). Channel is not ID!
    CC.SetChannel(ASelfID);
    // Timer
    chSysLock();
    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, NULL);     // Start timer
    chSysUnlock();
#else
    rMode = rmAlone;
    PktTx.rID = SelfID; // Always the same
#endif

    // Init thread. High priority is required to satisfy timings.
    PThr = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}

uint8_t rLevel1_t::AddPktToTx(uint8_t rID, uint8_t *Ptr, int32_t Length, uint8_t *PResult) {
    DataPkt_t DataPkt;
    DataPkt.rID = rID;
    DataPkt.Ptr = Ptr;
    DataPkt.Length = Length;
    DataPkt.PResult = PResult;
    return ITx.PutWithTimeout(&DataPkt, TIME_INFINITE);
}
