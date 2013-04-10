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
    //Uart.Printf("PTx\r");
    PktTx.rID = DataPktTx.rID;
    DataPktState = IN_PROGRESS; // Signal that first rpkt was sent
    memcpy(&PktTx.Data, DataPktTx.Ptr, RDATA_CNT);
    DataPktTx.Ptr += RDATA_CNT;
    DataPktTx.Length -= RDATA_CNT;
    if(DataPktTx.Length > 0) PktTx.Srv = R_DIR_GATE2DEV + R_CMD_DATA + R_NXTSLT;
    else PktTx.Srv = R_DIR_GATE2DEV + R_CMD_DATA;
}
void rLevel1_t::PreparePing() {
    PktTx.rID = SLOT2ID(SlotN);
    PktTx.Srv = R_DIR_GATE2DEV + R_CMD_PING;
}

void rLevel1_t::IReportTxOk() {
    *DataPktTx.PState = OK;     // Signal success
    DataPktState = OK;          // No big deal, must be not in_progress/new
    chEvtBroadcast(&IEvtSrcRadioTxEnd);
}
void rLevel1_t::IReportTxFail() {
    *DataPktTx.PState = FAILURE;    // Regardless - last rpkt or somewhere in middle
    DataPktState = FAILURE;
    DataPktTx.Length = -1;          // No more transmission
    chEvtBroadcast(&IEvtSrcRadioTxEnd);
}

// Radiotask
void rLevel1_t::Task() {
    chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by rTmr

    // ==== Sleep ended, transmit new pkt ====
    PktTx.SlotN = SlotN;
    // If not transmitting long pkt already, try to get new data pkt
    if(DataPktTx.Length <= 0)
        if(ITx.Get(&DataPktTx) == OK) DataPktState = NEW;   // Will not be transmitted until correct slot

    // Transmit data if there is data, and if either first pkt already sent, or it is correct slot
    if((DataPktTx.Length > 0) and ((DataPktState == IN_PROGRESS) or InsideCorrectSlot())) PrepareTxPkt();
    else PreparePing();

    DBG1_SET();
    CC.Transmit(&PktTx);
    DBG1_CLR();

    // ==== Pkt transmitted, enter RX ====
    uint8_t RxRslt = CC.Receive(R_RX_WAIT_MS, &PktRx);
    //Uart.Printf("Rx: %u\r", RxRslt);
    // Process result
    if(RxRslt == OK) {
        Uart.Printf("Rx\r");
        // Register answer if pkt received in appropriate slot
        if(InsideCorrectSlot()) Surround.RegisterPkt(SlotN, &PktRx);
        // If data received, put it to queue
        if(PktRx.Srv & R_CMD_DATA) {
            if(IRx.Put(&PktRx) == OK) chEvtBroadcast(&IEvtSrcRadioRx); // Put received pkt in buffer if Data
        } // if data

        // Check reply to data containing pkt
        if(DataPktState == IN_PROGRESS) {
            // Check if ACK
            if(PktRx.Srv & R_ACK) {
                if(DataPktTx.Length <= 0) IReportTxOk();     // If transmission completed
                // Else stay in_progress
            }
            else IReportTxFail();  // No ACK
        } // if data in progress
    } // if ok
    else {  // No answer
        // Register no answer if slot is appropriate
        if(InsideCorrectSlot()) Surround.RegisterNoAnswer(SlotN);
        // If data pkt in progress, fail it
        if(DataPktState == IN_PROGRESS) IReportTxFail();
    } // if rx
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
uint32_t rLevel1_t::ICalcWaitRx_ms(uint8_t RcvdSlot) {
    uint32_t SlotsToWait;
    uint32_t SelfSlot = ID2SLOT(SelfID);
    if(SelfSlot >= RcvdSlot) SlotsToWait = SelfSlot - RcvdSlot;
    else SlotsToWait = RDEVICE_CNT - (RcvdSlot - SelfSlot);
    // Add some reserve
    if(SlotsToWait >= RRX_START_RESERVE) SlotsToWait -= RRX_START_RESERVE;
    //Uart.Printf("Self:%u; Rc: %u; TS: %u\r", ISelfID, RcvdID, TimeslotsToWait);
    // Convert timeslots to ms
    return (SlotsToWait * RTIMESLOT_MS);
}

void rLevel1_t::ISleepIfLongToWait(uint8_t RcvdSlot) {
    // Calculate how long to wait to enter RX
    uint32_t msToWaitRx = ICalcWaitRx_ms(RcvdSlot);
    //Uart.Printf("ms: %u\r", msToWaitRx);
    // Restart EnterRx timer and enter sleep
    chVTReset(&rTmr);
    if(msToWaitRx > RMIN_TIME_TO_SLEEP_MS) {    // If enough time to sleep
        chVTSet(&rTmr, MS2ST(msToWaitRx), rTmrCallback, NULL);  // Start timer
        CC.Sleep();                             // Shutdown CC
        chSchGoSleepS(THD_STATE_SUSPENDED);     // Will wake up by rTmr
    }
}

// Called time after time to discover best gate to communicate with
void rLevel1_t::IDiscovery() {
    //Uart.Printf("Dsc\r");
    bool SomeoneIsNear = false, Retry = false;
    int8_t BestRssi = -126; // Lowest possible
    systime_t fTime;
    uint8_t RxRslt;
    uint32_t GateSlot = 0;
    // Iterate all channels
    for(uint16_t i=0; i<RGATE_CNT; i++) {
        CC.SetChannel(i);
        do {
            RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
            switch(RxRslt) {
                case TIMEOUT:
                    //Uart.Printf("TO\r");
                    Surround.RegisterNoAnswer(i);
                    Retry = false;
                    break;
                case FAILURE:
                    //Uart.Printf("Fl\r");
                    Retry = true; // Someone is near, listen again
                    break;
                case OK:
                    //Uart.Printf("Ok\r");
                    // Check if rpkt is gate-to-device
                    if(PktRx.Srv & R_DIR_GATE2DEV) {
                        Retry = false;              // No need to retry
                        //Uart.Printf("N: %u; Rssi: %d\r", i, pktRx.RSSI);
                        Surround.RegisterPkt(i, &PktRx);
                        SomeoneIsNear = true;
                        if(PktRx.RSSI > BestRssi) {     // Check if switch to this channel
                            GateN = i;                  // Note, number of concentrator != ID of concentrator
                            GateSlot = PktRx.SlotN;     // Save current slot of the gate
                            BestRssi = PktRx.RSSI;
                            fTime = chTimeNow();        // Save current time
                        }
                    }
                    else Retry = true;  // If not gate, try to receive again
                    break;
                //default:
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
        GateSlot += t / RTIMESLOT_MS;
        if(GateSlot > RDEV_TOP_ID) GateSlot -= RDEVICE_CNT;
        ISleepIfLongToWait(GateSlot); // Sleep if needed
    } // if gate is near
}

// Called periodically when gate is successfully discovered
void rLevel1_t::IInSync() {
    uint8_t RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
    if(RxRslt == OK) {
        Uart.Printf("Rx Slot=%u; ID=%u; Srv=%X\r", PktRx.SlotN, PktRx.rID, PktRx.Srv);
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
                    PktTx.Srv = R_DIR_DEV2GATE + R_ACK;     // Send back ACK
                } // if add is ok
                else {  // Buffer oveflow
                    IListenNextSlot = false;                // Nothing to do if buffer overflow
                    PktTx.Srv = R_DIR_DEV2GATE;             // No ACK
                } // if put is ok
            } // if data
            else PktTx.Srv = R_DIR_DEV2GATE + R_ACK;        // Just ping, just ACK

            // ==== Reply ====
            // Reply with Data if TxQueue is not empty
            if(ITx.Get(&DataPktTx) == OK) {
                PktTx.Srv += R_CMD_DATA;
                // Copy payload data
                uint32_t DataSz = (DataPktTx.Length > RDATA_CNT)? RDATA_CNT : DataPktTx.Length;
                memcpy(&PktTx.Data, DataPktTx.Ptr, DataSz);
            }
            DBG1_SET();
            CC.Transmit(&PktTx);
            DBG1_CLR();

            if(!IListenNextSlot) IDiscovery(); // Now for long  time there will be no requests for us => perform discovery.
        } // if pkt is ours
        else {  // Not our pkt => sync failure or our slot is occupied. Sleep if needed.
            // if from dev => sync failure, wait next pkt, do not sleep
            // if from gate and wrong slot => sync failure, sleep(slot)
            // if from gate and correct slot and wrong ID => occupied, sleep(slot)
            if(PktRx.Srv & R_DIR_GATE2DEV) ISleepIfLongToWait(PktRx.SlotN);
        }
    } // if received ok
    else {
        // Reset ongoing reception if any
        if(IListenNextSlot) {
            IListenNextSlot = false;
            IRx.Reset();    // Delete what received
        }
        // Check if we get lost
        if(RxRetryCounter++ > R_IN_SYNC_RETRY_CNT) rMode = rmAlone;
    }
}

