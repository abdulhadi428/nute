/*
 * File:   main.cpp of Tirilde
 * Author: Kreyl
 *
 */

#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include <stdlib.h>
#include "kl_lib.h"
#include "led.h"
#include "cc1101.h"

LedRGB_t Led;

// Neighbours counter
#define MAX_NB_COUNT    11
#define DEMONSTRATE_PERIOD  4005    // Demonstrate count every period, ms
class NCounter_t {
private:
    uint32_t Count;
    uint32_t IdTable[MAX_NB_COUNT][3];
public:
    void Demonstrate(void);
    void Reset(void) { Count = 0; }
    void Add(uint32_t *ID);
    void Task(void);
} NCounter;

// Sync module
//#define ALWAYS_RX               // DEBUG
#define CYCLE_MIN_DURATION  99 // ms
#define CYCLE_MAX_ADDITION  27  // ms
#define CYCLE_COUNT         4   // Rx every 0 cycle
class Sync_t {
private:
public:
    uint16_t CycleCounter;
    void Init(void) { CycleCounter = 0; }
    void Task(void);
} Sync;

// Prototypes
void GeneralInit(void);

// ============================ Implementation =================================
int main(void) {
    GeneralInit();
    // ==== Main cycle ====
    while (1) {
    	Uart.Task();
        Led.Task();
        CC.Task();
        Sync.Task();
        NCounter.Task();
    } // while(1)
}

inline void GeneralInit(void) {
    InitClock(clk1MHzInternal);
    klJtagDisable();

    Delay.Init();
    Led.Init();
//    Led.Blink(99, {1,1,0});

    Uart.Init(57600);
    Uart.Printf("\rTirilde\r");

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetPower(plP10dBm);

    Sync.Init();
    // Get unique ID
    GetUniqueID(&PktTx.IdArr[0]);
    Uart.Printf("ID: %X8 %X8 %X8\r", PktTx.IdArr[0], PktTx.IdArr[1], PktTx.IdArr[2]);
    NCounter.Reset();
}

// ================================== NCounter =================================
void NCounter_t::Add(uint32_t *ID) {
    // Check if already in table
    for(uint32_t i=0; i<Count; i++) {
        if((ID[0] == IdTable[i][0]) and (ID[1] == IdTable[i][1]) and (ID[2] == IdTable[i][2]))
            return; // Already in, nothing to do
    }
    // Add if table is not full
    if(Count < MAX_NB_COUNT) {
        IdTable[Count][0] = ID[0];
        IdTable[Count][1] = ID[1];
        IdTable[Count][2] = ID[2];
        Count++;
    }
}

void NCounter_t::Demonstrate() {
    //Uart.Printf("Nb count: %u\r", Count);
    if(Count != 0) Led.SetColorSmoothly((Color_t){36, 99, 99});
    else Led.SetColorSmoothly(clBlack);
}

void NCounter_t::Task() {
    static uint32_t FTmr;
    if(Delay.Elapsed(&FTmr, DEMONSTRATE_PERIOD)) {
        NCounter.Demonstrate();
        NCounter.Reset();
    }
}

// ==================================== Sync ===================================
/*
 * RX at 0 cycle for full cycle length FDuration = CYCLE_MIN_DURATION + [0; CYCLE_MAX_ADDITION];
 * RX at 1 cycle for FRxDuration = (CYCLE_MIN_DURATION + CYCLE_MAX_ADDITION) - FDuration.
 */
void Sync_t::Task() {
    static uint32_t FTmr, FDuration=CYCLE_MIN_DURATION;
#ifndef ALWAYS_RX
    static uint32_t FRxTmr, FRxDuration;
#endif
    if(Delay.Elapsed(&FTmr, FDuration)) {
        if (++CycleCounter >= CYCLE_COUNT) CycleCounter = 0;
        // Transmit at every cycle start
        CC.Transmit();
        // Generate random duration: CYCLE_MIN_DURATION + [0; CYCLE_MAX_ADDITION]
        FDuration = CYCLE_MIN_DURATION + rand() % CYCLE_MAX_ADDITION;
        // Calculate Rx duration of first cycle
#ifndef ALWAYS_RX
        if(Sync.CycleCounter == 0) {
            FRxDuration = (CYCLE_MIN_DURATION + CYCLE_MAX_ADDITION) - FDuration;
        }
        else if(Sync.CycleCounter == 1) {
            Delay.Reset(&FRxTmr);   // Prepare to end RX
        }
#endif
    }

    // Stop RX if needed
#ifndef ALWAYS_RX
    if((Sync.CycleCounter == 1) and (CC.Aim == caRx)) {
        if(Delay.Elapsed(&FRxTmr, FRxDuration)) {
            CC.EnterIdle();
        }
    }
#endif
}

void CC_t::TxEndHandler() {
#ifdef ALWAYS_RX
    Receive();
#else
    // Enter RX at zero cycle and at start of first, otherwise sleep
    if((Sync.CycleCounter == 0) or (Sync.CycleCounter == 1)) Receive();
    else EnterIdle();
#endif
}

void CC_t::NewPktHandler() {
    //Uart.Printf("NbID: %X8 %X8 %X8\r", PktRx.IdArr[0], PktRx.IdArr[1], PktRx.IdArr[2]);
    //Uart.Printf("dBm: %i\r", CC.RSSI_dBm(PktRx.RSSI));
    NCounter.Add(PktRx.IdArr);
    //Led.Blink(36, clYellow);  // DEBUG
}


