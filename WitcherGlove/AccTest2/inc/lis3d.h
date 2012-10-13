/*
 * lis3d.h
 *
 *  Created on: 09.10.2012
 *      Author: kreyl
 */

#ifndef LIS3D_H_
#define LIS3D_H_

#include "kl_lib.h"

/* PA0 = 4 = SPC/SCL
 * PA1 = 6 = SDI/SDO/SDA
 * PA2 = 7 = SDO/SA0
 * PA3 = 8 = CS
 * PA4 = 11 = IRQ
 */

#define ACC_CLK     0
#define ACC_DODI    1
//#define ACC_MISO    2
#define ACC_CS      3
#define ACC_IRQ     4

#define ACC_FIFO_SZ (uint8_t)10  // Count of FIFO stored data, MAX=31

// Possible Datarate values
#define ODR_1HZ         0x10
#define ODR_10HZ        0x20
#define ODR_25HZ        0x30
#define ODR_50HZ        0x40
#define ODR_100HZ       0x50
#define ODR_200HZ       0x60
#define ODR_400HZ       0x70
#define ODR_1250HZ      0x90

// Choose datarate here
#define ACC_DATARATE    ODR_10HZ

enum DataDir_t {ddIn, ddOut};   // relative to ARM

class Acc_t {
private:
    // Pin operations
    void CsHi() { klPinSet(GPIOA, ACC_CS); }
    void CsLo() { klPinClear(GPIOA, ACC_CS); }
    void ClkHi() { klPinSet(GPIOA, ACC_CLK);  }
    void ClkLo() { klPinClear(GPIOA, ACC_CLK); }
    void DodiHi() { klPinSet(GPIOA, ACC_DODI);  }
    void DodiLo() { klPinClear(GPIOA, ACC_DODI); }
    bool DodiIsHi() { return klPinIsSet(GPIOA, ACC_DODI); }
    // Read/Write
    void ReadNBytes(uint8_t *PDst, uint8_t N);
    void WriteByte(uint8_t AByte);
    void WriteReg(uint8_t AAddr, uint8_t AValue);

public:
    int16_t Axis[ACC_FIFO_SZ][3];
    uint8_t Init();
    void Read();
    uint8_t ReadReg(uint8_t AAddr);
    bool IrqIsHi() { return klPinIsSet(GPIOA, ACC_IRQ); }
};

extern Acc_t Acc;

// Register map
#define STATUS_REG_AUX  0x07
#define OUT_ADC1_L      0x08
#define OUT_ADC1_H      0x09
#define OUT_ADC2_L      0x0A
#define OUT_ADC2_H      0x0B
#define OUT_ADC3_L      0x0C
#define OUT_ADC3_H      0x0D
#define INT_COUNTER_REG 0x0E
#define WHO_AM_I        0x0F

#define TEMP_CFG_REG    0x1F
#define CTRL_REG1       0x20
#define CTRL_REG2       0x21
#define CTRL_REG3       0x22
#define CTRL_REG4       0x23
#define CTRL_REG5       0x24
#define CTRL_REG6       0x25
#define REFERENCE       0x26
#define STATUS_REG2     0x27

#define OUT_X_L         0x28
#define OUT_X_H         0x29
#define OUT_Y_L         0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L         0x2C
#define OUT_Z_H         0x2D

#define FIFO_CTRL_REG   0x2E
#define FIFO_SRC_REG    0x2F
#define INT1_CFG        0x30
#define INT1_SOURCE     0x31
#define INT1_THS        0x32
#define INT1_DURATION   0x33

#define CLICK_CFG       0x38
#define CLICK_SRC       0x39
#define CLICK_THS       0x3A
#define TIME_LIMIT      0x3B
#define TIME_LATENCY    0x3C
#define TIME_WINDOW     0x3D


#endif /* LIS3D_H_ */
