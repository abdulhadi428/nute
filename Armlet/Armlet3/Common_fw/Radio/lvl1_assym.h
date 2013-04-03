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

// ==== Pkt_t ====
#define RDATA_CNT       4
struct rPkt_t {
    uint16_t From;
    uint16_t To;
    //uint16_t PktID; // Need to distinct repeated packets
    uint8_t Cmd;
    uint8_t Data[RDATA_CNT];
    int8_t RSSI;                // Received signal level, RX use only
} __attribute__ ((__packed__));
#define RPKT_LEN    (sizeof(rPkt_t)-1)  // Exclude RSSI


// ==== Address space ====
#define RNO_ID          0
// Devices
#define RDEV_BOTTOM_ID  1000
#define RDEV_TOP_ID     1099
#define RDEVICE_CNT     ((1+RDEV_TOP_ID)-RDEV_BOTTOM_ID)
// Concentrators
#define RCONC_BOTTOM_ID 100
#define RCONC_TOP_ID    104
#define RCONC_CNT       (1+RCONC_TOP_ID-RCONC_BOTTOM_ID)

#ifdef DEVICE
#define RNEIGHBOUR_CNT  RCONC_CNT
#else
#define RNEIGHBOUR_CNT  RDEVICE_CNT
#endif

// ==== Surround ====
struct Neighbour_t {
    int8_t RSSI;
    uint16_t LastToID;
};

class Surround_t {
private:
    Neighbour_t Devs[RNEIGHBOUR_CNT];
public:
    void RegisterPkt(uint16_t N, rPkt_t *pPkt) {
        Devs[N].RSSI = pPkt->RSSI;
        Devs[N].LastToID = pPkt->To;
    }
    void RegisterNoAnswer(uint16_t N) { Devs[N].LastToID = RNO_ID; }
    uint16_t GetID(uint16_t N) { return Devs[N].LastToID; }
};

extern Surround_t Surround;


// ==== Commands ====
#define RCMD_PING       11
#define RCMD_NONE       0xFF

// ==== Timings ====
#define RTIMESLOT_MS    6   // Length of one timeslot
#define R_TX_TIME_MS    2   // Measured value of transmission length
#define R_RX_WAIT_MS    (RTIMESLOT_MS - R_TX_TIME_MS)   // How long to wait reply

// Minimum time worth sleeping
#define RMIN_TIME_TO_SLEEP_MS   12
// Start RX this-number-of-timeslots earlier than exact number
#define RRX_START_RESERVE       1

// In sync, try to receive pkt this number of times before returning to alone mode
#define R_IN_SYNC_RETRY_CNT     4

// Time to wait in discovery mode
#define RDISCOVERY_RX_MS        (RTIMESLOT_MS * 2)
#define RDISCOVERY_PERIOD_MS    504

// ==== Mailboxes ====
#define R_RX_BUF_SZ             18  // number of packets in buffer
class rMaiboxes_t {
private:
    Mailbox IRxMb;   // Here will be received packets; fetch them from upper level
    rPkt_t IRxBuf[R_RX_BUF_SZ];
    msg_t IRxMsgs[R_RX_BUF_SZ];
public:
    void Init();
    // Rx
    uint8_t FetchRxTimeout(rPkt_t *PPkt, uint32_t ms);
    uint8_t FetchRx(rPkt_t *PPkt) { return FetchRxTimeout(PPkt, TIME_INFINITE); }
    uint16_t GetRxCount() { int32_t r = chMBGetUsedCountI(&IRxMb); return (r < 0)? 0 : r; }
    void IAddPkt(rPkt_t *Pkt);   // Inner use
};

extern rMaiboxes_t rLevel1;


// ==== Prototypes ====
void rLvl1_Init(uint16_t ASelfID);

#endif /* LVL1_ASSYM_H_ */
