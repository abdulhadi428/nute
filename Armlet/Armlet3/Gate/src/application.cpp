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

static EventListener EvtLstnrApp;
static rPktIDState_t PktState;
static rPktWithData_t<RRX_PKT_DATA_SZ> SRxPkt;

// Prototypes
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length);

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 128);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");
    // Register Radio evts
    uint32_t EvtMsk;
    rLevel1.RegisterEvtRx(&EvtLstnrApp, EVTMASK_RADIO_RX);
    rLevel1.RegisterEvtTx(&EvtLstnrApp, EVTMASK_RADIO_TX);

    while(1) {
        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_RADIO_TX);
        // Serve event
        if(EvtMsk & EVTMASK_RADIO_RX) {
            while(rLevel1.GetReceivedPkt(&SRxPkt) == OK)
                Uart.Cmd(RPL_RRX, (uint8_t*)&SRxPkt, (sizeof(SRxPkt.rID) + sizeof(SRxPkt.Length) + SRxPkt.Length));   // SBuf[1] contains length
        } // if evtmsk

        if(EvtMsk & EVTMASK_RADIO_TX) {
            while(rLevel1.GetTransmittedState(&PktState) == OK) {
                Uart.Cmd(RPL_RTX, (uint8_t*)&PktState, sizeof(rPktIDState_t));
            }
        }
    } // while 1
    return 0;
}

//=========================== Command processing ===============================
void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

static uint8_t SBuf[252];
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    //Uart.Printf(">%02X; %A\r", CmdCode, PData, Length, ' ');
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
            b = PData[0];   // Armlet ID
            if((b >= RDEV_BOTTOM_ID) and (b <= RDEV_TOP_ID)) {
                Rslt = rLevel1.AddPktToTx(b, &PData[1], (Length-1));
            } // if addr ok
            Ack(Rslt);    // Reply Ack now
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
            b = PData[0];           // Pill address
            b2 = PData[1];          // Data size to read
            if(b2 > 250) b2 = 250;  // Check data size
            if(b <= 7) Rslt = Pill[b].Read(&SBuf[1], b2);
            SBuf[0] = Rslt;
            if(Rslt == OK) Uart.Cmd(RPL_PILL_READ, SBuf, b2+1);
            else Uart.Cmd(RPL_PILL_READ, SBuf, 1);
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

        case 0xF0:
            b2 = PData[0];
            for(b=0; b<b2; b++) {
                Uart.Cmd(0xF1, &b, 1);
                Uart.Cmd(0xF1, &b, 1);
                Uart.Cmd(0xF1, &b, 1);
                chThdSleepMilliseconds(21);
            }
            break;

        default:
            Ack(CMD_ERROR);
            break;
    } // switch
}

// =============================== App init ====================================
void AppInit() {
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}

// ================================= Pin control ===============================
Pin_t Pin;
void PinTmrCallback(void *p) { Pin.Low(); }

void Pin_t::Pulse(uint32_t ms) {
    chVTReset(&ITmr);
    High();
    // Start timer to switch off
    chVTSet(&ITmr, MS2ST(ms), PinTmrCallback, NULL);
}

