/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f0.h"

int main(void) {
    // ==== Init clock system ====
    Clk.UpdateFreqValues();
    Clk.SetupFlashLatency();
    // ==== Init OS ====
    halInit();
    chSysInit();

    PinSetupOut(GPIOC, 9, omPushPull, pudNone, ps10MHz);

    while(TRUE) {
        PinSet(GPIOC, 9);
        chThdSleepMilliseconds(207);
        PinClear(GPIOC, 9);
        chThdSleepMilliseconds(207);
    }
}
