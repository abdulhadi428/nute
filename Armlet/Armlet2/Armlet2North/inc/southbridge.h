/*
 * southbridge.h
 *
 *  Created on: 01.02.2013
 *      Author: kreyl
 */

#ifndef SOUTHBRIDGE_H_
#define SOUTHBRIDGE_H_

#include "peripheral.h"

// ==== SouthBridge class ====
#define SB_GPIO         GPIOC
#define SB_RST          5
#define SB_IN           6
#define SB_OUT          7
#define SB_UART         USART6
#define SB_UART_CLK     (Clk.APB2FreqHz)
#define SB_BAUDRATE     115200

#define SB_DMA_STREAM   STM32_DMA2_STREAM6

//SB to NB msg structure.
struct SbPkt_t {
    uint8_t MsgType;
    union {
        // ATR data, R; size = 1+4 = 5 bytes
        struct {
            uint8_t Status;
            uint32_t Version;
        } ATR PACKED;

        // Beep header data, W; size = 1+5 = 6
        struct {
            uint16_t ChunkCnt;
            BeepChunk_t Chunk0;
        };

        // Keys data, R; size = 9 bytes
        uint8_t Key[KEY_COUNT];

        // Power status data, R; size = 1+1+2 = 4 bytes
        struct {
            uint8_t ExternalPwrOn;  // }
            uint8_t IsCharging;     // } bool replaced by uint8_t to eliminate implicit type conversion
            uint16_t Voltage_mV;
        } PwrStatus PACKED;

        // Infrared
        struct {                    // Transmit params, W; size = 1+2 = 3 bytes
            uint8_t PwrPercent;
            uint16_t Data;
        } IRTransmit PACKED;
        uint16_t IRData;            // Recieved data, R; size = 2 bytes

        // Pills
        uint8_t PillStatus[PILL_COUNT_MAX]; // Statuses of all pills, R; size = 8 bytes
        struct {         // Data to read and write; size = 1+2+2+4 = 9 bytes
            uint8_t PillNumber;
            uint16_t MemAddress;
            uint16_t PayloadDataSz;
            uint8_t Data[PILL_BUF_SZ];
        } Pill PACKED;
    }; // union
} PACKED;

#define SB_PKT_SZ       sizeof(SbPkt_t)

// =========================== Southridge class ================================
struct SBCmd_t {
    uint8_t CmdType;
    void *Ptr;
    uint16_t wData;
} PACKED;
#define SB_CMD_SZ   sizeof(SBCmd_t)

enum SbStatus_t {sbsOn=0, sbsOff=1, sbsError=2};
#define SB_CMDBUF_SZ    16 // Buf size of MailBox
#define SB_UARTBUF_SZ   90
class SouthBridge_t {
private:
    void IInitVars();
    SbPkt_t NTSPkt, STNPkt;
    SBCmd_t CmdBuf[SB_CMDBUF_SZ];     // Queue of commands
    uint8_t IBuf[SB_UARTBUF_SZ];
    SBCmd_t *PCmdRead, *PCmdWrite;
    void Transmit(void *Ptr, uint16_t Length);
    // function-dependant dispatchers
    void DispatchBeep();
public:
    void FetchAndDispatchCmd();
    void PutCmd(SBCmd_t *PCmd);
    SbStatus_t Status;
    uint32_t FwVersion;
    void Init();
    void On();
    void Shutdown();
};
extern SouthBridge_t SouthBridge;

// ==================================== Commands ===============================
#define CMD_NONE                0
// North To South Bridge commands
#define NTS_VIBRO_HEADER        30
#define NTS_VIBRO_SEQUENCE      31
#define NTS_BEEP_HEADER         40
#define NTS_BEEP_SEQUENCE       41
#define NTS_IR_TRANSMIT         50
#define NTS_PILL_READ           60
#define NTS_PILL_WRITE          61

// South To North Bridge commands
#define STN_ATR                 100
#define STN_KEY_STATUS          110
#define STN_PWR_STATUS          120
#define STN_IR_IDLE             150
#define STN_IR_RECEPTION        151
#define STN_PILL_STATUS         160
#define STN_PILL_DATA           161



#endif /* SOUTHBRIDGE_H_ */
