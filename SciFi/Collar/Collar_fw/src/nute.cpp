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

//extern LedBlinkInverted_t Led;

void Nute_t::Init(uint8_t ASelfAddr) {
    IState = nsIdle;
    TX_Pkt.AddrFrom = ASelfAddr;
}

void Nute_t::Task() {
    switch (IState) {
#ifdef NUTE_MODE_STATION
        case nsSearch: DoSearch(); break;
#else   // Tixe
        case nsIdle:
            CC.Receive();
            Delay.Reset(&ITimer);
            IState = nsWaitingRx;
            break;

        case nsWaitingRx:
            if (Delay.Elapsed(&ITimer, RECALIBRATE_DELAY)) {   // Recalibrate
                CC.EnterIdle();
                IState = nsIdle;
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

// ============================ Commands =======================================
#ifdef NUTE_MODE_STATION
void Nute_t::Search(Tixe_t *PTixe) {
    Led.Off();
    IPTixe = PTixe;
    // Reset tixe values
    IPTixe->PwrID = LOWEST_PWR_LVL_ID;
    IPTixe->IsOnline = false;
    // Prepare to search
    IState = nsSearch;
    ISearchState = ssDoTx;
}
#endif

// ============================ Proceedings ====================================
#ifdef NUTE_MODE_STATION
void Nute_t::DoSearch() {
    if (ISearchState == ssDoTx) {   // Prepare pkt and transmit it
        TX_Pkt.AddrTo = IPTixe->Address;
        TX_Pkt.PwrID = IPTixe->PwrID;
        TX_Pkt.Cmd = NUTE_CMD_PING;
        // Setup output power
        CC.SetPower(IPTixe->PwrID);
        klPrintf("Pwr: %u\r", IPTixe->PwrID);
        // Start transmission
        CC.Transmit();
        NewPktRcvd = false;
        Delay.Reset(&ITmr);
        ISearchState = ssDoRx;  // Receive will be switched on at TxEnd IRQ handler
    }
    else {  // Do RX
        if (NewPktRcvd) {
            AdjustPwr(&(IPTixe->PwrID));
            // Check if correct Pkt - FIXME
            IState = nsIdle;
            IPTixe->IsOnline = true;
            if(IPTixe->Callback != 0) IPTixe->Callback();
        }
        // Check if timeout
        else if (Delay.Elapsed(&ITmr, REPLY_WAITTIME)) {
            CC.EnterIdle();
            if (IPTixe->PwrID == HIGHEST_PWR_LVL_ID) { // No answer even at top power, get out
                IState = nsIdle;
                if(IPTixe->Callback != 0) IPTixe->Callback();
            }
            else {  // Top not achieved
                IPTixe->PwrID++;
                ISearchState = ssDoTx;
            }
        } // if timeout
    } // do rx
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
