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
#include "adc.h"

#include "led_table.h"

// Prototypes
static inline void Init();
static void SendDataTask();

// ============================== Implementation ===============================
int main(void) {
    Init();
    while(1) {
        //Led.Task();
        CC.Task();
        Uart.Task();
        SendDataTask();
        Adc.Task();
    }
}

static inline void Init() {
	InitClock(clk8MHzInternal);
    klJtagDisable();

    Delay.Init();
    Uart.Init(115200);
    Uart.Printf("\rMagicOrb Control\r");

    Adc.Init();

    // Setup CC
    CC.Init();
    CC.SetChannel(CC_CHNL);

    // Switch on Pwr LED
    klGpioSetupByN(GPIOB, 12, GPIO_Mode_Out_PP);
    klGpioSetByN(GPIOB, 12);
}

uint32_t SDTmr;
void SendDataTask() {
    if(Delay.Elapsed(&SDTmr, 7)) {
        //Uart.Printf("%u; %u; %u; %u\r", R[0], R[1], R[2], R[3]);
        R[0] >>= 4;
        R[1] >>= 4;
        R[2] >>= 4;
        R[3] >>= 4;

//        CC.TX_Pkt.R = (R[0] < LED_TABLE_SZ)? LedTable[R[0]] : 255;
//        CC.TX_Pkt.G = (R[1] < LED_TABLE_SZ)? LedTable[R[1]] : 255;
//        CC.TX_Pkt.B = (R[2] < LED_TABLE_SZ)? LedTable[R[2]] : 255;
//        CC.TX_Pkt.W = (R[3] < LED_TABLE_SZ)? LedTable[R[3]] : 255;

        CC.TX_Pkt.R = R[0];
        CC.TX_Pkt.G = R[1];
        CC.TX_Pkt.B = R[2];
        CC.TX_Pkt.W = R[3];
        //Uart.Printf("%u; %u; %u; %u\r", CC.TX_Pkt.R, CC.TX_Pkt.G, CC.TX_Pkt.B, CC.TX_Pkt.W);

        CC.SendPkt();
    }
}
