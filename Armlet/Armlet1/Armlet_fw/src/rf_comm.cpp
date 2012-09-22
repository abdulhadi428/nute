/*
 * rf_comm.cpp
 *
 *  Created on: 16.09.2012
 *      Author: kreyl
 */

#include "rf_comm.h"
#include "lcd1200.h"
#include <stdlib.h>

Radio_t Radio;

void Radio_t::Init() {
    // Setup CC
    CC_t::Init();
    SetChannel(0);
    SetPower(pl0dBm);
    // Variables
    CycleCounter = 0;
    NbrCount = 0;
}

void Radio_t::Task() {
    CC_t::Task();
    // Neighbours
    static uint32_t NbrTmr;
    if(Delay.Elapsed(&NbrTmr, DEMONSTRATE_PERIOD)) {
        NbrDemonstrate();
        NbrCount = 0;   // Reset neighbours counter
    }
    /* ==== Sync task ====
     * RX at 0 cycle for full cycle length FDuration = CYCLE_MIN_DURATION + [0; CYCLE_MAX_ADDITION];
     * RX at 1 cycle for FRxDuration = (CYCLE_MIN_DURATION + CYCLE_MAX_ADDITION) - FDuration.
     */
    static uint32_t FTmr, FDuration=CYCLE_MIN_DURATION;
#ifndef ALWAYS_RX
    static uint32_t FRxTmr, FRxDuration;
#endif
    if(Delay.Elapsed(&FTmr, FDuration)) {
        if (++CycleCounter >= CYCLE_COUNT) CycleCounter = 0;
        Transmit();     // Transmit at every cycle start
        // Generate random duration: CYCLE_MIN_DURATION + [0; CYCLE_MAX_ADDITION]
        FDuration = CYCLE_MIN_DURATION + rand() % CYCLE_MAX_ADDITION;
#ifndef ALWAYS_RX
        // Calculate Rx duration of first cycle
        if(CycleCounter == 0) {
            FRxDuration = (CYCLE_MIN_DURATION + CYCLE_MAX_ADDITION) - FDuration;
        }
        else if(CycleCounter == 1) {
            Delay.Reset(&FRxTmr);   // Prepare to end RX
            // Stop RX if needed
            if(Aim == caRx) {
                if(Delay.Elapsed(&FRxTmr, FRxDuration)) EnterIdle();
            }
        } // if ==1
#endif
    } // if Delay.Elapsed(&FTmr, FDuration)
}

// =============================== Neighbours ==================================
void Radio_t::NbrAdd(uint32_t *ID) {
    // Check if already in table
    for(uint32_t i=0; i<NbrCount; i++) {
        if((ID[0] == IdTable[i][0]) and (ID[1] == IdTable[i][1]) and (ID[2] == IdTable[i][2]))
            return; // Already in, nothing to do here
    } // for
    // Add if table is not full
    if(NbrCount < MAX_NB_COUNT) {
        IdTable[NbrCount][0] = ID[0];
        IdTable[NbrCount][1] = ID[1];
        IdTable[NbrCount][2] = ID[2];
        NbrCount++;
    }
}

void Radio_t::NbrDemonstrate() {
    //Uart.Printf("Nb count: %u\r", NbrCount);
    //Lcd.Printf(0, 7, "Nb: %u  ", NbrCount);
//    if(NbrCount != 0) Led.SetColorSmoothly(clRed);
//    else Led.SetColorSmoothly(clBlack);
}

// ================================= Events ====================================
void Radio_t::TxEndHandler() {
    //Uart.Printf("tx\r");
#ifdef ALWAYS_RX
    Receive();
#else
    // Enter RX at zero cycle and at start of first, otherwise sleep
    if(CycleCounter <= 1) Receive();
    else EnterIdle();
#endif
}

void Radio_t::NewPktHandler() {
    int32_t rssi = RSSI_dBm(PktRx.RSSI);
    Uart.Printf("rx %d\r", rssi);
//
//    Lcd.Printf(0, 7, "rx %i   ", rssi);
    //Uart.Printf("NbID: %08X %08X %08X\r", PktRx.IdArr[0], PktRx.IdArr[1], PktRx.IdArr[2]);
    //Uart.Printf("dBm: %d\r", CC.RSSI_dBm(PktRx.RSSI));
    NbrAdd(PktRx.IdArr);
    //Led.Blink(36, clYellow);  // DEBUG
}



// ============================= Interrupts ====================================
void Radio_t::IRQ0Handler() {
    if (Aim == caTx) {
        Aim = caIdle;
        TxEndHandler();
    }
    else { // Was receiving
        if (ReadRX((uint8_t*)&PktRx)) NewPktHandler();
        FlushRxFIFO();
        // Do not reenter RX here to allow safely enter TX right after RX
    }
}

void Radio_t::IRQ2Handler(void) {

}


void EXTI3_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line3);
        Radio.IRQ2Handler();
    }
}
void EXTI4_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line4);
        Radio.IRQ0Handler();
    }
}

