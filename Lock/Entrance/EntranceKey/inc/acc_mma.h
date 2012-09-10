/*
 * File:   acc_mma.h
 * Author: Kreyl
 *
 * Created on June 4, 2011, 11:56 AM
 */

#ifndef ACC_MMA_H
#define	ACC_MMA_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "i2c_mgr.h"

#define ACC_MOTION_TRESHOLD     1.1     // g threshold

#define ACC_I2C_ADDR    0x1C

// Registers addresses
#define ACC_REG_STATUS          0x00
#define ACC_INT_SRC             0x0C
#define ACC_REG_XYZ_DATA_CFG    0x0E
#define ACC_FF_MT_CFG           0x15
#define ACC_FF_MT_SRC           0x16
#define ACC_FF_MT_THS           0x17
#define ACC_FF_MT_COUNT         0x18
#define ACC_REG_CONTROL1        0x2A
#define ACC_REG_CONTROL2        0x2B
#define ACC_REG_CONTROL3        0x2C
#define ACC_REG_CONTROL4        0x2D
#define ACC_REG_CONTROL5        0x2E

//#define ACC_ACCELERATIONS_NEEDED

#ifdef ACC_ACCELERATIONS_NEEDED
struct Accelerations_t {
    uint8_t Status;     // Read to reset latched data
    int8_t xMSB, xLSB, yMSB, yLSB, zMSB, zLSB;
} PACKED;
#define ACCELERATIONS_SIZE     sizeof(Accelerations_t)
#endif

class Acc_t {
private:
    uint32_t EvtTimer, CheckTimer;
    bool IsTriggered, IsHandled;
    bool IEnabled;
    //klPin_t IPin;
    CmdState_t ICmdState;
    void IClearIrq(void) { i2cMgr.AddCmdRegistersRead(ACC_I2C_ADDR, ACC_FF_MT_SRC, 0, 1, 0); }  // Dummy read
    void IWriteReg(uint8_t AAddr, uint8_t AValue) { i2cMgr.WriteRegPoll(ACC_I2C_ADDR, AAddr, AValue); }
public:
#ifdef ACC_ACCELERATIONS_NEEDED
    Accelerations_t Accelerations;
    bool NewData(void) { bool r = (ICmdState == CmdSucceded); ICmdState=CmdFailed; return r; }
    uint32_t ThresholdTop, ThresholdBottom;
    void ReadAccelerations(void) { i2cMgr.AddCmdRegistersRead(ACC_I2C_ADDR, ACC_REG_STATUS, (uint8_t*)&Accelerations, ACCELERATIONS_SIZE, &ICmdState); }
#endif
    uint32_t Delay_ms;
    void Init(void);
    void Task(void);
    void Enable(void)  { IEnabled = true; IsTriggered = false; IsHandled = false; }
    void Disable(void) { IEnabled = false; }
    void EvtTrigger(void);
    void EvtNoTrigger(void);
};


#endif	/* ACC_MMA_H */

