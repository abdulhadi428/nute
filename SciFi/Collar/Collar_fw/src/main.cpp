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
#include "nute.h"
#include "gps.h"
#include "collar.h"
#include "areas.h"

LedBlinkInverted_t Led;

// Prototypes
void GeneralInit(void);
void CollarStateHandler(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    while (1) {
        CC.Task();
        Nute.Task();
        Gps.Task();
        CmdUnit.Task();     // Uart
        CollarStateHandler();
        Led.Task();
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

    Gps.Init();

    Led.Init(GPIOB, 1);
    Led.Blink(999);

    Nute.Init(72);
    Situation = &Nute.TX_Pkt.Situation;
    CollarState = COLSTATE_OK;
    Areas.Init();

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(Nute.TX_Pkt.AddrFrom);

    CmdUnit.Printf("\rCollar\r");
}

// =================================== States ==================================
uint8_t CollarState;
uint8_t CollarStateCmd;
// State changes
void EnterState(uint8_t ANewState);

void CollarStateHandler(void) {
    // Check areas
    if (Situation->IsFixed) {   // if GPS ok
        uint8_t CurrentAreaIndx;
        bool IsInRestrictedArea = true;
        if (Areas.CoordToAreaIndx(Situation->Lattitude, Situation->Longitude, &CurrentAreaIndx)) {  // if we somewhere in areas
            IsInRestrictedArea = Areas.IsAreaRestricted(CurrentAreaIndx);
        }
        // Enter Delay if in restricted area
        if (IsInRestrictedArea and (CollarState == COLSTATE_OK)) EnterState(COLSTATE_DELAY_BY_AREA);
        // Enter Ok if moved out of restricted area, but do not if delay was by Cmd
        else if (!IsInRestrictedArea and (CollarState == COLSTATE_DELAY_BY_AREA)) EnterState(COLSTATE_OK);
    }

    // Check collar
    bool CollarIsBroken = false;    //FIXME
    if (CollarIsBroken and ((CollarState == COLSTATE_OK) or (CollarState == COLSTATE_DELAY_BY_AREA) or (CollarState == COLSTATE_DELAY_BY_CMD))) EnterState(COLSTATE_BOOM);

    // Check radio cmd
    if(CollarStateCmd == COLSTATE_CMD_BOOM) {
        if((CollarState == COLSTATE_OK) or (CollarState == COLSTATE_DELAY_BY_AREA) or (CollarState == COLSTATE_DELAY_BY_CMD)) EnterState(COLSTATE_BOOM);
    }
    else if(CollarStateCmd == COLSTATE_CMD_DELAY) {
        if(CollarState == COLSTATE_OK) EnterState(COLSTATE_DELAY_BY_CMD);
    }
    else if(CollarStateCmd == COLSTATE_CMD_BE_OK) {
        EnterState(COLSTATE_OK);
    }

    // Maintain state
    if ((CollarState == COLSTATE_DELAY_BY_AREA) or (CollarState == COLSTATE_DELAY_BY_CMD)) {
        // FIXME
    }
}

// Called only by state change. Switches state, does not maintain it.
void EnterState(uint8_t ANewState) {
    CollarState = ANewState;
    switch (CollarState) {
        case COLSTATE_OK:
            break;

        case COLSTATE_BOOM:
            break;

        case COLSTATE_DELAY_BY_AREA:
        case COLSTATE_DELAY_BY_CMD:
            break;

        default: break;
    }
}

