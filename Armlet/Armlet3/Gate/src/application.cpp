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

//static EventListener EvtLstnrApp;

// Prototypes
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length);

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    //uint8_t Buf[7];

#define PktSZ   4
//    uint8_t Buf[PktSZ], Rslt1 = FAILURE, Rslt2 = FAILURE;
//    for(uint8_t i=0; i<PktSZ; i++) Buf[i] = i+2;
//
    // Events
    //rLevel1.RegisterEvtTx(&EvtLstnrApp, EVTMASK_RADIO_TX);
    //PillRegisterEvtChange(&EvtLstnrApp, EVTMASK_PILLCHANGE);
    //uint32_t r;
    while(1) {
        chThdSleepMilliseconds(999);
//        Rslt1 = rLevel1.AddPktToTx(RDEV_BOTTOM_ID+1, Buf, PktSZ, &Rslt2);
//        Uart.Printf("> %u\r", Rslt1);
//
//        chEvtWaitOne(EVTMASK_PILLCHANGE);
//        //Uart.Printf("===");
//        if(Pill[0].Connected)
//            if(Pill[0].Read(Buf, 4) == OK)
//                Uart.Printf("%A\r", Buf, 4, ' ');

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

static uint8_t IBuf[252];
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    Uart.Printf(">%02X; %A\r", CmdCode, PData, Length, ' ');
    uint8_t b, b2, Rslt;
    Rslt = FAILURE;
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        // ==== Radio ====
        case CMD_SET_GATE_NUM:
            b = PData[0];   // Gate number
            if(b < RGATE_CNT) {
                rLevel1.SetID(b);
                Ack(OK);
            }
            else Ack(CMD_ERROR);
            break;
        case CMD_RTX:
            Ack(OK);    // Reply Ack now
            // Setup transmission
            break;

        // ==== Pills ====
        case CMD_PILL_STATE:
            b = PData[0];
            if(b <= 7) Rslt = Pill[b].CheckIfConnected();
            Ack(Rslt);
            break;
        case CMD_PILL_WRITE:
            b = PData[0];
            if(b <= 7) Rslt = Pill[b].Write(&PData[1], Length-1);
            Ack(Rslt);
            break;
        case CMD_PILL_READ:
            b = PData[0];
            b2 = PData[1];
            if(b2 > 250) b2 = 250;
            if(b <= 7) Rslt = Pill[b].Read(&IBuf[1], b2);
            IBuf[0] = Rslt;
            if(Rslt == OK) Uart.Cmd(RPL_PILL_READ, IBuf, b2+1);
            else Uart.Cmd(RPL_PILL_READ, IBuf, 1);
            break;

        // ==== Pin ====
        case CMD_PIN:
            Rslt = OK;
            b = PData[1];
            if     (b == 0x00) Pin.Low();
            else if(b == 0x01) Pin.High();
            else if(b == 0x02) Pin.Pulse(((uint32_t)PData[2]) * 100);
            else Rslt = CMD_ERROR;
            Ack(Rslt);
            break;

        default:
            Ack(CMD_ERROR);
            break;
    }
}

// =============================== App class ===================================
void App_t::Init() {
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}

// ================================= Pin control ===============================
Pin_t Pin;
void PinTmrCallback(void *p) {
    Pin.Low();
}

void Pin_t::Pulse(uint32_t ms) {
    chVTReset(&ITmr);
    High();
    // Start timer to switch off
    chVTSet(&ITmr, MS2ST(ms), PinTmrCallback, NULL);
}

