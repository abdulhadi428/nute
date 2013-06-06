/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "led_smooth.h"
#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"

static inline void Init();

//static WORKING_AREA(waKeyThread, 128);
//static msg_t KeyThread(void *arg);

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv8, apbDiv1);
    Clk.UpdateFreqValues();
    Clk.SetupFlashLatency();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();

    while(TRUE) {
//        Led.SetSmoothly(LED_TOP_VALUE);
//        chThdSleepMilliseconds(4005);
//        Led.SetSmoothly(0);
        chThdSleepMilliseconds(4005);
    }
}

void Init() {
    Uart.Init(57600);
    Uart.Printf("\rFirefly3  AHB=%u; APB=%u\r", Clk.AHBFreqHz, Clk.APBFreqHz);
//    Led.Init();
//    Led.SetSmoothly(LED_TOP_VALUE);
//    chThdCreateStatic(waKeyThread, sizeof(waKeyThread), NORMALPRIO, KeyThread, NULL);
}

// ==== LED Thread ====
//static msg_t KeyThread(void *arg) {
//    (void)arg;
//    chRegSetThreadName("Key");
//    // Init key pin
//    PinSetupIn(GPIOB, 1, pudNone);
//
//    bool WasPressed = false;
//    bool Trigger = false;
//
//    while(1) {
//        chThdSleepMilliseconds(207);
//        bool IsPressed = PinIsSet(GPIOB, 1);
//        if(IsPressed and !WasPressed) {
//            WasPressed = true;
//            // Switch light
//            if(Trigger) Led.SetSmoothly(LED_TOP_VALUE);
//            else Led.SetSmoothly(0);
//            Trigger = !Trigger;
//        }
//        else if(!IsPressed and WasPressed) {
//            WasPressed = false;
//        }
//
//    }
//    return 0;
//}
