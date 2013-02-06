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
    // ==== Setup clock ====
    uint8_t ClkResult = 1;
//    Clk.SetupFlashLatency(16);  // Setup Flash Latency for clock in MHz
//    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
//    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
//    // 48/4 = 12 MHz core clock. APB1 & APB2 clock derive on AHB clock
//    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
//    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
//    Clk.UpdateFreqValues();

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
    //Uart.Init(115200);
    //Uart.Printf("Southbridge2 AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);

}
