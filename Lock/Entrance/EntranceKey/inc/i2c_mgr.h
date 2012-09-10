/*
 * File:   i2c_mgr.h
 * Author: Kreyl
 *
 * Created on May 31, 2011, 8:34 PM
 */

#ifndef I2C_MGR_H
#define	I2C_MGR_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "misc.h"

#include "kl_lib.h"

//#define I2C_POLL_ONLY
#define I2C_CLOCK_FREQ          100000  // 100 or 400 kHz

// DMA
#ifndef I2C_POLL_ONLY
#define I2C_DMA_CHNL_TX         DMA1_Channel6
#define I2C_DMA_CHNL_RX         DMA1_Channel7
#define I2C_DMA_FLAG_TC_TX      DMA1_FLAG_TC6
#define I2C_DMA_FLAG_TC_RX      DMA1_FLAG_TC7
#endif

// ================================= Constants =================================
#define I2C_CMD_QUEUE_LENGTH    9
#define I2C_TIMEOUT             50004
#define I2C_TIMEOUT_MS          4

// Error codes
#define I2C_OK                  0
#define I2C_ERR_TIMEOUT         1
#define I2C_ERR_NOMASTER        2
#define I2C_ERR_SLAVE_NACK      3
#define I2C_WAITING             4


// ================================ Data types =================================
struct Buf8_t {
    uint8_t Length;
    uint8_t CurrentItem;
    bool UseInnerBuf;  // Time after time there is no need in outer buffer
    union {
        uint8_t *Buf;
        uint8_t InnerBuf[4];
    };
} PACKED;

enum CmdState_t {CmdPending, CmdWritingAddrTX, CmdWritingAddrRX, CmdWritingOne, CmdWritingMany, CmdReadingOne, CmdReadingMany, CmdSucceded, CmdFailed};

/* Any i2c transaction consists of one or two stages:
 * 1) Start - Addr+Write - [n bytes to write] - Stop
 * 2) Start - Addr+Write - [n bytes to write] - RepStart - Addr+Read - [m bytes to read] - Stop
 */

struct I2C_Cmd_t {
    Buf8_t DataToWrite, DataToRead; // Buffers of data to read or write
    uint8_t Address;                // Device address
    CmdState_t State;
    CmdState_t *StateReport;        // Callback to write cmd state
};

class i2cMgr_t {
private:
#ifndef I2C_POLL_ONLY
    uint32_t Timer;
    bool IsError;
    I2C_Cmd_t *CmdToWrite, *CmdToRead;
    I2C_Cmd_t Commands[I2C_CMD_QUEUE_LENGTH];
    // Task-based functions
    void SendAddrTX(void);
    uint8_t CheckAddrTXSending(void);
    void SendAddrRX(void);
    uint8_t CheckAddrRXSending(void);
    void WriteOneByte(void);
    uint8_t CheckOneByteWriting(void);
    void WriteMany(void);
    uint8_t CheckManyWriting(void);
    void ReadOneByte(void);
    uint8_t CheckOneByteReading(void);
    void ReadMany(void);
    uint8_t CheckManyReading(void);
    void GetNext(void);
    void StopAndGetNext(void);
#endif
    // Polling-based functions
    uint8_t BusyWait(void);
    uint8_t SendStart(void);
    uint8_t SendAddrTXPoll(uint8_t AAddr);
    uint8_t SendAddrRXPoll(uint8_t AAddr);
    uint8_t WriteBytePoll(uint8_t AByte);
public:
    void Init(void);
#ifndef I2C_POLL_ONLY
    void Task(void);
    void AddCmd(I2C_Cmd_t ACmd);
    void AddCmdRegistersRead(uint8_t ADevAddr, uint8_t ARegAddr, uint8_t *PBuf, uint8_t ALength, CmdState_t *PStateReport);
#endif
    uint8_t CmdPoll(I2C_Cmd_t ACmd);	// Perform Cmd in polling way
    uint8_t WriteRegPoll(uint8_t AI2CAddr, uint8_t ARegAddr, uint8_t AValue);

};

extern i2cMgr_t i2cMgr;


#endif	/* I2C_MGR_H */

