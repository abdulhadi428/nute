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

LedBlinkInverted_t Led;
uint8_t CollarState;

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    while (1) {
        CC.Task();
        Nute.Task();
        Gps.Task();
        CmdUnit.Task();
        //if(Delay.Elapsed(&Tmr, 999)) {
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
    Led.On();

    Nute.Init(72);
    Situation = &Nute.TX_Pkt.Situation;
    CollarState = COL_OK;

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(Nute.TX_Pkt.AddrFrom);

    CmdUnit.Printf("\rCollar\r");
}
