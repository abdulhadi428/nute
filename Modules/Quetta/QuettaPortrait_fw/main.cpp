/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "ch.h"
#include "hal.h"

#include "kl_lib_f2xx.h"
#include "kl_sd.h"
#include "sound.h"
#include "cmd_uart.h"
#include "ff.h"
#include "MassStorage.h"
#include "evt_mask.h"

int main() {
    // ==== Setup clock ====
    Clk.UpdateFreqValues();
    uint8_t ClkResult = FAILURE;
    Clk.SetupFlashLatency(12);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/4 = 12 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    Uart.Init(115200);
    SD.Init();
    // Read config
//    SD.iniReadInt32("Radio", "id", "settings.ini", &App.SelfID);
//    Uart.Printf("\rID=%u", App.SelfID);

//    MassStorage.Init();

    Sound.Init();
    Sound.SetVolume(255);
    Sound.RegisterAppThd(chThdSelf());
    Sound.Play("alive.wav");
//    Sound.Play("music.mp3");

    Uart.Printf("\rPortrait   AHB freq=%uMHz", Clk.AHBFreqHz/1000000);
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");

    while(true) {
        uint32_t EvtMsk;
        EvtMsk = chEvtWaitAny(ALL_EVENTS);
        if(EvtMsk == EVTMASK_PLAY_ENDS) {
            Uart.Printf("\rPlayEnd");
        }
    }
}

