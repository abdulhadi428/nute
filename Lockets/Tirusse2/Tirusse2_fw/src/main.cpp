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
#include "pwr.h"
#include "lvl1_lckt.h"

#define LED_LOW     63
#define LED_HIGH    999
static Pwr_t Pwr;

// Event masks

static inline void Init();

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv8, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
//    PinSetupOut(GPIOA, 1, omPushPull);

    bool IsConnected = false;
    while(TRUE) {
        chThdSleepMilliseconds(999);
        // Check if connected
        if(Pwr.ExtPwrOn() and !IsConnected) {
            IsConnected = true;
            // Radio disable
        }
        else if(IsConnected and !Pwr.ExtPwrOn()) {
            IsConnected = false;
            // Radio enable
        }

        // Demonstrate charge status
        if(IsConnected) {
            if(Pwr.Charging()) {
                // Glimmer LED
                if(LedSmooth.IsEqOrAbove(LED_HIGH)) LedSmooth.SetSmoothly(LED_LOW);
                else if(LedSmooth.IsEqOrBelow(LED_LOW))LedSmooth.SetSmoothly(LED_HIGH);
            }
            else LedSmooth.SetSmoothly(LED_LOW);
        }
        else LedSmooth.SetSmoothly(0);

    } // while
}

void Init() {
    JtagDisable();
    Uart.Init(57600);
    LedSmooth.Init();
    LedSmooth.SetSmoothly(0);
    Pwr.Init();
    Uart.Printf("\rTirusse2  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
}
