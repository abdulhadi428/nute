/*
 * lvl1_lckt.h
 *
 *  Created on: 12.06.2013
 *      Author: kreyl
 */

/*
 * Level 1 of locket protocol.
 * Level 1 is responsible to sync rpkt exchange and therefore has
 * high priority.
 */

#ifndef LVL1_LCKT_H_
#define LVL1_LCKT_H_

#include "ch.h"
#include "hal.h"
#include "main.h"   // To distinct between concentrator and device
#include "kl_lib_f100.h"
//#include "kl_buf.h"

// ============================== Pkt_t ========================================
struct rPkt_t {
    uint8_t ID;        // Device ID
    uint8_t Cycle;
    uint8_t TimeOwner;
    int8_t RSSI;        // Received signal level, RX use only
} __attribute__ ((__packed__));
#define RPKT_LEN    (sizeof(rPkt_t)-1)  // Exclude RSSI

// =========================== Address space ===================================
#define R_NO_ID         -1
// Devices
#define RDEV_BOTTOM_ID  1
#define RDEV_TOP_ID     7
#define RDEVICE_CNT     ((1+RDEV_TOP_ID)-RDEV_BOTTOM_ID)

// Slot count
#define SLOT_CNT        RDEVICE_CNT // Total slots count
#define ID2SLOT(id)     (id - RDEV_BOTTOM_ID)
#define SLOT2ID(slot)   (slot + RDEV_BOTTOM_ID)

// ============================== Cycles =======================================
#define CYCLE_CNT       4


// ============================== Timings ======================================
#define TIMESLOT_MS         20  // Length of one timeslot
#define SLOT_REMAINDER_MS   4   // Time between end of pkt and end of slot

// ================================ Level1 =====================================
class rLevel1_t {
private:
    uint8_t SelfID;
    uint8_t Slot, Cycle;
    uint8_t TimeOwner;
    //EventSource IEvtSrcRadioRx;
    // ==== Rx ====
    rPkt_t PktRx;       // Local rPkt to receive
    inline uint8_t HandleRxDataPkt();
    // ==== Tx ====
    rPkt_t PktTx;       // Local rPkt to transmit
public:
    void Init(uint16_t ASelfID);
//    void RegisterEvtRx(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&IEvtSrcRadioRx, PEvtLstnr, EvtMask); }
    // Inner use
    inline void Task();
    inline void NewSlot();
    inline void NewRxPkt();
};

extern rLevel1_t rLevel1;

#endif /* LVL1_LCKT_H_ */
