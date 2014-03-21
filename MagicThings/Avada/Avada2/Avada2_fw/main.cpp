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
#include "led.h"
#include "buzzer.h"
#include "keys.h"

Led_t Led;

class Flash_t {
public:
    void Fire() {
        Led.Fire();
        Buzzer.Off();
    }
    void Restart() { Buzzer.BuzzUp(); }
    bool IsReady() { return Buzzer.IsOnTop(); }
} Flash;

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
    Led.Init();
    Buzzer.Init();
    Keys.Init(chThdSelf());

    Uart.Printf("\rAvada2  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    Flash.Restart();

    // ==== Main cycle ====
    while(true) {
        eventmask_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
        // Keys
        if(EvtMsk & EVTMSK_KEY_FIRE) {
            Uart.Printf("KeyFire\r");
            if(Flash.IsReady()) {
                Buzzer.SetVolume(VOLUME_MAX);
                chThdSleepMilliseconds(4005);
                Buzzer.Off();
                chThdSleepMilliseconds(504);
                Flash.Fire();
                Flash.Restart();
            }
        }
    } // while
}


