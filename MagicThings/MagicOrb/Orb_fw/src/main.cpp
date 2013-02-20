/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Magic Orb
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "kl_lib.h"
#include "cc2500.h"
#include "led.h"

// Variables
LedRGBW_t Led;

// Prototypes
static inline void Init();
static void RxTask();


// ============================== Implementation ===============================
int main(void) {
    Init();
    while(1) {
        CC.Task();
        Uart.Task();
        RxTask();
    }
}

static inline void Init() {
	InitClock(clk8MHzInternal);
    klJtagDisable();

    Delay.Init();
    Uart.Init(115200);
    Uart.Printf("\rMagicOrb\r");

    // Setup CC
    CC.Init();
    CC.SetChannel(CC_CHNL);

    Led.Init();
}

uint32_t Tmr;
#define SHUTDOWN_DELAY_MS  7002
bool IsFading = false;

void RxTask() {
    if(CC.NewPktRcvd) {
        CC.NewPktRcvd = false;
        IsFading = false;
        Delay.Reset(&Tmr);
        //Uart.Printf("%u; %u; %u; %u\r", CC.RX_Pkt.R, CC.RX_Pkt.G, CC.RX_Pkt.B, CC.RX_Pkt.W);

        Color_t c;
        c.Red = CC.RX_Pkt.R;
        c.Green = CC.RX_Pkt.G;
        c.Blue = CC.RX_Pkt.B;
        c.White = CC.RX_Pkt.W;

        Led.SetColor(c);
    }
    else {
        if(IsFading) {
            if(Delay.Elapsed(&Tmr, 11)) {
                if(Led.CurrentColor == Led.NeededColor) return;
                // Red channel
                if (Led.CurrentColor.Red != Led.NeededColor.Red) {
                    if (Led.NeededColor.Red < Led.CurrentColor.Red) Led.CurrentColor.Red--;
                    else Led.CurrentColor.Red++;
                }
                // Green channel
                if (Led.CurrentColor.Green != Led.NeededColor.Green) {
                    if (Led.NeededColor.Green < Led.CurrentColor.Green) Led.CurrentColor.Green--;
                    else Led.CurrentColor.Green++;
                }
                // Blue channel
                if (Led.CurrentColor.Blue != Led.NeededColor.Blue) {
                    if (Led.NeededColor.Blue < Led.CurrentColor.Blue) Led.CurrentColor.Blue--;
                    else Led.CurrentColor.Blue++;
                }
                // White channel
                if (Led.CurrentColor.White != Led.NeededColor.White) {
                    if (Led.NeededColor.White < Led.CurrentColor.White) Led.CurrentColor.White--;
                    else Led.CurrentColor.White++;
                }
                Led.SetColor(Led.CurrentColor);
            }
        }
        else {
            if(Delay.Elapsed(&Tmr, SHUTDOWN_DELAY_MS)) {
                IsFading = true;
                Led.NeededColor = clBlack;
            }
        }
    }
}
