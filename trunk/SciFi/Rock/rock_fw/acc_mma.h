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
#include "delay_util.h"

#define ACC_I2C_ADDR    0x1C
#define ACC_INT_PIN     GPIO_Pin_15

#define ACC_PERIOD      100

// Bunches of registers
struct Accelerations_t {
    uint8_t Status;
    int8_t xMSB, xLSB, yMSB, yLSB, zMSB, zLSB;
};
#define ACCELERATIONS_SIZE      sizeof(struct Accelerations_t)

class Acc_t {
    uint8_t RegAddrToRead;
    I2C_Cmd_t i2cCmd;
    uint32_t Timer;
public:
    union {
        Accelerations_t Accelerations;
        uint8_t AccArray[ACCELERATIONS_SIZE];
    };
    void Init(void);
    void Task(void);
    void ReadAccelerations(void) { i2cMgr.AddCmd(i2cCmd); }
};

extern Acc_t Acc;

// Registers addresses
#define ACC_REG_STATUS          0x00
#define ACC_REG_XYZ_DATA_CFG    0x0E
#define ACC_REG_CONTROL1        0x2A



#endif	/* ACC_MMA_H */

