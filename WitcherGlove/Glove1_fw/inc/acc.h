/*
 * acc.h
 *
 *  Created on: 02.03.2013
 *      Author: kreyl
 */

#ifndef ACC_H_
#define ACC_H_

#include "kl_lib_f2xx.h"
#include "hal.h"
#include "ch.h"

#define ACC_ADDR    0x1C

class i2c_t {
private:
    GPIO_TypeDef *GPIO;
    uint16_t Scl, Sda;
    Thread *PThread;
    // Pin operations
    void SclHi()    { PinSet  (GPIO, Scl); }
    void SclLo()    { PinClear(GPIO, Scl); }
    void SdaHi()    { PinSet  (GPIO, Sda); }
    void SdaLo()    { PinClear(GPIO, Sda); }
    bool SclIsHi()  { return PinIsSet(GPIO, Scl); }
    bool SdaIsHi()  { return PinIsSet(GPIO, Sda); }
    // Delay
    void ILoop(volatile uint32_t lp) { while(lp--); }
    void IDelay() { ILoop(9); }
    // i2c operations
    uint8_t Start();
    void Stop();
    void WriteByte(uint8_t b);
    bool IsAcked();
public:
    void Init(GPIO_TypeDef *PGPIO, uint16_t AScl, uint16_t ASda);
    uint8_t WriteBuf(uint8_t Addr, uint8_t *P, uint16_t Sz);

};

class SingleAcc_t {
private:
    i2c_t i2c;
    uint16_t Irq;
public:
    void Init(GPIO_TypeDef *PGPIO, uint16_t AScl, uint16_t ASda, uint16_t AIrq);

};

#define ACC_CNT     1

// Registers addresses
#define ACC_REG_STATUS          0x00
#define ACC_REG_XYZ_DATA_CFG    0x0E
#define ACC_FF_MT_CFG           0x15
#define ACC_FF_MT_SRC           0x16
#define ACC_FF_MT_THS           0x17
#define ACC_REG_CONTROL1        0x2A

class Acc_t {
private:
    SingleAcc_t IAcc[ACC_CNT];
public:
    void Init();
};

extern Acc_t Acc;

#endif /* ACC_H_ */
