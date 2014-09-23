/*
 * mpr121.h
 *
 *  Created on: 16 сент. 2014 г.
 *      Author: Kreyl
 */

#ifndef MPR121_H_
#define MPR121_H_

#include "ch.h"

#define MPR_I2C                 I2C1
#define MPR_I2C_GPIO            GPIOB

#define MPR_I2C_ADDR            0x5A    // Addr pin connected to VSS

#define MPR_USED_SNS_CNT        5       // ELE0, ELE1, ...
#define MPR_VDD_MV              3300    // Voltage required for autocfg settings
// Electrode configuration:
// Baseline tracking by default, Eleprox disabled, elements 0...4 enabled
#define ELECTRODE_CFG           0b00000101  //

#if 1 // ==== Registers and constants ====
#define MPR_TOTAL_SNS_CNT       13  // ELE0, 1, ..., 12, ELEPROX
// Registers
#define MPR_REG_TOUCH_STATUS0           0x00
#define MPR_REG_TOUCH_STATUS1           0x01
#define MPR_REG_OUT_OF_RANGE            0x02
#define MPR_REG_BASELINE_FILTERING_CTRL 0x2B
#define MPR_REG_ELEPROX_FILTERING_CTRL  0x35
#define MPR_REG_TOUCH_RELEASE_THRESHOLD 0x41
#define MPR_REG_DEBOUNCE                0x5B
#define MPR_REG_CDC_CDT_CFG             0x5C
#define MPR_REG_ELECTRODE_CFG           0x5E    // Must be written last
#define MPR_REG_ELECTRODE_CURRENT       0x5F
#define MPR_REG_CHARGE_TIME             0x6C
#define MPR_REG_AUTO_CFG_CTRL0          0x7B
#define MPR_REG_RESET                   0x80
#define MPR_REG_GPIO                    0x73

#define MPR_RESET_VALUE                 0x63
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
    uint8_t Write(uint8_t RegAddr, uint8_t AData);  // Single byte at a time only
    uint8_t Read(uint8_t RegAddr, void *PData, uint8_t Length);
public:
    void Init();
};


#endif /* MPR121_H_ */
