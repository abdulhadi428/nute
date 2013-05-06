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

#include "cmd_uart.h"


// Prototypes
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length);

// =============================== App Thread ==================================
//static WORKING_AREA(waAppThread, 128);
//static msg_t AppThread(void *arg) {
//    (void)arg;
//    chRegSetThreadName("App");
//    // Register Radio evts
//    uint32_t EvtMsk;
//    rLevel1.RegisterEvtRx(&EvtLstnrAppRadioRx, EVTMASK_RADIO_RX);
//    rLevel1.RegisterEvtTx(&EvtLstnrAppRadioTx, EVTMASK_RADIO_TX);
//
//    while(1) {
//
//    } // while 1
//    return 0;
//}

//=========================== Command processing ===============================
void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    //Uart.Printf(">%02X; %A\r", CmdCode, PData, Length, ' ');
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        default:
            Ack(CMD_ERROR);
            break;
    } // switch
}

// =============================== App init ====================================
void AppInit() {


    //chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}


