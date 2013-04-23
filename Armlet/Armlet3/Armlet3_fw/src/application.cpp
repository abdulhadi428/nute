/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "lcd2630.h"
#include "peripheral.h"
#include "BeepSequences.h"
#include "VibroSequences.h"
#include "keys.h"

#include "lvl1_assym.h"
#include "evt_mask.h"

#include "kl_sd.h"

App_t App;

static EventListener EvtLstnrAppRadioRx, EvtListenerAppKeys;
static rPktWithData_t<RRX_PKT_DATA_SZ> SRxPkt;

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

//#define PktSZ   4
//    uint8_t Buf[PktSZ], Rslt1 = FAILURE, Rslt2 = FAILURE;
//    for(uint8_t i=0; i<PktSZ; i++) Buf[i] = i;

    // Events
    uint32_t EvtMsk;
    KeysRegisterEvt(&EvtListenerAppKeys, EVTMASK_KEYS);
    rLevel1.RegisterEvtRx(&EvtLstnrAppRadioRx, EVTMASK_RADIO_RX);

    while(1) {
        //chThdSleepMilliseconds(999);
//        Rslt1 = rLevel1.AddPktToTx(0, Buf, PktSZ, &Rslt2);
//        Uart.Printf("### %u\r", Rslt1);

        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_KEYS);

        if(EvtMsk &EVTMASK_KEYS) {
            if((KeyStatus[0] == KEY_PRESSED) or (KeyStatus[1] == KEY_PRESSED) or (KeyStatus[2] == KEY_PRESSED)) {
                Beeper.Beep(BeepBeep);
            }
            else if((KeyStatus[3] == KEY_PRESSED) or (KeyStatus[4] == KEY_PRESSED) or (KeyStatus[5] == KEY_PRESSED)) {
                Vibro.Vibrate(BrrBrr);
            }
            else if((KeyStatus[6] == KEY_PRESSED) or (KeyStatus[7] == KEY_PRESSED) or (KeyStatus[8] == KEY_PRESSED)) {
                Beeper.Beep(ShortBeep);
                Vibro.Vibrate(ShortBrr);
            }
        }

        if(EvtMsk & EVTMASK_RADIO_RX) {
            while(rLevel1.GetReceivedPkt(&SRxPkt) == OK) {
                rLevel1.AddPktToTx(0, SRxPkt.Data, SRxPkt.Length);
            }
        } // if evtmsk

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
