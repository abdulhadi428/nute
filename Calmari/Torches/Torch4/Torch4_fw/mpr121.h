/*
 * mpr121.h
 *
 *  Created on: 16 сент. 2014 г.
 *      Author: Kreyl
 */

#ifndef MPR121_H_
#define MPR121_H_

#include "ch.h"
#include "kl_lib_f072.h"

#define MPR_I2C                 I2C1
#define MPR_I2C_GPIO            GPIOB

#define MPR_I2C_ADDR            0x5A    // Addr pin connected to VSS

#define MPR_USED_SNS_CNT        5       // ELE0, ELE1, ...
#define MPR_VDD_MV              3300    // Voltage required for autocfg settings
// Electrode configuration:
// Baseline tracking by default, Eleprox disabled, elements 0...4 enabled
#define ELECTRODE_CFG           0b00000101  //

#if 1 // ==== Registers and constants ====
#define MPR_TOTAL_SNS_CNT               13  // ELE0, ELE1, ..., ELEPROX
// Registers
#define MPR_REG_TOUCH_STATUS0           0x00
#define MPR_REG_TOUCH_STATUS1           0x01
#define MPR_REG_OUT_OF_RANGE            0x02
#endif

// Output data
struct mpr121Output_t {
    uint16_t TouchStatus;   // ELE0...ELE11, ELEPROX and Overcurrent bit
    uint16_t OorStatus;     // E0_OOR...E11OOR, PROX_OOR, ARFF, ACFF
    uint16_t EleData[MPR_TOTAL_SNS_CNT];   // Filtered data ELE0...ELE11 and ELEPROX
    uint8_t Baseline[MPR_TOTAL_SNS_CNT];
} __attribute__((__packed__));

struct mpr121Gpio_t {
    uint8_t Ctrl0, Ctrl1, Data, Direction, Enable, Set, Clear, Toggle;
} __attribute__((__packed__));

class mpr121_t {
private:
    mpr121Output_t Output;
    // High-level cmds
    // Middle-level cmds
    uint8_t ClearOvercurrentBit() { return Write(MPR_REG_TOUCH_STATUS1, 0x80); }    // Write 1 to OVCF bit
    // Low-level i2c communication
    uint8_t Write(uint8_t RegAddr, uint8_t AData) { return OK; }  // Single byte at a time only
    uint8_t Read(uint8_t RegAddr, void *PData, uint8_t Length);
public:
    void Init();
};

extern mpr121_t Touch;

#endif /* MPR121_H_ */
