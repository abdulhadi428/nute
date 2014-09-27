/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: ChibiArmlet-Atlantis
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_L15x.h"
#include "ch.h"
#include "hal.h"
#include "clocking_L1xx.h"
#include "cmd_uart.h"

#include "mpr121.h"
#include "main.h"
#include "evt_mask.h"

App_t App;

int main(void) {
    // ==== Init Vcore & clock system ====
    SetupVCore(vcore1V2);
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    Uart.Init(115200);
    Uart.Printf("\rstm32L template   AHB=%u", Clk.AHBFreqHz);

    App.PThd = chThdSelf();

    Touch.Init();

    //Filter.Init(FirCoeff);
//
//    for(int32_t x=0; x<20; x++) {
//        Filter.AddAndCalc(1);
//    }

    // ==== Main cycle ====
    while(true) {
        App.ITask();
//        chThdSleepMilliseconds(450);
    } // while true

}

void App_t::ITask() {
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
    if(EvtMsk & EVTMSK_ON_FINGER_TOUCH) {
        Uart.Printf("\rTouch");
        if(!LightIsOn) {
            Uart.Printf("\rLightOn");
            LightIsOn = true;
        }
    }
    else if(EvtMsk & EVTMSK_ON_FINGER_MOVE) {
        Lt += Touch.dx;
        if(Lt < 0) Lt = 0;
        if(Lt > 300) Lt = 300;
        Uart.Printf("\rLt=%d;", Lt);
    }
}
