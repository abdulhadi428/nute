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
#define SB_UART_RX_REG  SB_UART->DR

// ========================== Packets =========================

//SB to NB msg structure.
struct SbPktold_t {
    uint8_t MsgType;
    union {
        // ATR data, R; size = 4 bytes
        uint32_t Version;

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

// =========================== Southridge class ================================
enum SbStatus_t {sbsOn=0, sbsOff=1, sbsError=2};
class SouthBridge_t {
private:
    void IInitVars();
public:
    SbStatus_t Status;
    uint32_t FwVersion;
    void Init();
    void On();
    void Shutdown();
};
extern SouthBridge_t SouthBridge;

void SBUartInit();

#endif /* SOUTHBRIDGE_H_ */
