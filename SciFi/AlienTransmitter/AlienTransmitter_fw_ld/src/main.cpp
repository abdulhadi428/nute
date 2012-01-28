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

void GeneralInit(void);

int main(void) {
    GeneralInit();

    while (1) {
        CC.Task();
    }
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();

    CC.Init();
    CC.SetChannel(150);

    klPrintf("\rTransmitter\r");
}
