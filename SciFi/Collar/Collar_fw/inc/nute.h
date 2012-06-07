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
#include "kl_lib.h"
#include "cc1101_rf_settings.h"

// Choose mode - station or tixe
//#define NUTE_MODE_STATION
#ifndef NUTE_MODE_STATION
#define NUTE_MODE_TIXE
#endif

// Power levels
#define LOWEST_PWR_LVL_ID   plN30dBm
#define HIGHEST_PWR_LVL_ID  plP5dBm
//#define HIGHEST_PWR_LVL_ID  plN6dBm
#define DESIRED_RSSI        (-90)       // dBm

// Timings
#ifndef NUTE_MODE_STATION
#define RECALIBRATE_DELAY   7002    // ms; recalibrate if no packet received within this time
#endif
#ifdef CC_BITRATE_10K
// Pkt duration is 45 ms
#define REPLY_WAITTIME  108  // ms; delay between start of transmission and end of answer reception, = 2x PktDuration
#elif defined CC_BITRATE_38K4
// Pkt duration is 15 ms
#define REPLY_WAITTIME  33  // ms; delay between start of transmission and end of answer reception, = 2x PktDuration
#elif defined CC_BITRATE_250K
// Pkt duration is 4 ms
#define REPLY_WAITTIME  11  // ms; delay between start of transmission and end of answer reception, = 2x PktDuration
#endif

// Commands and replies
#define NUTE_CMD_PING           0x00
#define NUTE_RPL_PING           0x01
#define NUTE_RPL_UNSUPPORTED    0xFF

struct Coord_t {    // Size = 2 * 4 = 8 bytes
    int32_t Lattitude, Longtitude;
};

struct Situation_t {
    uint8_t State;
    Time_t Time;
    uint8_t IsFixed, SatCount;
    uint16_t Precision;
    int32_t Lattitude, Longtitude;
} PACKED;

struct Area_t {     // Size = 1 + 1 + 2 * 8 = 18 bytes
    uint8_t TotalCount;
    uint8_t ID;
    Coord_t TopLeft, BottomRight;
} PACKED;


struct Pkt_t {
    uint8_t AddrTo;
    uint8_t AddrFrom;
    uint8_t PwrID;
    uint8_t Cmd;
    union {
        uint8_t Arr[18];
        Situation_t Situation;
        Area_t AreaInfo;
    };
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
    ftVoid_Void Callback;
    Situation_t Situation;
};
#endif

// Main class of protocol
class Nute_t {
private:
    uint32_t ITimer;
    NuteState_t IState;
#ifdef NUTE_MODE_STATION
    SearchState_t ISearchState;
    Tixe_t *IPTixe;
    void DoSearch(void);
#else

#endif
    void AdjustPwr(uint8_t *PPwrID);
public:
    Pkt_t RX_Pkt, TX_Pkt;
    void Init(uint8_t ASelfAddr);
    void Task(void);
    void HandleNewPkt(void);
    void HandleTxEnd(void);
#ifdef NUTE_MODE_STATION
    void Ping(Tixe_t *PTixe);
    void Search(Tixe_t *PTixe);
    //void Cmd(Tixe_t *PTixe, uint8_t ACmd, uint8_t );
#endif
};

extern Nute_t Nute;

#endif /* NUTE_H_ */
