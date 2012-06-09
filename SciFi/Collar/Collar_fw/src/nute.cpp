/*
 * nute.cpp
 *
 *  Created on: 06.05.2012
 *      Author: kreyl
 */

#include "nute.h"
#include "cc1101.h"
#include "led.h"
#include "string.h"
#ifndef NUTE_MODE_STATION
#include "collar.h"
#include "gps.h"
#endif

Nute_t Nute;

#ifdef NUTE_MODE_STATION
Tixe_t Tixe[TIXE_COUNT];
#endif


void Nute_t::Init(uint8_t ASelfAddr) {
    State = nsIdle;
    TX_Pkt.AddrFrom = ASelfAddr;
}

void Nute_t::Task() {
    switch (State) {
#ifdef NUTE_MODE_STATION
        case nsSearch: DoSearch(); break;
        case nsPing:   DoPing();   break;
#else   // Tixe
        case nsIdle:
            CC.Receive();
            Delay.Reset(&ITimer);
            State = nsWaitingRx;
            break;

        case nsWaitingRx:
            if (Delay.Elapsed(&ITimer, RECALIBRATE_DELAY)) {   // Recalibrate
                CC.EnterIdle();
                State = nsIdle;
            }
            break;

//        case nsTransmitting:
//            if (CC.Aim != caTx) {   // Tx completed
//                //klPrintf("no tx\r");
//                //Delay.Reset(&Tmr);
//                IState = nsIdle;
//            }
//            break;
#endif
        // ==== Common ====
        default:
            break;
    } // switch istate
}

// ============================= HubStation ====================================
#ifdef NUTE_MODE_STATION
void Nute_t::Ping(uint32_t ATixeNumber) {
    CmdUnit.Printf("Ping %u\r", ATixeNumber);
    ITixeNumber = ATixeNumber;
    Tixe[ITixeNumber].IsOnline = false;
    State = nsPing;
    IRadioTask = rtDoTx;
    TX_Pkt.AddrTo = ITixeNumber + TIXE_ADDR_OFFSET;
    TX_Pkt.Cmd = NUTE_CMD_PING;
}
inline void Nute_t::DoPing() {
    if (IRadioTask == rtDoTx) {
        // Setup output power
        TX_Pkt.PwrID = Tixe[ITixeNumber].PwrID;
        CC.SetPower(TX_Pkt.PwrID);
        CmdUnit.Printf("Pwr: %u\r", TX_Pkt.PwrID);
        // Start transmission
        CC.Transmit();
        Delay.Reset(&ITimer);
        IRadioTask = rtDoRx;  // Receive will be switched on at TxEnd IRQ handler
    }
    else {  // Do RX
        if (Tixe[ITixeNumber].IsOnline or Delay.Elapsed(&ITimer, REPLY_WAITTIME)) {
            CC.EnterIdle();
            State = nsIdle;
            HandleTixeReply(ITixeNumber);
        }
    } // do rx
}

void Nute_t::Search(uint32_t ATixeNumber) {
    CmdUnit.Printf("Search %u\r", ATixeNumber);
    ITixeNumber = ATixeNumber;
    // Reset tixe values
    Tixe[ITixeNumber].PwrID = LOWEST_PWR_LVL_ID;
    Tixe[ITixeNumber].IsOnline = false;
    // Prepare to search
    State = nsSearch;
    IRadioTask = rtDoTx;
    TX_Pkt.AddrTo = ITixeNumber + TIXE_ADDR_OFFSET;
    TX_Pkt.Cmd = NUTE_CMD_PING;
}
inline void Nute_t::DoSearch() {
    if (IRadioTask == rtDoTx) {
        // Setup output power
        TX_Pkt.PwrID = Tixe[ITixeNumber].PwrID;
        CC.SetPower(TX_Pkt.PwrID);
        CmdUnit.Printf("Pwr: %u\r", TX_Pkt.PwrID);
        // Start transmission
        CC.Transmit();
        Delay.Reset(&ITimer);
        IRadioTask = rtDoRx;  // Receive will be switched on at TxEnd IRQ handler
    }
    else {  // Do RX
        if (Tixe[ITixeNumber].IsOnline) {     // Tixe was found (look HandleNewPkt)
            State = nsIdle;
            HandleTixeReply(ITixeNumber);
        }
        // Check if timeout
        else if (Delay.Elapsed(&ITimer, REPLY_WAITTIME)) {
            CC.EnterIdle();
            if (Tixe[ITixeNumber].PwrID == HIGHEST_PWR_LVL_ID) { // No answer even at top power, get out
                State = nsIdle;
                HandleTixeReply(ITixeNumber);
            }
            else {  // Top not achieved
                Tixe[ITixeNumber].PwrID++;
                IRadioTask = rtDoTx;
            }
        } // if timeout
    } // do rx
}

