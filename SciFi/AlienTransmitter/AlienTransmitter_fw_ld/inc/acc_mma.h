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

#define ON_TIME     4005    // ms to stay online after trigger
#define ACC_MOTION_THRESHOLD_TOP    1206
#define ACC_MOTION_THRESHOLD_BOTTOM 856


#define ACC_I2C_ADDR    0x1C

// Bunches of registers
struct Accelerations_t {
    uint8_t Status;
    int8_t xMSB, xLSB, yMSB, yLSB, zMSB, zLSB;
} __attribute__ ((packed));
#define ACCELERATIONS_SIZE     7

class Acc_t {
private:
    uint32_t EvtTimer, CheckTimer;
    bool IsTriggered, IsHandled;
    uint8_t RegAddrToRead;
    I2C_Cmd_t i2cCmd;
public:
    Accelerations_t Accelerations;
    void Init(void);
    void Task(void);
    void ReadAccelerations(void) { i2cMgr.AddCmd(i2cCmd); }
    ftVoid_Void EvtTrigger, EvtNoTrigger;
};

// Registers addresses
#define ACC_REG_STATUS          0x00
#define ACC_REG_XYZ_DATA_CFG    0x0E
#define ACC_FF_MT_CFG           0x15
#define ACC_FF_MT_SRC           0x16
#define ACC_FF_MT_THS           0x17
#define ACC_REG_CONTROL1        0x2A



#endif	/* ACC_MMA_H */

