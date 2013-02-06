/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "ch.h"
#include "hal.h"

#include "kl_lib_f0.h"


static inline void Init();

int main(void) {
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    //if(ClkResult) Uart.Printf("Clock failure\r");

    while(TRUE) {
        chThdSleepMilliseconds(999);
        //Uart.Printf("Cl\r");
    }
}

void Init() {
    Uart.Init(115200);
    Uart.Printf("Southbridge2 AHB=%u; APB=%u\r", Clk.AHBFreqHz, Clk.APBFreqHz);

}
