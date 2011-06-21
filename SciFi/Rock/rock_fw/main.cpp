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
#include "sensors.h"

#include "lcd110x.h"
#include "images.h"

#include "uart.h"

void GeneralInit(void);

int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Let's rock!\r");

    GeneralInit();

    // ==== Main cycle ====
    while(1) {
        i2cMgr.Task();
        ESnd.Task();
        ESns.Task();
        Leds.Task();
        //CC.Task();
    } // while(1)
}

void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Init peripheral
    i2cMgr.Init();
    Delay.Init();
    CC.Init();
    // Leds
    Leds.Init();
    Leds.FadeInAll();
    // Sensors
    ESns.Init();
    // LCD
    Lcd.Init();
    Lcd.DrawImage(0,0, ImageLogo, NotInverted);
    // Sound
    SD.Init();
    Vs.Init();
    ESnd.Init();
    ESnd.Play("alive.wav");
}

// ================================== Events ===================================
// RF: new packet received
void EVENT_NewPacket(void) {

}

void EVENT_SensorsStateChanged(void) {
    SnsVerbose();
}