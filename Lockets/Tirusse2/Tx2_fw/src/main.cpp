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
//#include "pwr.h"
#include "lvl1_lckt.h"

//static Pwr_t Pwr;

// Event masks

static inline void Init();

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv8, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
//    PinSetupOut(GPIOA, 1, omPushPull);

    while(TRUE) {
        chThdSleepMilliseconds(999);
    } // while
}

static inline uint8_t GetID() {
    uint8_t id = 0;
    for(uint8_t i=3; i<9; i++) {
        id >>= 1;
        PinSetupIn(GPIOB, i, pudPullUp);
        Delay_ms(1);
        if(!PinIsSet(GPIOB, i)) id |= 0x20;    // Fuse detected
        PinSetupAnalog(GPIOB, i);
    }
    return id;
}

void Init() {
    JtagDisable();
    Uart.Init(57600);
//    Pwr.Init();
    // Get ID
    uint8_t id = GetID();
    rLevel1.Init(id);
    Uart.Printf("\rTx2 id=%u  AHB=%u; APB1=%u; APB2=%u\r", id, Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
}
