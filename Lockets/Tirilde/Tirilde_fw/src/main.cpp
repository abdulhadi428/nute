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

#define ALWAYS_RX

LedRGB_t Led;
klPin_t dp, gp;

// Sync module
#define CYCLE_MIN_DURATION  198 // ms
#define CYCLE_MAX_ADDITION  36  // ms
#define CYCLE_COUNT         4   // Rx every 0 cycle
class Sync_t {
private:
public:
    uint16_t CycleCounter;
    void Init(void);
    void Task(void);
} Sync;

// Prototypes
void GeneralInit(void);

// ============================ Implementation =================================
int main(void) {
    GeneralInit();
    //uint32_t Tmr;
    // ==== Main cycle ====
    while (1) {
    	Uart.Task();
        //Led.Task();
        CC.Task();
        Sync.Task();

        //dp = (CC.Aim != caRx);

//        if(Delay.Elapsed(&Tmr, 450)) {
//            dp = !dp;
////        	if(Cl == clBlack) Cl = clBlue;
////        	else Cl = clBlack;
////        	Led.SetColorSmoothly(Cl);
//        	//CC.Transmit();
//        }
    } // while(1)
}

inline void GeneralInit(void) {
    InitClock(clk1MHzInternal);
    klJtagDisable();

    Delay.Init();
    // DEBUG
    //Led.Init();
    dp.Init(GPIOA, 11, GPIO_Mode_Out_PP);
    dp=1;
    gp.Init(GPIOA, 8, GPIO_Mode_Out_PP);
    gp=1;

    Uart.Init(57600);
    Uart.Printf("\rTirilde\r");

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(0);   // dummy
    CC.SetPower(plN6dBm);

    Sync.Init();
    // Get unique ID
    GetUniqueID(&PktTx.IdArr[0]);
    Uart.Printf("ID: %X8 %X8 %X8\r", PktTx.IdArr[0], PktTx.IdArr[1], PktTx.IdArr[2]);
}

// ==================================== Sync ===================================
void Sync_t::Init() {
    CycleCounter = 0;
}

/*
 * RX at 0 cycle for full cycle length FDuration = CYCLE_MIN_DURATION + [0; CYCLE_MAX_ADDITION];
 * RX at 1 cycle for FRxDuration = (CYCLE_MIN_DURATION + CYCLE_MAX_ADDITION) - FDuration.
 */
void Sync_t::Task() {
    static uint32_t FTmr, FDuration=CYCLE_MIN_DURATION;
    static uint32_t FRxTmr, FRxDuration;
    if(Delay.Elapsed(&FTmr, FDuration)) {
        dp=0;
        if (++CycleCounter >= CYCLE_COUNT) CycleCounter = 0;
        // Transmit at every cycle start
        CC.Transmit();
        dp=1;
        // Generate random duration: CYCLE_MIN_DURATION + [0; CYCLE_MAX_ADDITION]
        FDuration = CYCLE_MIN_DURATION + rand() % CYCLE_MAX_ADDITION;
        // Calculate Rx duration of first cycle
        if(Sync.CycleCounter == 0) {
            FRxDuration = (CYCLE_MIN_DURATION + CYCLE_MAX_ADDITION) - FDuration;
        }
        else if(Sync.CycleCounter == 1) {
            Delay.Reset(&FRxTmr);   // Prepare to end RX
        }
    }

    // Stop RX if needed
#ifndef ALWAYS_RX
    if((Sync.CycleCounter == 1) and (CC.Aim == caRx)) {
        if(Delay.Elapsed(&FRxTmr, FRxDuration)) {
            dp=0;
            CC.EnterIdle();
            dp=1;
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
    Uart.Printf("NbID: %X8 %X8 %X8\r", PktRx.IdArr[0], PktRx.IdArr[1], PktRx.IdArr[2]);
}


