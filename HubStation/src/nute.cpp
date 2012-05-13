/*
 * nute.cpp
 *
 *  Created on: 06.05.2012
 *      Author: kreyl
 */

#include "nute.h"
#include "cc1101.h"

#define RSSI_DUMMY  0x7F

void Nute_t::Init() {
    IState = nsIdle;
}

void Nute_t::Task() {
    switch (IState) {
        case nsSearch:
            if (ISearchState == ssDoTx) {   // Prepare pkt and transmit it
                if (!CC.IsIdle()) return;
                CC.TX_Pkt.Address = IPTixe->Address;
                CC.TX_Pkt.LastRxRawRssi = RSSI_DUMMY;
                CC.TX_Pkt.Cmd = NUTE_CMD_PING;
                // Setup output power
                CC.SetPower(IPTixe->TxPwrLevel);
                klPrintf("Pwr: %u\r", IPTixe->TxPwrLevel);
                // Start transmission
                CC.Transmit();
                Delay.Reset(&ITmr);
                ISearchState = ssDoRx;
            }
            else {  // Do RX
                if (CC.NewPktRcvd) {
                    // Check if correct Pkt - FIXME
                    IState = nsIdle;
                    // Setup tixe
                    IPTixe->LastRxRawRssi = CC.RX_Pkt.LastRxRawRssi;
                    IPTixe->Online = true;
                    if(IPTixe->Callback != 0) IPTixe->Callback();
                }
                // Check if timeout
                else if (Delay.Elapsed(&ITmr, REPLY_WAITTIME)) {
                    CC.EnterIdle();
                    klPrintf("t\r");
                    if (IPTixe->TxPwrLevel == HIGHEST_PWR_LVL_ID) { // No answer even at top power
                        IState = nsIdle;
                        if(IPTixe->Callback != 0) IPTixe->Callback();
                    }
                    else {  // Top not achieved
                        IPTixe->TxPwrLevel++;
                        ISearchState = ssDoTx;
                    }
                }
            }
            break;

        default: break;
    }
}

void Nute_t::Search(Tixe_t *PTixe) {
    // Reset tixe values
    PTixe->LastRxRawRssi = RSSI_DUMMY;
    PTixe->TxPwrLevel = LOWEST_PWR_LVL_ID;
    PTixe->Online = false;
    // Prepare to search
    IPTixe = PTixe;
    IState = nsSearch;
    ISearchState = ssDoTx;
    CC.TX_Pkt.PktID++;
}


