/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "kl_sd.h"

#include "lvl1_assym.h"

App_t App;

// Event masks
#define EVT_RADIO_TX    EVENT_MASK(0)
#define EVT_RADIO_RX    EVENT_MASK(1)

static EventListener EvtLstnrApp;

// Prototypes

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    // Open file
//    FRESULT rslt;
//    rslt = f_open(&SD.File, "settings.ini", FA_READ+FA_OPEN_EXISTING);
//    Uart.Printf("OpenFile: %u\r", (uint8_t)rslt);
//    if(rslt == FR_OK) {
//        Uart.Printf("Size: %u\r", SD.File.fsize);
//        uint32_t N=0;
//        rslt = f_read(&SD.File, Str, 250, (UINT*)&N);
//        if(rslt == FR_OK) {
//            Uart.Printf("N: %u; Str: %s\r", N, Str);
//        }
//        else Uart.Printf("ReadFile: %u\r", (uint8_t)rslt);
//        f_close(&SD.File);
//    }
//    Uart.Printf("1\r");
//    uint32_t Count=0;
//    iniReadUint32("Sound", "Count", "settings.ini", &Count);
//    Uart.Printf("Cnt: %u\r", Count);

//    Color_t c = clBlack;

#define PktSZ   4
    uint8_t Buf[PktSZ], Rslt = FAILURE;
    for(uint8_t i=0; i<PktSZ; i++) Buf[i] = i+2;

    // Events
    rLevel1.RegisterEvtTx(&EvtLstnrApp, EVT_RADIO_TX);

    while(1) {
        chThdSleepMilliseconds(702);
//        Rslt = rLevel1.AddPktToTx(RDEV_BOTTOM_ID, Buf, PktSZ, &Rslt);
//        Uart.Printf("> %u\r", Rslt);
//
//        chEvtWaitOne(EVT_RADIO_TX);
//        Uart.Printf("Rslt = %u\r", Rslt);
        //Uart.Printf("Evt \r");

        //Lcd.Cls(c);
//        for(uint8_t y=0; y<128; y+=8) {
//            chThdSleepMilliseconds(999);
//            //Lcd.Printf(0, y, clBlue, c, "YA=%u", y);
//            //Beep(BeepBeep);
//
//        }
//        c = (c == clBlack)? clWhite : clBlack;

//        Lcd.Cls(c);
//        switch(c) {
//            case clRed: c = clGreen; break;
//            case clGreen: c = clBlue; break;
//            case clBlue: c = clRed; break;
//            default: c = clRed; break;
//        }
    }
    return 0;
}

// =============================== App class ===================================
void App_t::Init() {
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}
