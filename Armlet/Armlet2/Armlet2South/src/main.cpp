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
#include "southbridge.h"
#include "main.h"
#include "keys.h"
#include "beep.h"

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
    Keys.Init();
    Beep.Init();

    Bridge.Init();

    // Send ATR
    SBCmd_t Cmd;
    Cmd.CmdType = STN_ATR;
    Cmd.DataSz = 4;
    Cmd.Ptr = (void*)&FwVersion;
    Transmitter.AddCmd(&Cmd);
}
