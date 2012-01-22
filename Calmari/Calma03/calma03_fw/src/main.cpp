/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "rilma.h"


void GeneralInit(void);

int main(void) {
    GeneralInit();

    Rilma.SmoothOn();
    //uint32_t tmr;
    while (1) {
        //if(Delay.Elapsed(&tmr, 999)) klPrintf("r\r");
    }
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    Rilma.Init();
    UART_Init();
    klPrintf(" Calma03\r");
}
