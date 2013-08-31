/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "cmd_uart.h"
#include "pill.h"

// Prototypes
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length);

//=========================== Command processing ===============================
void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

static uint8_t SBuf[252];
static uint8_t GateNum;
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
//    Uart.Printf(">%02X; %A\r", CmdCode, PData, Length, ' ');
    uint8_t b, b2;
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        // ==== Radio ====
        case CMD_SET_GATE_NUM:
            GateNum = PData[0];   // Gate number
            SBuf[0] = GateNum;
            SBuf[1] = OK;
            Uart.Cmd(RPL_SET_GATE_NUM, SBuf, 2);
            break;
        case CMD_RTX:
            Ack(CMD_ERROR);
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
                chThdSleepMilliseconds(21);
            }
            break;

        default:
            Ack(CMD_ERROR);
            break;
    } // switch
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

