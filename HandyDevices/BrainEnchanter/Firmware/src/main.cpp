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

int main(void) {
    // Init Vcore & clock system
    SetupVCore(vcore1V2);
    Clk.UpdateFreqValues();
    // Init OS
    halInit();
    chSysInit();

    // Init Hard & Soft
    Uart.Init(115200);
    Uart.Printf("BrainEnchanter AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);

    Lcd.Init();
    Lcd.Backlight(50);
    Beeper.Init();
    Beeper.Beep(BeepBeep);

    App.Init();
    Keys.Init();

#if USE_QUARTZ_32768
    systime_t t = chTimeNow();
    while(true) {
        chThdSleepMilliseconds(99);
        Clk.StartLSE();
        chThdSleepMilliseconds(1107);
        if(Clk.IsLseOn()) {
            Uart.Printf("32768 started; t=%u\r", chTimeNow()-t);
            //Lcd.Printf(0,0,"Q");
            //App.UseQuartz = true;
            chThdSleep(TIME_INFINITE);
        }
        else {
            Uart.Printf("32768 failure\r");
            Clk.DisableLSE();
        }
    } // while
#else
    while(true) { chThdSleep(TIME_INFINITE); }
#endif
}
