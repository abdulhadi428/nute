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
#include "images.h"

#include "uart.h"
#include "acc_mma.h"

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

    // Forever
    while(1) {
        if(Delay.Elapsed(&FTimer, 504)) {
            Acc.ReadAccelerations();
            UART_StrInt("X: ", Acc.Accelerations.xMSB);
            UART_StrInt("Y: ", Acc.Accelerations.yMSB);
            UART_StrInt("Z: ", Acc.Accelerations.zMSB);
            UART_NewLine();
        }

        i2cMgr.Task();
        //Leds.Task();
        //Vs.Task();
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

    Acc.Init();

    Lcd.Init();
    Lcd.DrawImage(0,0, ImageLogo, NotInverted);

}

// ================================== Events ===================================
// RF: new packet received
void EVENT_NewPacket(void) {

}
