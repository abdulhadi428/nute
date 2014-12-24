/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f0.h"
#include "ch.h"
#include "hal.h"
#include "led_smooth.h"

#define BTN_GPIO        GPIOA
#define BTN_PIN         0
#define BtnIsPressed()  (PinIsSet(GPIOA, BTN_PIN))

#define SMOOTH_CONST    450

void GoSleep();

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv4, apbDiv1); // Sysclk = 2MHz
    Clk.UpdateFreqValues();
    Clk.SetupFlashLatency();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    Uart.Init(115200);
    Uart.Printf("\rTittaCalma  AHB=%u APB=%u", Clk.AHBFreqHz, Clk.APBFreqHz);
    // Led
    Led.Init();
    Led.FadeIn(450);
    // Button
    PinSetupIn(GPIOA, BTN_PIN, pudPullDown);

    chThdSleepMilliseconds(720);

    // ==== Main cycle ====
    bool BtnWasPressed = false, LedIsOn = true;
    while(TRUE) {
        chThdSleepMilliseconds(45);
        // Check button
        if(BtnIsPressed() and !BtnWasPressed) {
            BtnWasPressed = true;
            if(LedIsOn) Led.FadeOut(SMOOTH_CONST);
            else Led.FadeIn(SMOOTH_CONST);
            LedIsOn = !LedIsOn;
        }
        else if(!BtnIsPressed() and BtnWasPressed) {
            BtnWasPressed = false;
        }
        // Check if time to sleep
        if(Led.CurrentBrt == 0 and Led.DesiredBrt == 0 and !BtnIsPressed()) GoSleep();
    } // while true
}

void GoSleep() {
    chSysLock();
    // Enable WKUP1 pin
    PWR->CSR |= PWR_CSR_EWUP1;
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
    chSysUnlock();
}
