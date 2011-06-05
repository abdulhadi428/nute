/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "i2c_mgr.h"
#include "leds_pca.h"

#include "uart.h"
#include "delay_util.h"

void GeneralInit(void);

int main(void) {
    Uart.Init();
    Delay.ms(100);
    Uart.PrintString("Let's rock!\r");

    GeneralInit();
    // Forever
    while(1) {

    }
}


void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Init peripherial
    i2cMgr.Init();
    Leds.Init();
    Delay.Init();
}


