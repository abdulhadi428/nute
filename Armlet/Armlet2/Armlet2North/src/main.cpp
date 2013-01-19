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
#include "application.h"

static inline void Init();

// Application entry point.
int main(void) {
    halInit();
    chSysInit();
    Init();

    //uint32_t i=0;
    while(TRUE) {
        chThdSleepMilliseconds(999);
        //Uart.Printf("%u %u %u\r", i, i, i);
        //i++;
    }
}

void Init() {
    // Setup System Core Clock. Fix it in future clock switching.
    SysCoreClock = 16000000;

    Uart.Init(115200);
    Uart.Printf("Armlet2\r");
    Lcd.Init();
    SouthBridge.Init();
    // Application init
    App.Init();
}
