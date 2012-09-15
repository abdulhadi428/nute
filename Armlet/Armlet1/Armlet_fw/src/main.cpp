/*
 * File:   main.cpp of Armlet1 project
 * Author: Kreyl
 * Date:   2012-09-13 10:11
 *
 */

#include "stm32f10x.h"
#include <stdlib.h>
#include "kl_lib.h"
#include "led.h"
#include "cc1101.h"
#include "lcd1200.h"
#include "beep.h"
#include "battery.h"
#include "interface.h"
#include "kl_time.h"

// Prototypes
void GeneralInit(void);

// ============================ Implementation =================================
int main(void) {
    GeneralInit();

    // ==== Main cycle ====
    while (1) {
    	Uart.Task();
    	Lcd.Task();
    	Battery.Task();
    	Beep.Task();
    	Battery.Task();
    	Interface.Task();
        //CC.Task();

    } // while(1)
}

inline void GeneralInit(void) {
    InitClock(clk2MHzInternal);
    klJtagDisable();

    Delay.Init();
    Beep.Init();
    Beep.SetFreqHz(2007);
    Beep.Squeak(2, 7);

    Uart.Init(115200);
    Uart.Printf("\rArmlet1\r");

    Time.Init();
    Lcd.Init();
    Lcd.Backlight(0);
    Interface.Init();

    Battery.Init();
    // Setup CC
//    CC.Init();
//    CC.SetChannel(0);
//    CC.SetPower(pl0dBm);

    // Get unique ID
    //GetUniqueID(&PktTx.IdArr[0]);
    //Uart.Printf("ID: %X8 %X8 %X8\r", PktTx.IdArr[0], PktTx.IdArr[1], PktTx.IdArr[2]);
}

// ================================= Events ====================================
void CC_t::TxEndHandler() {
#ifdef ALWAYS_RX
//    Receive();
#else
    // Enter RX at zero cycle and at start of first, otherwise sleep
//    if((Sync.CycleCounter == 0) or (Sync.CycleCounter == 1)) Receive();
//    else EnterIdle();
#endif
}

void CC_t::NewPktHandler() {
    //Uart.Printf("NbID: %X8 %X8 %X8\r", PktRx.IdArr[0], PktRx.IdArr[1], PktRx.IdArr[2]);
    //Uart.Printf("dBm: %i\r", CC.RSSI_dBm(PktRx.RSSI));
//    NCounter.Add(PktRx.IdArr);
    //Led.Blink(36, clYellow);  // DEBUG
}


