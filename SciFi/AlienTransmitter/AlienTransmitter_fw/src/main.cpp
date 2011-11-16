/*
 * main.cpp
 *
 *  Created on: 27.08.2011
 *      Author: Kreyl
 */

#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_exti.h"
#include "misc.h"
#include "delay_util.h"
#include "cc2500.h"
#include "uart.h"

void EVENT_NewPacket(void);
void GeneralInit(void);

int main(void) {
    GeneralInit();

    uint32_t tmr;
    while (1) {
        //CC.Task();
        if(Delay.Elapsed(&tmr, 999))
            klPrintf("r\r");
    }
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();
    //CC.Init();
    //CC.EvtNewPkt = EVENT_NewPacket;
#ifdef CC_MODE_RX
    klPrintf("\rReceiver\r");
#else
    klPrintf("\rTransmitter\r");
#endif
}

void EVENT_NewPacket(void) {
    klPrintf("RSSI: %i; LQI:%u\r", (int8_t)CC.RX_Pkt.RSSI, CC.RX_Pkt.LQI);
}
