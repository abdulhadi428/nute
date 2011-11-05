/*
 * main.cpp
 *
 *  Created on: 27.08.2011
 *      Author: Kreyl
 */

#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "delay_util.h"
#include "main.h"
#include "stm32l1xx_exti.h"
#include "misc.h"
#include "klprintf.h"
#include "cc2500.h"

void EVENT_NewPacket(void);

int main(void) {
    Delay.Init();
    klPrintf_Init();
    CC.Init();
    CC.EvtNewPkt = EVENT_NewPacket;

//    uint32_t tmr;

    while (1) {
        CC.Task();
//        if(Delay.Elapsed(&tmr, 999))
//            klPrintf("r\r");

    } // while 1
}

void EVENT_NewPacket(void) {
    klPrintf("RSSI: %i; LQI:%u\r", (int8_t)CC.RX_Pkt.RSSI, CC.RX_Pkt.LQI);
}
