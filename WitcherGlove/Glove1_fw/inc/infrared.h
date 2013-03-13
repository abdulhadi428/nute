/*
 * infrared.h
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
 */

#ifndef INFRARED_H_
#define INFRARED_H_

#include "kl_lib_f2xx.h"

#define IR_CARRIER_HZ   56000
#define IR_BIT_NUMBER   14

#define IR_TX_DMA_STR   STM32_DMA1_STREAM2
#define IR_TX_DMA_CHNL  6

// Delays, uS
#define IR_TICK_US      600 // Protocol smallest time unit, us
/* Header = 4 * IR_TICK_US
 * Space  = 1 * IR_TICK_US
 * Zero   = 1 * IR_TICK_US
 * One    = 2 * IR_TICK_US
 */

class Infrared_t {
private:
    uint16_t Buf[4+1 + IR_BIT_NUMBER*(2+1) + 1];    // Buffer of power values: header + all one's + 1 delay after
    uint32_t MaxPower;
    PwmPin_t Carrier;
public:
    bool IsBusy;
    void Init();
    void TransmitWord(uint16_t wData, uint8_t PwrPercent);
};

extern Infrared_t IR;

#endif /* INFRARED_H_ */
