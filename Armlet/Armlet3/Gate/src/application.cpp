/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"
#include "evt_mask.h"
#include "kl_sd.h"

#include "lvl1_assym.h"

#include "cmd_uart.h"

App_t App;

static EventListener EvtLstnrApp;

// Prototypes
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length);

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

#define PktSZ   4
//    uint8_t Buf[PktSZ], Rslt1 = FAILURE, Rslt2 = FAILURE;
//    for(uint8_t i=0; i<PktSZ; i++) Buf[i] = i+2;
//
    // Events
    rLevel1.RegisterEvtTx(&EvtLstnrApp, EVTMASK_RADIO_TX);

    while(1) {
        chThdSleepMilliseconds(999);
//        Rslt1 = rLevel1.AddPktToTx(RDEV_BOTTOM_ID+1, Buf, PktSZ, &Rslt2);
//        Uart.Printf("> %u\r", Rslt1);
//
//        chEvtWaitOne(EVTMASK_RADIO_TX);
//        Uart.Printf("Rslt = %u\r", Rslt2);
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

//=========================== Command processing ===============================
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    //Uart.Printf("Rx: %X\r", CmdCode);
    uint8_t Buf[4];
    switch(CmdCode) {
        case 0x01:
            Buf[0] = OK;
            Uart.Cmd(0x90, Buf, 1);
            break;

        default: break;
    }
}

// =============================== App class ===================================
void App_t::Init() {
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}
