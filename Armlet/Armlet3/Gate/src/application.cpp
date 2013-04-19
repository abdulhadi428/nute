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
#include "pill.h"

App_t App;

static EventListener EvtLstnrApp;

// Prototypes
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length);

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    uint8_t Buf[7];

#define PktSZ   4
//    uint8_t Buf[PktSZ], Rslt1 = FAILURE, Rslt2 = FAILURE;
//    for(uint8_t i=0; i<PktSZ; i++) Buf[i] = i+2;
//
    // Events
    //rLevel1.RegisterEvtTx(&EvtLstnrApp, EVTMASK_RADIO_TX);
    PillRegisterEvtChange(&EvtLstnrApp, EVTMASK_PILLCHANGE);

    while(1) {
        chThdSleepMilliseconds(999);
//        Rslt1 = rLevel1.AddPktToTx(RDEV_BOTTOM_ID+1, Buf, PktSZ, &Rslt2);
//        Uart.Printf("> %u\r", Rslt1);
//
        chEvtWaitOne(EVTMASK_PILLCHANGE);
        //Uart.Printf("===");
        if(Pill[0].Connected)
            if(Pill[0].Read(Buf, 4) == OK)
                Uart.Printf("%A\r", Buf, 4, ' ');

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
void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    //Uart.Printf("Rx: %X\r", CmdCode);
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        case CMD_RTX:
            Ack(OK);    // Reply Ack now
            // Setup transmission
            break;

        case CMD_PILL_WRITE:
            // ...
            // Reply result
            Ack(FAILURE);
            break;

        case CMD_PILL_READ:
            Ack(FAILURE);
            break;

        default: break;
    }
}

// =============================== App class ===================================
void App_t::Init() {
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}
