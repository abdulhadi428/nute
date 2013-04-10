/*
 * lvl1_assym.h
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

/*
 * Level 1 of assymetric protocol.
 * Level 1 is responsible to sync rpkt exchange and therefore has
 * high priority.
 */

#ifndef LVL1_ASSYM_H_
#define LVL1_ASSYM_H_

#include "ch.h"
#include "hal.h"
#include "main.h"   // To distinct between concentrator and device
#include "kl_lib_f2xx.h"
#include "kl_buf.h"

// ==== Pkt_t ====
#define RDATA_CNT       17
struct rPkt_t {
    uint8_t SlotN;      // Number of timeslot
    uint8_t rID;        // Device ID
    uint16_t Srv;       // Service bits
    uint8_t Data[RDATA_CNT];
    int8_t RSSI;        // Received signal level, RX use only
} __attribute__ ((__packed__));
#define RPKT_LEN    (sizeof(rPkt_t)-1)  // Exclude RSSI

// Srv bit masks
#define R_DIR_GATE2DEV  0x8000
#define R_DIR_DEV2GATE  0x0000
// Cmd
#define R_CMD_MASK      0x7000
#define R_CMD_PING      0x0000
#define R_CMD_DATA      0x1000

#define R_NXTSLT        0x0800  // Listen next slot
//#define R_RPIDTX
///#define R_RPIDRX
#define R_ACK           0x0010
//#define R_ACKDYN
//#define R_RPIDDYN

// ======= Address space =======
#define RNO_ID          0   // When device has no ID
// Devices
#define RDEV_BOTTOM_ID  10
#define RDEV_TOP_ID     109
#define RDEVICE_CNT     ((1+RDEV_TOP_ID)-RDEV_BOTTOM_ID)

// Gates
#define RGATE_CNT       5

// Slot count
#define RDYN_SLOT_CNT   0   // Count of dynamic slots
#define RSLOT_CNT       (RDEVICE_CNT + RDYN_SLOT_CNT) // Total slots count
#define ID2SLOT(id)     (id - RDEV_BOTTOM_ID)
#define SLOT2ID(slot)   (slot + RDEV_BOTTOM_ID)

#ifdef DEVICE
#define RNEIGHBOUR_CNT  RGATE_CNT
#else
#define RNEIGHBOUR_CNT  RDEVICE_CNT
#endif

// ==== Surround ====
struct Neighbour_t {
    uint16_t ID;
    int8_t RSSI;
};

class Surround_t {
private:
    Neighbour_t Devs[RNEIGHBOUR_CNT];
public:
    void RegisterPkt(uint16_t N, rPkt_t *pPkt) {
        Devs[N].ID = pPkt->rID;
        Devs[N].RSSI = pPkt->RSSI;
    }
    void RegisterNoAnswer(uint16_t N) { Devs[N].ID = RNO_ID; }
    uint16_t GetID(uint16_t N) { return Devs[N].ID; }
};

extern Surround_t Surround;

// ============================== Timings ======================================
#define R_TX_WAIT_MS    9   // Measured value of transmission length
#define R_RX_WAIT_MS    1   // How long to wait reply to start
#define RTIMESLOT_MS    (R_TX_WAIT_MS + R_RX_WAIT_MS)   // Length of one timeslot

// Minimum time worth sleeping
#define RMIN_TIME_TO_SLEEP_MS   12
// Start RX this-number-of-timeslots earlier than exact number
#define RRX_START_RESERVE       1

// In sync, try to receive pkt this number of times before returning to alone mode
#define R_IN_SYNC_RETRY_CNT     4

// Time to wait in discovery mode
#define RDISCOVERY_RX_MS        (RTIMESLOT_MS * 2)
#define RDISCOVERY_PERIOD_MS    504

// ================================ Level1 =====================================
// Data Packet item
struct DataPkt_t {
    uint8_t rID;
    uint8_t *Ptr;
    int32_t Length;
    uint8_t *PState;
};

#define R_RX_BUF_SZ             36  // Size of buffer for Rx pkts
#define R_TX_BUF_SZ             36  // Size of buffer for Tx pkts
class rLevel1_t {
private:
    EventSource IEvtSrcRadioRx, IEvtSrcRadioTxEnd;
    // ==== Rx ====
    rPkt_t PktRx;
    rPkt_t IRxBuf[R_RX_BUF_SZ];
    CircBuf_t<rPkt_t> IRx;
    // ==== Tx ====
    rPkt_t PktTx;
    DataPkt_t DataPktTx;
    DataPkt_t ITxBuf[R_TX_BUF_SZ];
    CircBufSemaphored_t<DataPkt_t> ITx;
    inline void PrepareTxPkt();
#ifdef DEVICE
    uint8_t RxRetryCounter; // to check if we get lost
    bool IListenNextSlot;
    uint16_t GateN;   // Number of concentrator to use. Note, Number != ID.
    inline void IInSync();
    inline void IDiscovery();
    uint32_t ICalcWaitRx_ms(uint8_t RcvdSlot);
    void ISleepIfLongToWait(uint8_t RcvdSlot);
    inline void PrepareAck();
#endif
#ifdef GATE
    uint8_t SlotN;
    uint8_t DataPktState;
    inline void PreparePing();
    inline bool InsideCorrectSlot() { return (SlotN == ID2SLOT(DataPktTx.rID)); }
    inline void IReportTxOk();
    inline void IReportTxFail();
#endif
public:
    uint8_t SelfID;
    void Init(uint16_t ASelfID);
    // Rx
    uint8_t GetRxPkt(rPkt_t *PPkt) { return IRx.Get(PPkt); }
    uint32_t GetRxCount() { return IRx.GetFullSlotsCount(); }
    void RegisterEvtRx(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegister(&IEvtSrcRadioRx, PEvtLstnr, EvtMask); }
    // Tx
    uint8_t AddPktToTx(uint8_t rID, uint8_t *Ptr, int32_t Length, uint8_t *PState);
    uint32_t GetTxCount() { return ITx.GetFullSlotsCount(); }
    void RegisterEvtTx(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&IEvtSrcRadioTxEnd, PEvtLstnr, EvtMask); }
    // Inner use
    inline void Task();
#ifdef GATE
    inline void IncSlotN() { if(++SlotN >= RSLOT_CNT) SlotN = 0; }
#endif
};

extern rLevel1_t rLevel1;

#endif /* LVL1_ASSYM_H_ */
