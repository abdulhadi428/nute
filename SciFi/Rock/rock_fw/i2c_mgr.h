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
#include "misc.h"

// Inner use defines
#define I2C_DMA_CHNL    DMA1_Channel5
#define I2C_DMA_IRQ     DMA1_Channel5_IRQn

// Declare DMA1_Channel5 IRQ. Use externC to make it visible from asm file.
#ifdef __cplusplus
extern "C" {
#endif
void DMA1_Channel5_IRQHandler(void);
#ifdef __cplusplus
}
#endif


// ================================= Constants =================================
#define I2C_CMD_QUEUE_LENGTH    9
#define I2C_TIMEOUT             50004

// ================================ Data types =================================
struct Buf8_t {
    uint8_t *Buf;
    uint8_t Length;
    uint8_t CurrentItem;
};
typedef Buf8_t* Buf8_p;

enum CmdState_t {CmdPending, CmdWriting, CmdReading, CmdSucceded, CmdFailed};

struct I2C_Cmd_t {
    Buf8_t DataToWrite, DataToRead;     // Buffers of data to read or write
    uint8_t Address;                    // Device address
    CmdState_t State;
};

class i2cMgr_t {
private:
    bool IsBusy;
    I2C_Cmd_t Commands[I2C_CMD_QUEUE_LENGTH];
    uint8_t SendStart(void);
    uint8_t SendAddrTX(uint8_t AAddr);
    uint8_t SendAddrRX(uint8_t AAddr);
    void ProcessCmd(void);
public:
    // Needed here for Interrupt Handler to have access
    I2C_Cmd_t *CmdToWrite, *CmdToRead;
    void PrepareToWrite(void);
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

