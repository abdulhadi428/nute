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
#include "flash.h"
#include "buzzer.h"

Flash_t Flash;

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
    Flash.Init();
    Buzzer.Init();

    Uart.Printf("\rAvada2  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);

    // ==== Main cycle ====
    while(true) {

        Buzzer.BuzzUp();

        chThdSleepMilliseconds(180000);
//        Buzzer.Off();
//        chThdSleepMilliseconds(1800);
        //Flash.Fire();
//            if(rLevel1.SomethingIsNear) LedSmooth.SetSmoothly(LED_TOP_VALUE);
//            else LedSmooth.SetSmoothly(0);
    } // while
}

