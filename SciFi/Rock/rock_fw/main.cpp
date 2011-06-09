/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "i2c_mgr.h"
#include "leds_pca.h"
#include "delay_util.h"
#include "sd.h"
#include "vs.h"

#include "uart.h"

void GeneralInit(void);

int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Let's rock!\r");

    GeneralInit();

    //Leds.FadeInAll();

    // Forever
    while(1) {
        //Leds.Task();

    }
}


void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Init peripherial
    //i2cMgr.Init();
    //Leds.Init();
    Delay.Init();
    // Sound etc.
    //SD.Init();
    Vs.Init();

    uint16_t IData;
    Vs.Enable();

    if (Vs.CmdRead(VS_REG_MODE, &IData) == VS_OK) {
        UART_StrHex16("VS Mode: ", IData);
    }

    if (Vs.CmdRead(VS_REG_CLOCKF, &IData) == VS_OK) {
        UART_StrHex16("VS Clk: ", IData);
    }



}


