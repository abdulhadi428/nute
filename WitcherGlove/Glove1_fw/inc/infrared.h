/*
 * infrared.h
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
 */

#ifndef INFRARED_H_
#define INFRARED_H_

#include "kl_lib_f2xx.h"

#define IR_CARRIER_HZ   57000

// Delays, uS
#define IR_SPACE_US     600
#define IR_HEADER_US    (IR_SPACE_US + 2400)
#define IR_ZERO_US      (IR_SPACE_US + 600)
#define IR_ONE_US       (IR_SPACE_US + 1200)

// Counts
#define IR_BIT_NUMBER   14
#define IR_DELAY_COUNT  (1 + IR_BIT_NUMBER) // first delay for header

class Infrared_t {
private:
    uint16_t BufDelays[IR_DELAY_COUNT];
    uint16_t MaxPower;
    PwmPin_t Carrier;
public:
    void Init();
    void TransmitWord(uint16_t wData, uint16_t Power);
};

extern Infrared_t IR;

#endif /* INFRARED_H_ */
