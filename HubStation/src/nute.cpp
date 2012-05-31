/*
 * nute.cpp
 *
 *  Created on: 06.05.2012
 *      Author: kreyl
 */

#include "nute.h"
#include "cc1101.h"
#include "led.h"

Nute_t Nute;

void Nute_t::Init(uint8_t ASelfAddr) {
    ITask = ntIdle;
    TX_Pkt.AddrFrom = ASelfAddr;
}

void Nute_t::Task() {
    switch (ITask) {
        case ntSearch:  DoSearch(); break;
        case ntWaitCmd: DoWaitCmd(); break;

        default: break;
    }
}

// ============================ Commands =======================================
#ifdef NUTE_MODE_STATION
void Nute_t::Search(Tixe_t *PTixe) {
    IPTixe = PTixe;
    // Reset tixe values
    IPTixe->PwrID = LOWEST_PWR_LVL_ID;
    IPTixe->IsOnline = false;
    // Prepare to search
    ITask = ntSearch;
    ISearchState = ssTxCmd;
}
#endif

// ============================ Proceedings ====================================
void Nute_t::DoWaitCmd() {
//case nsIdle:
//    CC.Receive();
//    Delay.Reset(&ITimer);
//    IState = nsWaitingRx;
//    break;
//
//case nsWaitingRx:
//    if (Delay.Elapsed(&ITimer, RECALIBRATE_DELAY)) {   // Recalibrate
//        CC.EnterIdle();
//        IState = nsIdle;
//    }
//    break;

}

#ifdef NUTE_MODE_STATION
void Nute_t::DoSearch() {
    switch (ISearchState) {
        case ssTxCmd:    // Prepare pkt and transmit it
            TX_Pkt.AddrTo = IPTixe->Address;
            TX_Pkt.PwrID = IPTixe->PwrID;
            TX_Pkt.Cmd = NUTE_CMD_PING;
            // Setup output power
            CC.SetPower(IPTixe->PwrID);
            klPrintf("Pwr: %u\r", IPTixe->PwrID);
            // Start transmission
            CC.Transmit();
            Delay.Reset(&ITimer);
            ISearchState = ssWaitRx;  // Receive will be switched on at TxEnd IRQ handler
            break;

        case ssWaitRx: // Check if timeout
            if (Delay.Elapsed(&ITimer, REPLY_WAITTIME)) {
                CC.EnterIdle();
                if (IPTixe->PwrID == HIGHEST_PWR_LVL_ID) { // No answer even at top power, get out
                    ITask = ntIdle;
                    if(IPTixe->Callback != 0) IPTixe->Callback();
                }
                else {  // Top not achieved
                    IPTixe->PwrID++;
                    ISearchState = ssTxCmd;
                }
            } // if timeout
            break;

        case ssCompleted:
            AdjustPwr(&(IPTixe->PwrID));
            ITask = ntIdle;
            IPTixe->IsOnline = true;
            if(IPTixe->Callback != 0) IPTixe->Callback();
            break;
    } // switch
}


// ============================= Pkt events ====================================
void Nute_t::HandleNewPkt() {
    klPrintf("NewPkt\r");
    switch (ITask) {
        case ntSearch:
            // Check if correct Pkt - FIXME
            ISearchState = ssCompleted;
            break;

        case ntWaitCmd:

            break;

        default: break;
    }

}
void Nute_t::HandleTxEnd() {
    CC.Receive();
    Delay.Reset(&ITimer);
}


#endif

// ============================ Tixe ===========================================
#ifdef NUTE_MODE_TIXE
void Nute_t::HandleNewPkt() {
    klPrintf("NewPkt\r");
    Delay.Reset(&ITimer);           // Reset recalibrate timer
    uint8_t PwrID = RX_Pkt.PwrID;   // Initially, transmit at same power as transmitter
    AdjustPwr(&PwrID);              // Adjust power to transmit at needed power
    // ==== Reply to cmd ====
    // Common preparations
    TX_Pkt.AddrTo = RX_Pkt.AddrFrom;
    TX_Pkt.PwrID = PwrID;
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
    klPrintf("Pwr: %u\r", PwrID);
    // Start transmission
    CC.Transmit();
    IState = nsTransmitting;
}

void Nute_t::HandleTxEnd() {
    CC.Receive();
    Delay.Reset(&ITimer);
    IState = nsWaitingRx;
}

#endif

// ============================ Common =========================================
void Nute_t::AdjustPwr(uint8_t *PPwrID) {
    int16_t RSSI_dBm = CC.RSSI_dBm(RX_Pkt.RSSI);
    int16_t Diff = (DESIRED_RSSI - RSSI_dBm) / 4;   // every single ID mean 4dB difference
    int16_t NewPwrID = *PPwrID + Diff;
    if      (NewPwrID > HIGHEST_PWR_LVL_ID) NewPwrID = HIGHEST_PWR_LVL_ID;
    else if (NewPwrID < LOWEST_PWR_LVL_ID)  NewPwrID = LOWEST_PWR_LVL_ID;
    klPrintf("Pwr: RSSI=%i; Old=%u; New=%u\r", RSSI_dBm, *PPwrID, (uint8_t)NewPwrID);
    *PPwrID = (uint8_t)NewPwrID;
}
