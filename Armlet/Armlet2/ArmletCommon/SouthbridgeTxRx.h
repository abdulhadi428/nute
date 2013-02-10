/*
 * SouthbridgeTxRx.h
 *
 *  Created on: 09.02.2013
 *      Author: kreyl
 */

#ifndef SOUTHBRIDGETXRX_H_
#define SOUTHBRIDGETXRX_H_

#include <stdint.h>
#include "ch.h"

// Buf size to transmit and receive. Be careful not to overflow it by changing other parameters!
#define SB_UARTBUF_SZ           99
#define SB_CMDBUF_SZ            16  // Buf size of cmd queue

// =============================== Commands ====================================
#define CMD_NONE                0x00
#define CMD_ADDITIONAL_DATA     0x01
// North To South Bridge commands
#define NTS_VIBRO               0x10
#define NTS_BEEP                0x20
#define NTS_IR_TRANSMIT         0x30
#define NTS_PILL_READ           0x40
#define NTS_PILL_WRITE          0x41

// South To North Bridge commands
#define STN_ATR                 0x70
#define STN_KEY_STATUS          0x80
#define STN_PWR_STATUS          0x90
#define STN_IR_IDLE             0xA0
#define STN_IR_RECEPTION        0xB0
#define STN_PILL_STATUS         0xC0
#define STN_PILL_DATA           0xC1

// ============================= Transmitter ===================================
struct SBCmd_t {
    uint8_t CmdType;
    void *Ptr;
    uint16_t DataSz;
} __attribute__ ((__packed__));
#define SB_CMD_SZ   sizeof(SBCmd_t)

class Transmitter_t {
private:
    // Transmitting
    uint32_t BufCounter;
    uint8_t TxBuf[SB_UARTBUF_SZ];
    uint8_t *PArray;
    uint16_t ArraySz;
    void AddByte(uint8_t Byte) { TxBuf[BufCounter++] = Byte; }
    uint8_t PackByte(uint8_t Byte);
    void PackWord(uint16_t Word) {PackByte(Word & 0xFF); PackByte((Word >> 8) & 0xFF); }
    void PackArray();
    void Transmit();
    // Command queue
    SBCmd_t CmdBuf[SB_CMDBUF_SZ];     // Queue of commands
    SBCmd_t *PCmdRead, *PCmdWrite;
public:
    inline bool IsCmdQueueEmpty() { return (PCmdRead->CmdType == CMD_NONE); }
    Thread *PThread;
    void DispatchCmd();
    void Init();
    void AddCmd(SBCmd_t *PCmd);
};

extern "C" {
void IrqSBTxCompleted(void *p, uint32_t flags);
} // extern C

// ================================ Receiver ===================================
enum FeederRetVal_t {frvOk, frvNoMore};

// Abstract class of dispatcher
class Feeder_t {
protected:
    uint8_t *PFeedData;
    uint32_t FdrByteCnt;
public:
    virtual FeederRetVal_t FeedStart(uint8_t Byte) = 0;
    virtual FeederRetVal_t FeedData(uint8_t Byte) = 0;
    virtual void FeederEndPkt() = 0;
};

// Receiver class
enum RcvrState_t {rsStart, rsMsgType, rsLength1, rsLength2, rsData};
class Rcvr_t {
private:
    RcvrState_t RState;
    uint8_t IMsgType;
    uint16_t ILength;
    bool WasEE; // Unpacker
    Feeder_t *PFeeder;
    void SortByte(uint8_t b);
    void EndOfPkt();
    uint8_t RxBuf[SB_UARTBUF_SZ];
public:
    InputQueue iqueue;
    void ProcessByte(uint8_t b);
    void Reset() { RState = rsStart; PFeeder = 0; }
    void Init();
};

extern Transmitter_t Transmitter;
extern Rcvr_t Rcvr;

extern Feeder_t* const PFeeders[];
extern const uint8_t FeederCnt;


#endif /* SOUTHBRIDGETXRX_H_ */
