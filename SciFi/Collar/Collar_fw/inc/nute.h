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

// Choose mode - station or tixe
//#define NUTE_MODE_STATION

// Power levels
#define LOWEST_PWR_LVL_ID   plN20dBm
#define HIGHEST_PWR_LVL_ID  plN6dBm
#define DESIRED_RSSI        (-90)       // dBm

// Timings
#define PKT_DURATION    99  // ms
#define REPLY_WAITTIME  (PKT_DURATION * 2.5)    // Since TX->RX, wait for 2 PktDurations minimum

// Commands and replies
#define NUTE_CMD_PING           0x00
#define NUTE_RPL_PING           0x01
#define NUTE_RPL_UNSUPPORTED    0xFF

struct Pkt_t {
    uint8_t AddrTo;
    uint8_t AddrFrom;
    uint8_t PwrID;
    uint8_t Cmd;
    uint8_t Arr[18];
    uint8_t RSSI;
    uint8_t LQI;
} PACKED;

#define CC_PKT_LEN  (sizeof(Pkt_t)-2)

enum NuteState_t {nsIdle, nsSearch, nsWaitingRx, nsTransmitting};


#ifdef NUTE_MODE_STATION
enum SearchState_t {ssDoTx, ssDoRx};

struct Tixe_t {
    uint8_t Address;
    bool IsOnline;
    uint8_t PwrID;
    uint8_t Cmd;
    ftVoid_Void Callback;
};
#endif

// Main class of protocol
class Nute_t {
private:
    NuteState_t IState;
#ifdef NUTE_MODE_STATION
    SearchState_t ISearchState;
    Tixe_t *IPTixe;
    void DoSearch(void);
#else
    void RxHandler(void);
#endif
    uint32_t ITmr;
    void AdjustPwr(uint8_t *PPwrID);
public:
    bool NewPktRcvd;
    Pkt_t RX_Pkt, TX_Pkt;
    void Init(uint8_t ASelfAddr);
    void Task(void);
#ifdef NUTE_MODE_STATION
    void Ping(Tixe_t *PTixe);
    void Search(Tixe_t *PTixe);
    //void Cmd(Tixe_t *PTixe, uint8_t ACmd, uint8_t );
#endif
};

extern Nute_t Nute;

#endif /* NUTE_H_ */
