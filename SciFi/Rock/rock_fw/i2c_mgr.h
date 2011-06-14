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
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "misc.h"

#include "common.h"

// Inner use defines
#define I2C_DMA_CHNL    DMA1_Channel6
#define I2C_DMA_IRQ     DMA1_Channel6_IRQn
#define I2C_DMA_IT_TC6  DMA1_IT_TC6
#define I2C_DMA_IT_GLB  DMA1_IT_GL6

// Declare DMA1_Channel5 IRQ. Use externC to make it visible from asm file.
#ifdef __cplusplus
extern "C" {
#endif
void DMA1_Channel6_IRQHandler(void);
#ifdef __cplusplus
}
#endif


// ================================= Constants =================================
#define I2C_CMD_QUEUE_LENGTH    9
#define I2C_TIMEOUT             50004

#define I2C_OK                  0
#define I2C_ERR_TIMEOUT         1
#define I2C_ERR_NOMASTER        2
#define I2C_ERR_SLAVE_NACK      3
#define I2C_WAITING             4


// ================================ Data types =================================
// Struct to write or read single register
struct SingleReg_t {
    uint8_t RegAddr;
    uint8_t RegValue;
};
#define I2C_SINGLEREG_SIZE      2

enum CmdState_t {CmdPending, CmdWritingAddrTX, CmdWritingAddrRX, CmdWritingOne, CmdWritingMany, CmdReadingOne, CmdReadingMany, CmdSucceded, CmdFailed};

struct I2C_Cmd_t {
    Buf8_t DataToWrite, DataToRead;     // Buffers of data to read or write
    uint8_t Address;                    // Device address
    CmdState_t State;
};

class i2cMgr_t {
private:
    bool IsBusy;
    uint32_t Timer;
    I2C_Cmd_t Commands[I2C_CMD_QUEUE_LENGTH];
    // Bus handling
    uint8_t SendStart(void);
    void SendAddrTX(void);
    void SendAddrRX(void);
    uint8_t CheckAddrSending(void);

    void WriteOneByte(void);
    uint8_t CheckOneByteWriting(void);
    void WriteMany(void);
    uint8_t CheckManyWriting(void);

    void ReadOneByte(void);
    uint8_t CheckOneByteReading(void);
    void ReadMany(void);
    uint8_t CheckManyReading(void);

    uint8_t SendAddrTXPoll(uint8_t AAddr);
    uint8_t SendAddrRXPoll(uint8_t AAddr);
    void ProcessCmd(void);
    uint8_t WriteOneBytePoll(void);
    uint8_t WriteBufDMA(void);
public:
    // Needed here for Interrupt Handler to have access
    I2C_Cmd_t *CmdToWrite, *CmdToRead;
    uint8_t PrepareToWrite(void);
    void PrepareToRead (void);
    void StopAndGetNext(void);
    // Common use
    void Init(void);
    void Task(void);
    void WriteBufferNoDMA(uint8_t AAddr, uint8_t *ABuffer, uint8_t ABufferSize);
    void AddCmd(I2C_Cmd_t ACmd);
};

extern i2cMgr_t i2cMgr;


#endif	/* I2C_MGR_H */

