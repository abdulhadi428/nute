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
#include "areas.h"

LedBlinkInverted_t Led;

// Prototypes
void GeneralInit(void);
void InterrogationTask(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    while (1) {
        //Led.Task();
        CC.Task();
        Nute.Task();
        CmdUnit.Task(); // UART cmds
        InterrogationTask();
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
    AreaList.Init();

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

    // Find next Indx which is to be found. Every time iterate throug lis untel such Indx is not found
    for(uint8_t i=0; i<TIXE_COUNT; i++) {   // Iterate through all ids only once
        if(++indx > TIXE_COUNT-1) indx=0;
        if(Tixe[indx].IsToBeFound) {
            if(Tixe[indx].IsOnline) {
                // Check if enough time passed since last ping
                if(Delay.Elapsed(&Tixe[indx].Timer, 900)) Nute.Ping(indx);
            }
            else Nute.Search(indx); // Search as fast as possible
            break;
        } // if is to be found
    } // for
}

void Nute_t::HandleTixeReply(uint8_t AIndx) {
    if (Tixe[AIndx].IsOnline) {
        Situation_t *PStn = &Tixe[AIndx].Situation;
        // $C,ID,PwrID,Battery,State,HHMMSS,Latitude,Longitude,IsFixed,SatelliteCount,Precision
        CmdUnit.Printf("$C,%u,%u,%u,%u,%u2%u2%u2,%i,%i,%u,%u,%u\r",
                AIndx,
                Tixe[AIndx].PwrID,
                PStn->Battery,
                PStn->State,
                PStn->Time.H, PStn->Time.M, PStn->Time.S,
                PStn->Latitude, PStn->Longitude,
                PStn->IsFixed, PStn->SatCount, PStn->Precision
                );
    }
    else CmdUnit.Printf("No answer\r");
}

// ========================== Commands handling ================================
inline bool CharIsDecNumber(char c) {
    return ((c >= '0') and (c <= '9'));
}
inline bool CharIsHexNumber(char c) {
    return (((c >= '0') and (c <= '9')) or ((c >= 'A') and (c <= 'F')));
}

bool TryCommaStrToUint(char **Src, uint32_t *AResult) {
    uint32_t Fres = 0;
    char *S = *Src, c;
    for (uint8_t i=0; i<10; i++) {
        c = *S++;
        if ((c == 0) or (c == ',')) break;  // End of string
        if(!CharIsDecNumber(c)) return false;
        Fres = Fres*10 + (c - '0');
    }
    *AResult = Fres;
    *Src = S;
    return true;
}
bool TryHexToByte(char **Src, uint8_t *AByte) {
    char *S = *Src, c;
    uint8_t b;
    c = *S++;
    if(!CharIsHexNumber(c)) return false;
    b = ((c >= '0') and (c <= '9'))? (c - '0') : ((c - 'A') + 10);
    b <<= 4;
    c = *S++;
    if(!CharIsHexNumber(c)) return false;
    b += ((c >= '0') and (c <= '9'))? (c - '0') : ((c - 'A') + 10);
    *AByte = b;
    *Src = S;
    return true;
}


void CmdUnit_t::NewCmdHandler() {
    if (RXBuf[0] != '$') { Printf("$ERROR0\r"); return; }
    // Kick
    if (RXBuf[1] == 'K') Printf("$Ok\r");

    // Signal
    else if (RXBuf[1] == 'S') {
        if (RXBuf[2] != ',') { Printf("$ERROR1\r"); return; }
        char *S = &RXBuf[3];
        uint32_t FIndx=0, FSignal=0;
        if (!CharIsDecNumber(*S))             { Printf("$ERROR2\r"); return; }
        if (!TryCommaStrToUint(&S, &FIndx))   { Printf("$ERROR3\r"); return; }
        if (!CharIsDecNumber(*S))             { Printf("$ERROR4\r"); return; }
        if (!TryCommaStrToUint(&S, &FSignal)) { Printf("$ERROR5\r"); return; }
        Printf("\rID: %u; S: %u\r", FIndx, FSignal);
        if (FIndx > 49)  { Printf("$ERROR6\r"); return; }
        if (FSignal > 3) { Printf("$ERROR6\r"); return; }
        // Parsing ok, change tixe
        Tixe[FIndx].StateCmd = FSignal;
        Tixe[FIndx].IsToBeFound = true;
        Printf("$Ok\r");
    }

    // Areas
    else if (RXBuf[1] == 'A') {
        Printf("A ");
        if (RXBuf[2] != ',') { Printf("$ERROR1\r"); return; }
        uint8_t b=0;
        char *S = &RXBuf[2];
        for (uint8_t i=0; i<18; i++) {
            S++;    // next chunk
            if(!CharIsHexNumber(*S))  { Printf("$ERROR2 %u\r", i); return; }
            if(!TryHexToByte(&S, &b)) { Printf("$ERROR3 %u\r", i); return; }
            Printf("%X ", b);
            AreaList.Restriction[i] = b;
        }
        Printf("\r");
        Printf("$Ok\r");
    }

    // Unknown cmd
    else Printf("$ERROR: Unknown\r");
}