void Nute_t::HandleNewPkt() {
    //CmdUnit.Printf("NewPkt: %A\r", &RX_Pkt, sizeof(Pkt_t));
    Tixe[ITixeNumber].PwrID = RX_Pkt.PwrID;   // Initially, transmit at same power as transmitter
    AdjustPwr(&(Tixe[ITixeNumber].PwrID));    // Adjust power to transmit at needed one
    Tixe[ITixeNumber].IsOnline = true;
    // Copy data
    memcpy(&Tixe[ITixeNumber].Situation, &RX_Pkt.Situation, sizeof(Situation_t));
}

void Nute_t::HandleTxEnd() {
    CC.Receive();
}

#endif // Station

// ============================ Tixe ===========================================
#ifdef NUTE_MODE_TIXE
void Nute_t::HandleNewPkt() {
    //klPrintf("NewPkt\r");
    Delay.Reset(&ITimer);           // Reset recalibrate timer
    uint8_t PwrID = RX_Pkt.PwrID;   // Initially, transmit at same power as transmitter
    AdjustPwr(&PwrID);              // Adjust power to transmit at needed power
    // ==== Reply to cmd ====
    // Common preparations
    TX_Pkt.AddrTo = RX_Pkt.AddrFrom;
    TX_Pkt.PwrID = PwrID;
    // Place situation to pkt
    Situation->State = CollarState;
    // Cmd-dependant preparations
    switch (RX_Pkt.Cmd) {
        case NUTE_CMD_PING:
            TX_Pkt.Cmd = NUTE_RPL_PING;
            break;

        default: // Say "Command not supported"
            TX_Pkt.Cmd = NUTE_RPL_UNSUPPORTED;
            break;
    } // switch
    // Setup output power
    CC.SetPower(PwrID);
    //klPrintf("Pwr: %u\r", PwrID);
    // Start transmission
    CC.Transmit();
    State = nsTransmitting;
}

void Nute_t::HandleTxEnd() {
    CC.Receive();
    Delay.Reset(&ITimer);
    State = nsWaitingRx;
}

#endif

// ============================ Common =========================================
void Nute_t::AdjustPwr(uint8_t *PPwrID) {
    int16_t RSSI_dBm = CC.RSSI_dBm(RX_Pkt.RSSI);
    int16_t Diff = (DESIRED_RSSI - RSSI_dBm) / 4;   // every single ID mean 4dB difference
    int16_t NewPwrID = *PPwrID + Diff;
    if      (NewPwrID > HIGHEST_PWR_LVL_ID) NewPwrID = HIGHEST_PWR_LVL_ID;
    else if (NewPwrID < LOWEST_PWR_LVL_ID)  NewPwrID = LOWEST_PWR_LVL_ID;
    //CmdUnit.Printf("Pwr: RSSI=%i; Old=%u; New=%u\r", RSSI_dBm, *PPwrID, (uint8_t)NewPwrID);
    *PPwrID = (uint8_t)NewPwrID;
}
