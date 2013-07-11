/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "ir.h"

// IR pkt: 0xxx xxxx  0000 01
//          ^^^^^^^^ ID
// Pkt is left-adjusted.

#define TRANSMIT_PERIOD_MS  999

ir_t ir;
static inline void Init();
static inline void GoSleep();
static uint8_t ID;
static IWDG_t Iwdg;
static EventListener EvtLstnrIrTxEnd;

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    ir.RegisterEvtTxEnd(&EvtLstnrIrTxEnd, EVENT_MASK(0));
    while(TRUE) {
        uint16_t w = ID;
        w <<= 8;
        w |= 0x04;
        //Uart.Printf("%X\r", w);
        ir.TransmitWord(w, dc100mA, 100);
        chEvtWaitAny(EVENT_MASK(0));
        // Transmission completed
        //Uart.Printf("TxEnd\r");
        //GoSleep();
        chThdSleepMilliseconds(999);
    }
}

static inline uint8_t GetID() {
    uint8_t id = 0;
    for(uint8_t i=3; i<=9; i++) {
        id <<= 1;
        PinSetupIn(GPIOB, i, pudPullUp);
        Delay_ms(1);
        if(!PinIsSet(GPIOB, i)) id |= 1;    // Fuse detected
        PinSetupAnalog(GPIOB, i);
    }
    return id;
}

void Init() {
    JtagDisable();
    Uart.Init(57600);
    ir.Init();
    ID = GetID();
    ir.Init();
    // Set white and print info only when switch on, not after watcdog reset.
    if(!Iwdg.ResetOccured()) {
        Uart.Printf("\rGirandole #%u  AHB=%u; APB1=%u; APB2=%u\r", ID, Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
        chThdSleepMilliseconds(999);    // Timeout to connect programmator
    }
    else Uart.Printf("W\r");
}

void GoSleep() {
    // Start LSI
    Clk.LsiEnable();
    // Start IWDG
    Iwdg.SetTimeout(TRANSMIT_PERIOD_MS);
    Iwdg.Enable();
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
}