uint32_t N=0, Sec=0;

void rLevel1_t::Task() {

    uint8_t RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
    if(RxRslt == OK) N++;

//    switch(rMode) {
//        case rmAlone:
//            CC.Sleep();  // Shutdown CC
//            chThdSleepMilliseconds(RDISCOVERY_PERIOD_MS);
//            IDiscovery();
//            break;
//
//        case rmInSync:
//            IInSync();
//            break;
//    } // switch
}

// Timer callback
void rTmrCallback(void *p) {
    Sec++;
    Uart.Printf("%u; %u\r\n", Sec, N);
    N=0;

    chSysLockFromIsr();
    chVTSet(&rTmr, MS2ST(1000), rTmrCallback, p);
    chSysUnlockFromIsr();


    // ==== Periodic code here ====
    // Resume thread if needed
//    chSysLockFromIsr();
//    if(PThr->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThr);
//    chSysUnlockFromIsr();
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
    chEvtInit(&IEvtSrcRadioTxEnd);
    // Get concentrator channel (0...RCONC_CNT-1). Channel is not ID!
    CC.SetChannel(ASelfID);
    // Timer
    chSysLock();
    chVTSetI(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, NULL);     // Start timer
    chSysUnlock();
#else
    rMode = rmAlone;
    PktTx.rID = SelfID; // Always the same
    chVTSet(&rTmr, MS2ST(1000), rTmrCallback, NULL);  // Start timer
    CC.SetChannel(1);
#endif

    // Init thread. High priority is required to satisfy timings.
    PThr = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}

uint8_t rLevel1_t::AddPktToTx(uint8_t rID, uint8_t *Ptr, int32_t Length, uint8_t *PState) {
    DataPkt_t DataPkt;
    DataPkt.rID = rID;
    DataPkt.Ptr = Ptr;
    DataPkt.Length = Length;
    DataPkt.PState = PState;
    return ITx.PutWithTimeout(&DataPkt, TIME_INFINITE);
}
