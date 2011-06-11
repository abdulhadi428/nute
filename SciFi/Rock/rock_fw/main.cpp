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
#include "cc1101.h"
#include "lcd110x.h"

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
        //Vs.Task();
        //CC.Task();
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

//    uint16_t IData;
//    Vs.Enable();
//
//    if (Vs.CmdRead(VS_REG_MODE, &IData) == VS_OK) {
//        UART_StrHex16("VS Mode: ", IData);
//    }

    CC.Init();

    Lcd.Init();
    //Lcd.PrintString(0, 0, "Aiya Feanaro!", NotInverted);
    //for (uint8_t i=0; i<8; i++)
        Lcd.PrintString(0, 0, "Aiya Feanaro!", NotInverted);

//    uint8_t a = 0xC5;
//
//    uint32_t r1;
//
//    __ASM volatile ("rbit %0, %1" : "=r" (r1) : "r" (a) );
//
//    __ASM volatile ("rev %0, %1" : "=r" (r1) : "r" (r1) );
//
//
//    UART_StrHex8("a: ", a);
//    UART_StrHex8("r: ", r1);



}

// ================================== Events ===================================
void EVENT_NewPacket(void) {

}
