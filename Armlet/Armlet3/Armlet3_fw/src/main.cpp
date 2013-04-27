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
#include "lcd2630.h"
#include "peripheral.h"
#include "kl_sd.h"
#include "sound.h"
#include "keys.h"
#include "infrared.h"
#include "lvl1_assym.h"
#include "cmd_uart.h"
#include "power.h"

#include "application.h"

//extern void armlet_main();

static inline void Init();

// Application entry point.
int main() {
    // ==== Setup clock ====
    uint8_t ClkResult = 1;
    Clk.SetupFlashLatency(16);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/4 = 12 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();
    Clk.LSIEnable();        // To allow RTC to run

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");

    chThdSleepSeconds(3);

    while(TRUE) {
        chThdSleepMilliseconds(999);
        ShutdownPeriphery();
        Power.EnterStandby();
    }

    //armlet_main();
}

void Init() {
    Uart.Init(115200);
    Uart.Printf("Armlet3 AHB=%u; APB1=%u; APB2=%u; UsbSdio=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz, Clk.UsbSdioFreqHz);
    //    Lcd.Init();
//    SD.Init();
    KeysInit();
    Beeper.Init();
    Vibro.Init();
    IR.TxInit();
    IR.RxInit();
    Power.Init();
    //Sound.Init();
    //Sound.Play("alive.wav");
    // Radio
    rLevel1.Init(RDEV_BOTTOM_ID+1); // FIXME: replace RBOTTOMID with value from SD
    App.Init();
}
