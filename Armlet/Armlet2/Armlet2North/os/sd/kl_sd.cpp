/*
 * kl_sd.cpp
 *
 *  Created on: 13.02.2013
 *      Author: kreyl
 */

#include "kl_sd.h"
#include "sdc.h"
#include "sdc_lld.h"
#include "kl_lib_f2xx.h"

sd_t SD;

void sd_t::Init() {
    IsReady = FALSE;

    // Bus pins
    PinSetupAlterFunc(GPIOC,  8, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC,  9, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC, 10, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC, 11, omPushPull, pudPullUp, AF12, ps25MHz);
    PinSetupAlterFunc(GPIOC, 12, omPushPull, pudNone,   AF12, ps25MHz);
    PinSetupAlterFunc(GPIOD,  2, omPushPull, pudPullUp, AF12, ps25MHz);
    // Power pin
    PinSetupOut(GPIOC, 4, omPushPull, pudNone);
    PinClear(GPIOC, 4); // Power on
    Delay_ms(450);

    FRESULT err;
    sdcInit();
    sdcStart(&SDCD1, NULL);
    if (sdcConnect(&SDCD1)) {
        Uart.Printf("SD connect error\r");
        return;
    }
    else {
        Uart.Printf("SD capacity: %u\r", SDCD1.capacity);
    }

    err = f_mount(0, &SDC_FS);
    if (err != FR_OK) {
        Uart.Printf("SD mount error\r");
        sdcDisconnect(&SDCD1);
        return;
    }
    //Uart.Printf("SD filesystem mounted\r");
    IsReady = TRUE;

//    FRESULT rslt;
//    FIL IFile;
//    // Open file
//    rslt = f_open(&IFile, "settings.ini", FA_READ+FA_OPEN_EXISTING);
//    Uart.Printf("OpenFile: %u\r", (uint8_t)rslt);
}


// ==== Hardware ====
#ifdef __cplusplus
extern "C" {
#endif

bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) {
    (void)sdcp;
    return TRUE;
}

bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) {
    (void)sdcp;
    return FALSE;
}

#ifdef __cplusplus
}
#endif
