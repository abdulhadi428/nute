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
#include "acc_mma.h"
#include "media.h"

#include "lcd110x.h"
#include "images.h"

#include "uart.h"

void GeneralInit(void);

int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Let's rock!\r");

    GeneralInit();

    //Leds.FadeInAll();

    uint32_t FTimer;
    Delay.Reset(&FTimer);
//    uint8_t d='a';

    // ==== Main cycle ====
    while(1) {
        i2cMgr.Task();
        ESnd.Task();
//
        if (ESnd.State == sndStopped) ESnd.Play("the_moon.wav");
        //if (ESnd.State == sndStopped) ESnd.Play("alive.wav");
        //Leds.Task();
        //CC.Task();
    }
}


void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Init peripherial
    i2cMgr.Init();
//    Leds.Init();
//    Leds.FadeInAll();

    Delay.Init();
    CC.Init();

    Acc.Init();

    Lcd.Init();
    Lcd.DrawImage(0,0, ImageLogo, NotInverted);

    SD.Init();
    Vs.Init();
    ESnd.Init();
}

// ================================== Events ===================================
// RF: new packet received
void EVENT_NewPacket(void) {

}
