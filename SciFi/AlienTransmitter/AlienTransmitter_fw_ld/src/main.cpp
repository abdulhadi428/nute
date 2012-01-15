/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "led.h"
#include "cc2500.h"

void EVENT_NewPacket(void);
void GeneralInit(void);

int main(void) {
    GeneralInit();

    //uint32_t tmr;
    while (1) {
        CC.Task();
        //if(Delay.Elapsed(&tmr, 999)) klPrintf("r\r");
    }
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();
    CC.Init();
    CC.EvtNewPkt = EVENT_NewPacket;
#ifdef CC_MODE_RX
#else
    klPrintf("\rTransmitter\r");
#endif
}

void EVENT_NewPacket(void) {
    klPrintf("RSSI: %i; LQI:%u\r", (int8_t)CC.RX_Pkt.RSSI, CC.RX_Pkt.LQI);
}
