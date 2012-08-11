/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "kl_lib.h"
#include "led.h"
#include "cc1101.h"

LedRGB_t Led;
void GeneralInit(void);

int main(void) {
    GeneralInit();

    uint32_t Tmr;
//    Color_t Cl = clBlack;

    CC.Receive();
    // ==== Main cycle ====
    while (1) {
    	Uart.Task();
        Led.Task();
        CC.Task();

        if(Delay.Elapsed(&Tmr, 450)) {
//        	if(Cl == clBlack) Cl = clBlue;
//        	else Cl = clBlack;
//        	Led.SetColorSmoothly(Cl);
        	//CC.Transmit();
        }
    } // while(1)
}

inline void GeneralInit(void) {
    InitClock(clk2MHzInternal);
    klJtagDisable();

    Delay.Init();
    Led.Init();

    Uart.Init(115200);
    Uart.Printf("\rTirilde\r");

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(0);   // dummy
    CC.SetPower(plN6dBm);
}

void CC_t::TxEndHandler() {
    Uart.Printf("Tx\r");
}

void CC_t::NewPktHandler() {
    Uart.Printf("Rx\r");
    CC.Receive();
}
