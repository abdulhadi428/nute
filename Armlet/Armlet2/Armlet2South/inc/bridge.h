/*
 * bridge.h
 *
 *  Created on: 06.02.2013
 *      Author: kreyl
 */

#ifndef BRIDGE_H_
#define BRIDGE_H_

#define SB_BAUDRATE     115200

// ==================================== Commands ===============================
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

// =================================== Types ===================================
struct SBCmd_t {
    uint8_t CmdType;
    void *Ptr;
    uint32_t DataSz;
} PACKED;
#define SB_CMD_SZ   sizeof(SBCmd_t)

#define SB_CMDBUF_SZ    16  // Buf size of MailBox
#define SB_UARTBUF_SZ   99  // Buf size to transmit and receive. Be careful not to overflow it by changing other parameters!
class Bridge_t {
private:
    SBCmd_t CmdBuf[SB_CMDBUF_SZ];     // Queue of commands
    SBCmd_t *PCmdRead, *PCmdWrite;
public:
    uint8_t RxBuf[SB_UARTBUF_SZ];
    InputQueue iqueue;
    Thread *PTxThread;
    inline bool IsCmdQueueEmpty() { return (PCmdRead->CmdType == CMD_NONE); }
    void DispatchCmd();
    void AddCmd(SBCmd_t *PCmd);
    void Init();
};
extern Bridge_t Bridge;

#endif /* BRIDGE_H_ */
