/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "led.h"
#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"

#include "buttons.h"
#include "load.h"
#include "encoder.h"
#include "lcd.h"

static inline void Init();
static inline void GoSleep();

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv8, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();

    Lcd.Printf(0,0, "%u", Load.TimeToWork);
    Load.PrintMode(Load.Mode);

    while(TRUE) {
        chThdSleepMilliseconds(999);
//        Uart.Printf("a");
    }
}

void Init() {
//    JtagDisable();
    Uart.Init(57600);
//    Led.Init();
    Button.Init();
    Load.Init();
    Encoder.Init();
    Lcd.Init();
    Uart.Printf("\rAnganare  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
}

void GoSleep() {
    // Start LSI
    Clk.LsiEnable();
    // Start IWDG
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
}
