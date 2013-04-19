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
#define DBG_PIN1    14
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

static Thread *PThr;
//Surround_t Surround;
rLevel1_t rLevel1;

// ================================ Gate task ==================================
#ifdef GATE
VirtualTimer rTmr;
void rTmrCallback(void *p);

void rLevel1_t::PrepareTxPkt() {
    //Uart.Printf("PTx\r");
    PktTx.rID = DataPktTx.rID;
    // Check if last pkt
    if(DataPktTx.Length <= RLASTPKT_DATASZ) {
        IDataPktState = LAST;
        memcpy(&PktTx.Data, DataPktTx.Ptr, DataPktTx.Length);
        PktTx.Data[RDATA_CNT-1] = DataPktTx.Length; // Set data count in last byte
        PktTx.Srv = R_DIR_GATE2DEV | R_CMD_DATA;    // Do not listen next slot
        DataPktTx.Length = 0;
    }
    else {
        IDataPktState = IN_PROGRESS; // Signal that first rpkt was sent
        memcpy(&PktTx.Data, DataPktTx.Ptr, RDATA_CNT);
        DataPktTx.Ptr += RDATA_CNT;
        DataPktTx.Length -= RDATA_CNT;
        PktTx.Srv = R_DIR_GATE2DEV | R_CMD_DATA | R_NXTSLT; // Listen next slot
    }
}
void rLevel1_t::PreparePing() {
    PktTx.rID = SLOT2ID(SlotN);
    PktTx.Srv = R_DIR_GATE2DEV | R_CMD_PING;
}

void rLevel1_t::IReportTxOk() {
    *DataPktTx.PState = OK;     // Signal success
    IDataPktState = OK;          // No big deal, must be not in_progress/new
    chEvtBroadcast(&IEvtSrcRadioTxEnd);
}
void rLevel1_t::IReportTxFail() {
    *DataPktTx.PState = FAILURE;    // Regardless - last rpkt or somewhere in middle
    IDataPktState = FAILURE;
    chEvtBroadcast(&IEvtSrcRadioTxEnd);
}
void rLevel1_t::IReportTxBusy() {
    *DataPktTx.PState = BUSY;       // Regardless - last rpkt or somewhere in middle
    IDataPktState = FAILURE;
    chEvtBroadcast(&IEvtSrcRadioTxEnd);
}

// Just one pkt at a time
uint8_t rLevel1_t::HandleRxDataPkt() {
    if(IRx.StartNewPkt() != OK) return FAILURE;
    // Get effective length
    uint32_t Length = PktRx.Data[RDATA_CNT-1];
    if(IRx.AddData(PktRx.Data, Length) == OK) {
        IRx.CompletePkt();
        chEvtBroadcast(&IEvtSrcRadioRx);    // Pkt received completely
        return OK;
    }
    else { // Seems like buffer overflow
        IRx.CancelPkt();
        return FAILURE;
    }
}

// Radiotask
void rLevel1_t::Task() {
    chSysLock();
    chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by rTmr
    chSysUnlock();

    // ==== Sleep ended, transmit new pkt ====
    if(++SlotN >= RSLOT_CNT) SlotN = 0;
    PktTx.SlotN = SlotN;
    // If not transmitting long pkt already, try to get new data pkt
    if((IDataPktState == OK) or (IDataPktState == FAILURE))
        if(ITx.Get(&DataPktTx) == OK) IDataPktState = NEW;   // Will not be transmitted until correct slot

    // Transmit data if new and in correct slot, or if first pkt is already sent
    if(((IDataPktState == NEW) and InsideCorrectSlot()) or (IDataPktState == IN_PROGRESS)) PrepareTxPkt();
    else PreparePing();

    DBG1_SET();
    CC.Transmit(&PktTx);
    DBG1_CLR();

    // ==== Pkt transmitted, enter RX ====
    uint8_t RxRslt = CC.Receive(R_RX_WAIT_MS, &PktRx);
    //Uart.Printf("Rx: %u\r", RxRslt);
    // Process result
    if(RxRslt == OK) {
        Uart.Printf("Rx Slot=%u; ID=%u; Srv=%X\r", SlotN, PktRx.rID, PktRx.Srv);
        // Register answer if pkt received in appropriate slot
        //if(InsideCorrectSlot()) Surround.RegisterPkt(SlotN, &PktRx);
        // If data received, put it to queue
        if(PktRx.Srv & R_CMD_DATA) HandleRxDataPkt();

        // Check reply to data containing pkt
        if((IDataPktState == LAST) and (PktRx.Srv & R_ACK)) IReportTxOk();    // If transmission completed
        if(((IDataPktState == IN_PROGRESS) or (IDataPktState == LAST)) and !(PktRx.Srv & R_ACK)) IReportTxBusy();
    } // if ok
    else {  // No answer
        // Register no answer if slot is appropriate
        //if(InsideCorrectSlot()) Surround.RegisterNoAnswer(SlotN);
        // If data pkt in progress, fail it
        if((IDataPktState == IN_PROGRESS) or (IDataPktState == LAST)) IReportTxFail();
    } // if rx
}

