/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f0.h"
#include "ch.h"
#include "hal.h"
#include "led_smooth.h"

int main(void) {
    // ==== Init clock system ====
    Clk.UpdateFreqValues();
    Clk.SetupFlashLatency();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    Uart.Init(115200);
    Uart.Printf("\rTittaCalma  AHB=%u APB=%u", Clk.AHBFreqHz, Clk.APBFreqHz);
    Led.Init();

    // ==== Main cycle ====
    while(TRUE) {
        Led.FadeIn(360);
        chThdSleepMilliseconds(4500);
        Led.FadeOut(360);
        chThdSleepMilliseconds(4500);
    }
}

