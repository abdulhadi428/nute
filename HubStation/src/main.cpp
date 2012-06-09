/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "kl_lib.h"
#include "led.h"
#include "cc1101.h"
#include "cc1190.h"
#include "nute.h"

LedBlinkInverted_t Led;

// Prototypes
void GeneralInit(void);
void InterrogationTask(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

//    uint32_t Tmr;
    while (1) {
        //Led.Task();
        CC.Task();
        Nute.Task();
        CmdUnit.Task(); // UART cmds
        InterrogationTask();
//        if(Delay.Elapsed(&Tmr, 200)) CmdUnit.Printf("1234567890123456789012345678901234567890\r");
    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();
    klJtagDisable();

    Delay.Init();
    Delay.ms(63);
    CmdUnit.Init();

    Led.Init(GPIOB, 1);
    Led.On();

    Nute.Init(1);

    Tixe[0].IsToBeFound = true;

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(Nute.TX_Pkt.AddrFrom);

    CmdUnit.Printf("\rCollar station\r");
}

void InterrogationTask(void) {
    if (Nute.State != nsIdle) return;
    static uint8_t indx=TIXE_COUNT;

    for(uint8_t i=0; i<TIXE_COUNT; i++) {   // Iterate through ids only once
        if(++indx > TIXE_COUNT-1) indx=0;
        if(Tixe[indx].IsToBeFound) {
            if(Tixe[indx].IsOnline) Nute.Ping(indx);
            else Nute.Search(indx);
            break;
        } // if is to be found
    } // for
}

void Nute_t::HandleTixeReply(uint8_t AID) {
    if (Tixe[AID].IsOnline) {
        Situation_t *PStn = &Tixe[AID].Situation;
        // $C,ID,PwrID,Battery,State,HHMMSS,Latitude,Longitude,IsFixed,SatelliteCount,Precision
        CmdUnit.Printf("$C,%u,%u,%u,%u,%u2%u2%u2,%i,%i,%u,%u,%u\r",
                AID + TIXE_ADDR_OFFSET,
                Tixe[AID].PwrID,
                0/*PStn->Battery*/,
                PStn->State,
                PStn->Time.H, PStn->Time.M, PStn->Time.S,
                PStn->Lattitude, PStn->Longitude,
                PStn->IsFixed, PStn->SatCount, PStn->Precision
                );
    }
    else CmdUnit.Printf("No answer\r");
}

// ========================== Commands handling ================================
void CmdUnit_t::NewCmdHandler() {
    // Kick
    if (RXBuf[0] == 'K') Printf("$Ok\r");

    // Signal
    else if (RXBuf[0] == 'S') {
        //if (RXBuf[1] == '0')  // Send Ok
        //else if (RXBuf[1] == '1')  // Boom now
        //else if (RXBuf[1] == '2')  // Boom with delay
        Printf("$Ok\r");
    }

    // Remove areas
    else if (RXBuf[0] == 'R') {
        Printf("$Ok\r");
    }
    // Add area
    else if (RXBuf[0] == 'A') {
        Printf("$Ok\r");
    }

    // Unknown cmd
    else Printf("$ERROR: Unknown\r");
}
