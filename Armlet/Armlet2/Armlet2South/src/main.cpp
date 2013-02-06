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
#include "bridge.h"
#include "main.h"

static inline void Init();

int main(void) {
    // ==== Init clock system ====
    Clk.UpdateFreqValues();
    Clk.SetupFlashLatency();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();

    while(TRUE) {
        chThdSleepMilliseconds(999);
    }
}

void Init() {
    //Uart.Init(115200);
    //Uart.Printf("Southbridge2 AHB=%u; APB=%u\r", Clk.AHBFreqHz, Clk.APBFreqHz);
    Bridge.Init();

    // Send ATR
    SBCmd_t Cmd;
    Cmd.CmdType = STN_ATR;
    Cmd.DataSz = 4;
    Cmd.Ptr = (void*)&FwVersion;
    Bridge.AddCmd(&Cmd);
}
