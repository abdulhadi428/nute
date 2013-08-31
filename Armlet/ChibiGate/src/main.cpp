/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_L15x.h"
#include "ch.h"
#include "hal.h"
#include "cmd_uart.h"
#include "pill.h"
#include "application.h"

static inline void Init();

int main(void) {
    // ==== Init Vcore & clock system ====
    SetupVCore(vcore1V8);
    uint8_t ClkResult = 1;
    Clk.SetupFlashLatency(16);  // Setup Flash Latency for clock in MHz
    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToHSI()) == 0) {
        Clk.HSEDisable();
        Clk.MSIDisable();
    }
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    if(ClkResult) Uart.Printf("Clock failure\r");
//    PinSetupOut(GPIOA, 1, omPushPull);
    while(1) {
        chThdSleepMilliseconds(999);
//        Uart.Printf("abc");
    } // while
}

void Init() {
    Uart.Init(115200);
    Uart.Printf("\rChibiGate  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    PillInit();
    Pin.Init();
}
