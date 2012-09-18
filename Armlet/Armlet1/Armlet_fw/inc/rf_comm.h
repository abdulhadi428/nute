/*
 * rf_comm.h
 *
 *  Created on: 16.09.2012
 *      Author: kreyl
 */

#ifndef RF_COMM_H_
#define RF_COMM_H_

#include "cc1101.h"

// Neighbours counter
#define MAX_NB_COUNT    11
#define DEMONSTRATE_PERIOD  4005    // Demonstrate count every period, ms

// Sync module
#define ALWAYS_RX               // DEBUG
#define CYCLE_MIN_DURATION  99  // ms
#define CYCLE_MAX_ADDITION  27  // ms
#define CYCLE_COUNT         4   // Rx every 0 cycle

class Radio_t : public CC_t {
private:
    // Neighbours counter
    uint32_t NbrCount;
    uint32_t IdTable[MAX_NB_COUNT][3];
    void NbrDemonstrate();
    void NbrAdd(uint32_t *ID);
    // Sync module
    uint16_t CycleCounter;
    // Events
    void TxEndHandler(void);
    void NewPktHandler(void);
public:
    // Irq handlers; needed here to be called from C irq handlers
    void IRQ0Handler(void);
    void IRQ2Handler(void);
    // General
    void Init();
    void Task();
};

extern Radio_t Radio;

extern "C" {
void EXTI3_IRQHandler(void);    // GDO0
void EXTI4_IRQHandler(void);    // GDO2
}

#endif /* RF_COMM_H_ */
