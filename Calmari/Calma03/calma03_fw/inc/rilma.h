/*
 * rilma.h
 *
 *  Created on: 22.01.2012
 *      Author: kreyl
 */

#ifndef RILMA_H_
#define RILMA_H_

#include "delay_util.h"

//#define CHANNEL1_ENABLE
#define CHANNEL2_ENABLE


typedef enum {rSmoothOn, rSmoothOff} RilmaState_t;

class Rilma_t {
private:
    RilmaState_t IState;
    uint32_t ITimer;
    void Set(uint32_t AChannel, uint8_t AValue);
public:
    void Init(void);
    void Task(void);
    void SmoothOn(void);
    void SmoothOff(void) { IState = rSmoothOff; }
};


extern Rilma_t Rilma;

#endif /* RILMA_H_ */