// Timer callback
void rTmrCallback(void *p) {
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
    if(msToWaitRx > RMIN_TIME_TO_SLEEP_MS) {    // If enough time to sleep
        CC.Sleep();                             // Shutdown CC
        chThdSleepMilliseconds(msToWaitRx);
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
                    //Surround.RegisterNoAnswer(i);
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
                        //Surround.RegisterPkt(i, &PktRx);
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

uint8_t rLevel1_t::HandleRxDataPkt() {
    if(!IRx.PktInProgress)
        if(IRx.StartNewPkt() != OK) return FAILURE;
    // Check if this pkt is last
    if(!(PktRx.Srv & R_NXTSLT)) {   // No NXT_SLT => last pkt
        // Get effective length
        uint32_t Length = PktRx.Data[RDATA_CNT-1];
        if(IRx.AddData(PktRx.Data, Length) == OK) {
            IRx.CompletePkt();
            chEvtBroadcast(&IEvtSrcRadioRx);    // Pkt received completely
            return OK;
        }
        else { // Seems like buffer overflow
            IRx.CancelPkt();
            return FAILURE;
        }
    }
    // Not a last pkt
    else return IRx.AddData(PktRx.Data, RDATA_CNT);
}

// Called periodically when gate is successfully discovered
void rLevel1_t::IInSync() {
    uint8_t RxRslt = CC.Receive(RDISCOVERY_RX_MS, &PktRx);
    if(RxRslt == OK) {
        //Uart.Printf("Rx Slot=%u; ID=%u; Srv=%X\r", PktRx.SlotN, PktRx.rID, PktRx.Srv);
        RxRetryCounter = 0;     // Something was successfully received, reset counter
        // Check if pkt is ours
        if(PktRx.rID == SelfID) {
            if(PktRx.Srv & R_CMD_DATA) {
                if(HandleRxDataPkt() == OK) PktTx.Srv = R_DIR_DEV2GATE | R_ACK;
                else PktTx.Srv = R_DIR_DEV2GATE;        // No ACK
            }
            else {
                PktTx.Srv = R_DIR_DEV2GATE | R_ACK;     // Just ping, just ACK
                if(IRx.PktInProgress) IRx.CancelPkt();  // Ping instead of data
            }

            // Reply with Data if TxQueue is not empty
            if(ITx.Get(&DataPktTx) == OK) {
                PktTx.Srv |= R_CMD_DATA;
                // Copy payload data
                uint32_t DataSz = (DataPktTx.Length > RLASTPKT_DATASZ)? RLASTPKT_DATASZ : DataPktTx.Length;
                memcpy(&PktTx.Data, DataPktTx.Ptr, DataSz);
                PktTx.Data[RDATA_CNT-1] = DataSz;   // Set data count in last byte
            }
            DBG1_SET();
            CC.Transmit(&PktTx);
            DBG1_CLR();

            // If data was transmitted, report TX ok
            if(PktTx.Srv & R_CMD_DATA) {
                *DataPktTx.PState = OK;     // Signal success
                chEvtBroadcast(&IEvtSrcRadioTxEnd);
            }

            // Enter discovery if no need to listen next slot
            if(!(PktRx.Srv & R_NXTSLT)) IDiscovery();
        } // if pkt is ours
        else {  // Not our pkt => sync failure or our slot is occupied. Sleep if needed.
            // if from dev => sync failure, wait next pkt, do not sleep
            // if from gate and wrong ID and wrong   slot => sync failure, sleep(slot)
            // if from gate and wrong ID and correct slot => occupied, sleep(slot)
            if(PktRx.Srv & R_DIR_GATE2DEV) ISleepIfLongToWait(PktRx.SlotN);
        }
    } // if received ok
    else {
        // Reset ongoing reception if any
        if(IRx.PktInProgress) IRx.CancelPkt();
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
    chEvtInit(&IEvtSrcRadioTxEnd);
    IRx.Init(IPtrBuf, RRX_PTRBUF_SZ, IDataBuf, RRX_DATABUF_SZ);
    ITx.Init(ITxBuf, R_TX_BUF_SZ);

    // ==== General ====
    SelfID = ASelfID;
    IDataPktState = FAILURE;
    // Init radioIC
    CC.Init();
    CC.SetTxPower(PwrMinus6dBm);

#ifdef GATE
    SlotN = 0; // Slot number to start from
    // Get gate channel (0...RCONC_CNT-1). Channel is not ID!
    CC.SetChannel(ASelfID);
    // Timeslot Timer
    chVTSet(&rTmr, MS2ST(RTIMESLOT_MS), rTmrCallback, NULL);  // Start timer
#else
    rMode = rmAlone;
    PktTx.rID = SelfID; // Always the same
    CC.SetChannel(1);
#endif

    // Init thread. High priority is required to satisfy timings.
    PThr = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, rLvl1Thread, NULL);
}

#ifdef GATE
void rLevel1_t::SetID(uint16_t ASelfID) {
    SelfID = ASelfID;
    CC.SetChannel(ASelfID);
}
#endif

uint8_t rLevel1_t::AddPktToTx(uint8_t rID, uint8_t *Ptr, int32_t Length, uint8_t *PState) {
    DataPkt_t DataPkt;
    DataPkt.rID = rID;
    DataPkt.Ptr = Ptr;
    DataPkt.Length = Length;
    DataPkt.PState = PState;
    return ITx.PutWithTimeout(&DataPkt, TIME_INFINITE);
}
