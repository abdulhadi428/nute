/*
 * lvl1_candle.h
 *
 *  Created on: 20.07.2013
 *      Author: kreyl
 */

/*
 * Level 1 of Candle protocol.
 * Level 1 is responsible to rpkt exchange and therefore has
 * high priority.
 */

#ifndef LVL1_CANDLE_H_
#define LVL1_CANDLE_H_

#include "ch.h"
#include "hal.h"
#include "kl_lib_f100.h"

// ============================== Pkt_t ========================================
struct rPkt_t {
    uint8_t R, G, B;
    int8_t RSSI;        // Received signal level, RX use only
} __attribute__ ((__packed__));
#define RPKT_LEN    (sizeof(rPkt_t)-1)  // Exclude RSSI

// ============================== Timings ======================================
#define RX_DURATION_MS  18      // How long to listen
#define RX_PERIOD_MS    3600

// ================================ Level1 =====================================
#define R_CHANNEL       7

class rLevel1_t {
private:
    rPkt_t PktRx;
    EventSource IEvtSrcRadioRx;
public:
    void Init(uint8_t TxPwr);
    rPkt_t PktTx;
    inline void TxDone();
    bool Enabled;
    void RegisterEvtAppearance(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&IEvtSrcRadioRx, PEvtLstnr, EvtMask); }
    // Inner use
    void Task();
};

extern rLevel1_t rLevel1;

#endif /* LVL1_CANDLE_H_ */
