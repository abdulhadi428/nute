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

static EventListener EvtLstnrAppRadioRx, EvtLstnrAppRadioTx;
static rPktIDState_t PktState;
static rPktWithData_t<RRX_PKT_DATA_SZ> SRxPkt;

// Prototypes
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length);

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 128);
static void AppThread(void *arg) {
    chRegSetThreadName("App");
    // Register Radio evts
    uint32_t EvtMsk;
    rLevel1.RegisterEvtRx(&EvtLstnrAppRadioRx, EVTMASK_RADIO_RX);
    rLevel1.RegisterEvtTx(&EvtLstnrAppRadioTx, EVTMASK_RADIO_TX);

    while(1) {
        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_RADIO_TX);
        // Serve event
        if(EvtMsk & EVTMASK_RADIO_RX) {
            while(rLevel1.GetReceivedPkt(&SRxPkt) == OK)
                Uart.Cmd(RPL_RRX, (uint8_t*)&SRxPkt, (sizeof(SRxPkt.rID) + sizeof(SRxPkt.Length) + SRxPkt.Length));
        } // if evtmsk

        if(EvtMsk & EVTMASK_RADIO_TX) {
            while(rLevel1.GetTransmittedState(&PktState) == OK) {
                Uart.Cmd(RPL_RTX, (uint8_t*)&PktState, sizeof(rPktIDState_t));
            }
        }
    } // while 1
}

//=========================== Command processing ===============================
void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

static uint8_t SBuf[252];
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    //Uart.Printf(">%02X; %A\r", CmdCode, PData, Length, ' ');
    uint8_t b, b2;
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        // ==== Radio ====
        case CMD_SET_GATE_NUM:
            SBuf[0] = PData[0];   // Gate number
            if(SBuf[0] < RGATE_CNT) {
                rLevel1.SetID(SBuf[0]);
                SBuf[1] = OK;
            }
            else SBuf[1] = FAILURE;
            Uart.Cmd(RPL_SET_GATE_NUM, SBuf, 2);
            break;
        case CMD_GET_GATE_NUM:
            SBuf[0] = (uint8_t)rLevel1.GetID();
            Uart.Cmd(RPL_GET_GATE_NUM, SBuf, 1);
            break;

        case CMD_RTX:
            b = PData[0];   // Armlet ID
            if((b >= RDEV_BOTTOM_ID) and (b <= RDEV_TOP_ID)) {
                rLevel1.AddPktToTx(b, &PData[1], (Length-1));
            } // if addr ok
            break;

        // ==== Pills ====
        case CMD_PILL_STATE:
            b = PData[0];   // Pill address
            if(b <= 7) SBuf[1] = Pill[b].CheckIfConnected();
            SBuf[0] = b;
            Uart.Cmd(RPL_PILL_STATE, SBuf, 2);
            break;
        case CMD_PILL_WRITE:
            b = PData[0];
            if(b <= 7) SBuf[1] = Pill[b].Write(&PData[1], Length-1);
            SBuf[0] = b;
            Uart.Cmd(RPL_PILL_WRITE, SBuf, 2);
            break;
        case CMD_PILL_READ:
            b = PData[0];           // Pill address
            b2 = PData[1];          // Data size to read
            if(b2 > 250) b2 = 250;  // Check data size
            if(b <= 7) SBuf[1] = Pill[b].Read(&SBuf[2], b2);
            SBuf[0] = b;
            if(SBuf[1] == OK) Uart.Cmd(RPL_PILL_READ, SBuf, b2+2);
            else Uart.Cmd(RPL_PILL_READ, SBuf, 2);
            break;

        // ==== Pin ====
        case CMD_PIN:
            SBuf[0] = PData[0];     // Pin ID
            SBuf[1] = OK;
            b = PData[1];
            if     (b == 0x00) Pin.Low();
            else if(b == 0x01) Pin.High();
            else if(b == 0x02) Pin.Pulse(((uint32_t)PData[2]) * 100);
            else SBuf[1] = CMD_ERROR;
            Uart.Cmd(RPL_SET_GATE_NUM, SBuf, 2);
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

        case 0xF1:  // Check if AHB ok
            if(Clk.AHBFreqHz == 12000000) SBuf[0] = 0;
            else SBuf[0] = 1;
            Uart.Cmd(0xF2, SBuf, 1);
            break;

        default:
            Ack(CMD_ERROR);
            break;
    } // switch
}

// =============================== App init ====================================
void AppInit() {
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
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

