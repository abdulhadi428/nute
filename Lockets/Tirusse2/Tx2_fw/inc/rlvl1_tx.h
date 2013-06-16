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
    uint8_t TheByte;
    int8_t RSSI;        // Received signal level, RX use only
} __attribute__ ((__packed__));
#define RPKT_LEN    (sizeof(rPkt_t)-1)  // Exclude RSSI

// =========================== Address space ===================================
// Devices
#define RDEV_BOTTOM_ID  1
#define RDEV_TOP_ID     7
#define RDEVICE_CNT     ((1+RDEV_TOP_ID)-RDEV_BOTTOM_ID)
#define CHANNEL_ZERO    200

// ============================== Timings ======================================
#define TIMESLOT_MS         20  // Length of one timeslot

// ================================ Level1 =====================================
class rLevel1_t {
private:
public:
    void Init(uint16_t ASelfID);
    // Inner use
    rPkt_t PktTx;       // Local rPkt to transmit
    inline void TxDone();
};

extern rLevel1_t rLevel1;

#endif /* LVL1_LCKT_H_ */
