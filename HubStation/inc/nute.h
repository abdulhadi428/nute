/*
 * nute.h
 *
 *  Created on: 06.05.2012
 *      Author: kreyl
 */

#ifndef NUTE_H_
#define NUTE_H_

#include "stdint.h"
#include "kl_util.h"
#include "cc1101_rf_settings.h"

// Power levels
#define LOWEST_PWR_LVL_ID   plN20dBm
#define HIGHEST_PWR_LVL_ID  plN6dBm

// Timings
#define PKT_DURATION    99  // ms
#define REPLY_WAITTIME  (PKT_DURATION * 2.5)    // Since TX->RX, wait for 2 PktDurations minimum

// Counts
//#define SEARCH_RETRY_COUNT  4

// Commands and replies
#define NUTE_CMD_PING   0x00

class Tixe_t {
private:

public:
    uint16_t Address;
    bool Online;
    uint8_t LastRxRawRssi;      // Signal level reported last time
    uint8_t TxPwrLevel;         // Which power to use in communication
    ftVoid_Void Callback;
};

enum NuteState_t {nsIdle, nsSearch};
enum SearchState_t {ssDoTx, ssDoRx};

class Nute_t {
private:
    NuteState_t IState;
    SearchState_t ISearchState;
    Tixe_t *IPTixe;
    uint32_t ITmr;
public:
    void Init(void);
    void Task(void);
    void Search(Tixe_t *PTixe);
};


#endif /* NUTE_H_ */
