/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_L15x.h"
#include "clocking_L1xx.h"
#include "ch.h"
#include "hal.h"

int main(void) {
    // ==== Init Vcore & clock system ====
    SetupVCore(vcore1V8);
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    PinSetupOut(GPIOC, 9, omPushPull);
    PinSet(GPIOC, 9);

    while(1) {
//        PinSet(GPIOC, 8);
//        chThdSleepMilliseconds(207);
//        PinClear(GPIOC, 8);
//        chThdSleepMilliseconds(360);
    } // while
}
