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
    klPrintf("\rDetector\r");
}

void EVENT_NewPacket(void) {
    int32_t RSSI_dBm = CC.RX_Pkt.RSSI;
    //if (RSSI_dBm == 0) return;
    if (RSSI_dBm >= 128) RSSI_dBm -= 256;
    RSSI_dBm  = (RSSI_dBm / 2) - 69;
    klPrintf("RSSI: %i; LQI:%u\r", RSSI_dBm, CC.RX_Pkt.LQI);
}
