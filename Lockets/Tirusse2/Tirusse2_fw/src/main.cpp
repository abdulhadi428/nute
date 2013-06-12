/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "led.h"
#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"

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
        chThdSleepMilliseconds(4500);
    } // while
}

void Init() {
    JtagDisable();
    Uart.Init(57600);
    LedSmooth.Init();
    Uart.Printf("\rTirusse2  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    LedSmooth.SetSmoothly(LED_BOTTOM_VALUE);
}
