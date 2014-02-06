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
#include "keys.h"

IWDG_t Iwdg;

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv2, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    JtagDisable();
    Uart.Init(57600);
    Uart.Printf("\rCalma04  AHB=%u; APB1=%u; APB2=%u; PwrCsr=%X\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz, PWR->CSR);

    bool LedIsOn = false;
    if(Sleep::WasInStandby()) {
        Sleep::DisableWakeupPin();  // Disable wakeup pin to allow key to act properly
        Led.Init(0, chThdSelf());
        Led.SetSmoothly(LED_TOP_VALUE);
        LedIsOn = true;
    }
    else {  // Normal switch on
        Led.Init(LED_TOP_VALUE, chThdSelf());
        Led.SetSmoothly(0);
    }
    Keys.Init(chThdSelf());

    // ==== Main cycle ====
    while(true) {
        eventmask_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
        if(EvtMsk & EVTMSK_KEY) {
            if(LedIsOn) Led.SetSmoothly(0);
            else Led.SetSmoothly(LED_TOP_VALUE);
            LedIsOn = !LedIsOn;
        }

        if(EvtMsk & EVTMSK_LED_OFF) {   // Sleep now
            Sleep::EnableWakeupPin();
            Sleep::EnterStandbyMode();
        }
    } // while
}

