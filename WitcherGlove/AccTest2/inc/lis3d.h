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

#define ACC_CLK     GPIO_Pin_0
#define ACC_MOSI    GPIO_Pin_1
#define ACC_MISO    GPIO_Pin_2
#define ACC_CS      GPIO_Pin_3
#define ACC_IRQ     GPIO_Pin_4

#define ACC_FIFO_SZ 32

struct Acceleration_t {
    int16_t x, y, z;
} PACKED;

class Acc_t {
private:
    void CsHi() { klGpioSetByMsk(GPIOA, ACC_CS); }
    void CsLo() { klGpioClearByMsk(GPIOA, ACC_CS); }
    void ClkHi() { klGpioSetByMsk(GPIOA, ACC_CLK);  }
    void ClkLo() { klGpioClearByMsk(GPIOA, ACC_CLK); }
    void MosiHi() { klGpioSetByMsk(GPIOA, ACC_MOSI);  }
    void MosiLo() { klGpioClearByMsk(GPIOA, ACC_MOSI); }
    bool MisoIsHi() { return klGpioIsSetByMsk(GPIOA, ACC_MISO); }
    void ReadNBytes(uint8_t *PDst, uint8_t N);
    uint16_t ReadWord();
    void WriteByte(uint8_t AByte);
    void WriteReg(uint8_t AAddr, uint8_t AValue);

public:
    Acceleration_t Buf[ACC_FIFO_SZ];
    void Init();
    void Read();
    uint8_t ReadReg(uint8_t AAddr);
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
