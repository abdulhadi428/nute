/*
 * mpr121.h
 *
 *  Created on: 16 сент. 2014 г.
 *      Author: Kreyl
 */

#ifndef MPR121_H_
#define MPR121_H_

#include "ch.h"
#include "hal.h"
#if defined STM32L1XX_HD || defined STM32L1XX_MD
#include "kl_lib_L15x.h"
#else
#include "kl_lib_f072.h"
#endif

// I2C
#define MPR_I2C                 I2C1
#define MPR_I2C_GPIO            GPIOB
#define MPR_SCL_PIN             8
#define MPR_SDA_PIN             9
#define MPR_I2C_BITRATE_HZ      100000
#define MPR_DMATX               STM32_DMA1_STREAM6
#define MPR_DMARX               STM32_DMA1_STREAM7

// IRQ
#define MPR_IRQ_GPIO            GPIOB
#define MPR_IRQ_PIN             10

// I2C address
#define MPR_I2C_ADDR            0x5A    // Addr pin connected to VSS

#define MPR_USED_SNS_CNT        5       // ELE0, ELE1, ...
#define MPR_SAMPLE_INTRVL_MS    16      // 16 by default; 1,2,4,...16,32,...128
#define MPR_SAMPLE_INTRVL_MS_LP 128     // Sample interval for low power mode
#define MPR_VDD_MV              3300    // Voltage required for autocfg settings


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

class mpr121_t {
private:
    mpr121Output_t Output;
    PinIrq_t Irq;
    Thread *PThd;
    // High-level cmds
    // Middle-level cmds
    void ReadOutput() { Read(MPR_REG_TOUCH_STATUS0, &Output, sizeof(mpr121Output_t)); }
    void ClearOvercurrentBit() { Write(MPR_REG_TOUCH_STATUS1, 0x80); }    // Write 1 to OVCF bit
    // Low-level i2c communication
    void Write(void *PRegData, uint8_t Length); // PRegdata contains reg addr first
    void Write(uint8_t RegAddr, uint8_t AData);
    void Read(uint8_t RegAddr, void *PData, uint8_t Length);
    void SetSampleInterval(uint8_t SampleInterval);
    void Reset() { Write(0x80, 0x63); }
public:
    void Init();
    void On();
    void Off();
    // Inner use
    void ITask();
};

extern mpr121_t Touch;

#endif /* MPR121_H_ */
