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
#include "clocking_L1xx.h"
#include "peripheral.h"
#include "sequences.h"
#include "cmd_uart.h"
#include "application.h"
#include "lcd1200.h"
#include "keys.h"

extern IWDG_t Iwdg;

int main(void) {
    // Init Vcore & clock system
    SetupVCore(vcore1V2);
    Clk.UpdateFreqValues();
    // Init OS
    halInit();
    chSysInit();

    // Check if IWDG reset occured
    if(Iwdg.ResetOccured()) {

    }

    // Init Hard & Soft
    Uart.Init(115200);
    Uart.Printf("BrainEnchanter AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);

    Lcd.Init();
    Lcd.Backlight(50);

    // Start 32768 quartz
    if(Clk.EnableLSE() != OK) {
        Uart.Printf("Quartz failure\r");
        Lcd.Printf(0, 0, "Quartz Failure");
        while(true);
    }
    Uart.Printf("32768 started\r");

    Beeper.Init();
    Beeper.Beep(BeepBeep);
    App.Init();
    Keys.Init();

    while(1) { chThdSleep(TIME_INFINITE); } // while
}
